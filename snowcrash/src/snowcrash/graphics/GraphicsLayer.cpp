
#include "GraphicsLayer.hpp"
#include "snowcrash/application/Layer.hpp"

#include <snowcrash/resource/ResourceRawFile.hpp>
#include <snowcrash/resource/ResourceImage.hpp>
#include <snowcrash/resource/ResourceModel.hpp>

#include <snowcrash/graphics/vulkan/descriptors/Descriptor.hpp>

#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SC
{
	struct PushConstant
	{
		glm::mat4 model;
	};

	GraphicsLayer::GraphicsLayer(Context *context, Window *window)
		: Layer(context), m_window(window) {}

	GraphicsLayer::~GraphicsLayer()
	{
		SC_TRACE("Clean graphics layer");

		m_logicalDevice->DeviceWaitIdle();

		CleanSubrenders();

		delete m_indexBuffer;
		delete m_vertexBuffer;

		delete m_fence;
		delete m_imageAvailableSemaphore;
		delete m_renderFinishedSemaphore;

		delete m_pipeline;
		delete m_layout;
		delete m_descriptorSet;
		delete m_uniformBuffer;
		delete m_textureImage;
		delete m_textureSampler;

		for (int i = 0; i < m_shaderModules.GetIndex(); i++)
		{
			delete m_shaderModules[i];
		}

		delete m_descriptorPool;
		delete m_vertexDescription;
		delete m_renderPass;
		delete m_commandPool;
		delete m_swapchain;
		delete m_logicalDevice;
		delete m_physicalDevice;
		m_instance->DestroyDebugUtils();
		delete m_instance;
	}

	void GraphicsLayer::Init()
	{
		using namespace vulkan;

		m_instance = new vulkan::Instance(m_window->GetHandle());
		m_physicalDevice = new vulkan::PhysicalDevice(m_instance);
		m_logicalDevice = new vulkan::LogicalDevice(m_physicalDevice);
		m_swapchain = new vulkan::Swapchain(m_instance, m_window->GetHandle(), m_physicalDevice, m_logicalDevice);
		m_commandPool = new vulkan::CommandPool(m_physicalDevice, m_logicalDevice, 1);

		m_renderPass = new vulkan::RenderPass(m_swapchain, m_logicalDevice, m_physicalDevice, m_commandPool);

		const auto *vertexShader = GetResource<ResourceRawFile>("vert.spv");
		const auto *fragmentShader = GetResource<ResourceRawFile>("frag.spv");

		m_shaderModules.Add(new ShaderModule(m_logicalDevice, ShaderModule::ShaderType_Vertex, vertexShader->GetBytes(), vertexShader->GetSize()));
		m_shaderModules.Add(new ShaderModule(m_logicalDevice, ShaderModule::ShaderType_Fragment, fragmentShader->GetBytes(), fragmentShader->GetSize()));

		m_vertexDescription = new vulkan::VertexDescription();
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec2);
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);

		const ResourceModel *model = GetResource<ResourceModel>("viking_room.obj");
		m_indexBuffer = new IndexBuffer(m_physicalDevice, m_logicalDevice,
										m_commandPool, model->GetModel()->m_indicies.GetArray(), model->GetModel()->m_indicies.GetIndex() * sizeof(u32));

		m_vertexBuffer = new VertexBuffer(m_physicalDevice, m_logicalDevice,
										  m_commandPool, model->GetModel()->m_vertices.GetArray(), model->GetModel()->m_vertices.GetIndex() * sizeof(ModelVertex));

		ArrayList<DescriptorPool::DescriptorBinding> poolBindings;
		poolBindings.Add(DescriptorPool::DescriptorBinding{Descriptor::DescriptorType::DescriptorType_Uniform, 10});
		poolBindings.Add(DescriptorPool::DescriptorBinding{Descriptor::DescriptorType::DescriptorType_TextureSampler, 10});
		m_descriptorPool = new vulkan::DescriptorPool(m_logicalDevice, poolBindings);

		m_uniformBuffer = new UniformBuffer(m_physicalDevice, m_logicalDevice, m_commandPool, sizeof(UniformBufferData));

		UniformBufferData ubo;
		ubo.model = glm::mat4(1.0f);
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.projection = glm::perspective(glm::radians(45.0f), m_swapchain->GetSwapchainExtent().width / (float)m_swapchain->GetSwapchainExtent().height, 0.1f, 10.0f);
		ubo.projection[1][1] *= -1;
		m_uniformBuffer->CopyDataToBuffer(&ubo, sizeof(ubo));

		const ResourceImage *image = GetResource<ResourceImage>("viking_room.png");
		m_textureImage = new TextureImage2D(m_physicalDevice, m_logicalDevice, m_commandPool, image->GetImage(),
											TextureData{
												image->GetWidth(),
												image->GetHeight(),
												static_cast<VkDeviceSize>(image->GetWidth() * image->GetHeight() * 4)});

		m_textureSampler = new TextureSampler(m_physicalDevice, m_logicalDevice);

		ArrayList<Descriptor *> *m_descriptors = new ArrayList<Descriptor *>();
		m_descriptors->Add(new DescriptorUniform(m_uniformBuffer));
		m_descriptors->Add(new DescriptorTextureSampler(m_textureSampler, m_textureImage->GetImageView()));
		m_layout = new DescriptorLayout(m_logicalDevice, m_descriptors);

		m_descriptorSet = new DescriptorSet(m_physicalDevice, m_logicalDevice, m_descriptorPool, m_layout, 1);
		// delete layout;

		ArrayList<VkPushConstantRange> pushConstants = ArrayList<VkPushConstantRange>();
		VkPushConstantRange pushConstant;
		pushConstant.size = sizeof(float) * 4 * 4;
		pushConstant.offset = 0;
		pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstants.Add(pushConstant);

		m_pipeline = new vulkan::RenderPipeline(m_swapchain, m_physicalDevice, m_logicalDevice,
												vulkan::RenderPipeline::RenderPipelineDef{vulkan::RenderPipeline::Mode_Polygon,
																						  vulkan::RenderPipeline::Depth_ReadWrite,
																						  m_renderPass,
																						  m_vertexDescription,
																						  m_descriptorSet},
												m_shaderModules, pushConstants);

		m_fence = new Fence(m_logicalDevice);
		m_imageAvailableSemaphore = new Semaphore(m_logicalDevice);
		m_renderFinishedSemaphore = new Semaphore(m_logicalDevice);

		InitSubrenders();
	}

	void GraphicsLayer::InitSubrenders()
	{
		for (int i = 0; i < m_subrenders.GetIndex(); i++)
		{
			m_subrenders[i]->Init();
		}
	}

	void GraphicsLayer::CleanSubrenders()
	{
		for (int i = 0; i < m_subrenders.GetIndex(); i++)
		{
			delete m_subrenders[i];
		}
	}

	void GraphicsLayer::Update()
	{
		// Profit
		m_fence->WaitForFence();
		uint32_t imageIndex = m_swapchain->AcquireNextImage(m_imageAvailableSemaphore);
		m_fence->ResetFence();

		vkResetCommandBuffer(m_commandPool->GetCommandBuffer(0), 0);

		VkCommandBuffer buffer = m_commandPool->GetCommandBuffer(0);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;				  // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS)
		{
			assert("failed to begin recording command buffer!");
		}

		// DRAW SHIT HERE

		m_pipeline->BeginRenderPass(buffer, m_renderPass, m_renderPass->GetFramebuffer(imageIndex)->GetHandle(), m_swapchain->GetSwapchainExtent());
		m_vertexBuffer->Bind(buffer);
		m_indexBuffer->Bind(buffer);
		m_pipeline->BindPipeline(buffer);

		m_pipeline->SetViewport(buffer, m_swapchain->GetSwapchainExtent().width, m_swapchain->GetSwapchainExtent().height);
		m_pipeline->SetScissor(buffer, m_swapchain->GetSwapchainExtent());

		PushConstant c;
		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		c.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_pipeline->BindPushConstant<PushConstant>(buffer, VK_SHADER_STAGE_VERTEX_BIT, &c);

		m_pipeline->DrawIndexed(buffer, m_indexBuffer->GetIndiciesCount());

		for (int i = 0; i < m_subrenders.GetIndex(); i++)
		{
			m_subrenders[i]->Render(buffer);
		}

		m_pipeline->EndRenderPass(buffer);

		if (vkEndCommandBuffer(buffer) != VK_SUCCESS)
		{
			assert("failed to record command buffer!");
		}

		// STOP DRAWING STUFF

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore->GetHandle()};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		VkCommandBuffer b = m_commandPool->GetCommandBuffer(0);
		submitInfo.pCommandBuffers = &b;
		VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore->GetHandle()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_logicalDevice->GetGraphicsQueue(), 1, &submitInfo, m_fence->GetHandle()) != VK_SUCCESS)
		{
			assert("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = {m_swapchain->GetHandle()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		VkResult presentResult = vkQueuePresentKHR(m_logicalDevice->GetPresentQueue(), &presentInfo);
		if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
		{
			SC_TRACE("Recreating swapchain");
			m_logicalDevice->DeviceWaitIdle();
			m_swapchain->RecreateSwapchain();
			m_renderPass->DestroyFramebuffers();
			m_renderPass->CreateFramebuffers();

			// Recalculate camera
			UniformBufferData ubo;
			ubo.model = glm::mat4(1.0f);
			ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.projection = glm::perspective(glm::radians(45.0f), m_swapchain->GetSwapchainExtent().width / (float)m_swapchain->GetSwapchainExtent().height, 0.1f, 10.0f);
			ubo.projection[1][1] *= -1;
			m_uniformBuffer->CopyDataToBuffer(&ubo, sizeof(ubo));
		}
		else if (presentResult != VK_SUCCESS)
		{
			SC_ERROR("failed to present swap chain image!");
		}
	}
}
