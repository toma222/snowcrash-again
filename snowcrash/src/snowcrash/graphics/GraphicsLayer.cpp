
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

#include <imgui/imgui.h>

#include <snowcrash/debug/Debug.hpp>

namespace SC
{

	GraphicsLayer::GraphicsLayer(Context *context, Window *window)
		: Layer(context), m_window(window) {}

	GraphicsLayer::~GraphicsLayer()
	{
		SC_TRACE("Clean graphics layer");

		m_logicalDevice->DeviceWaitIdle();

		CleanSubrenders();

		delete m_fence;
		delete m_imageAvailableSemaphore;
		delete m_renderFinishedSemaphore;

		delete m_descriptorPool;
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

		ArrayList<DescriptorPool::DescriptorBinding> poolBindings;
		poolBindings.Add(DescriptorPool::DescriptorBinding{Descriptor::DescriptorType::DescriptorType_Uniform, 10});
		poolBindings.Add(DescriptorPool::DescriptorBinding{Descriptor::DescriptorType::DescriptorType_TextureSampler, 10});
		m_descriptorPool = new vulkan::DescriptorPool(m_logicalDevice, poolBindings);

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
		Timer frameTimer("frameTimer", 0, false);

		// Profit
		Timer getNextImageTimer("next image", 0, false);
		m_fence->WaitForFence();
		uint32_t imageIndex = m_swapchain->AcquireNextImage(m_imageAvailableSemaphore);
		m_fence->ResetFence();
		getNextImageTimer.EndTimer();

		Timer fillCommandBufferTimer("fill command buffer", 0, false);
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

		m_renderPass->BeginRenderPass(imageIndex, m_swapchain->GetSwapchainExtent(), buffer);

		for (int i = m_subrenders.GetIndex() - 1; i >= 0; i--)
		{
			m_subrenders[i]->Render(buffer);
		}

		m_renderPass->EndRenderPass(buffer);

		if (vkEndCommandBuffer(buffer) != VK_SUCCESS)
		{
			assert("failed to record command buffer!");
		}

		fillCommandBufferTimer.EndTimer();

		// STOP DRAWING STUFF

		Timer presentTimer("present timer", 0, false);
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

			for (int i = 0; i < m_subrenders.GetIndex(); i++)
				m_subrenders[i]->RecreateSwapchain(m_swapchain->GetSwapchainExtent());
		}
		else if (presentResult != VK_SUCCESS)
		{
			SC_ERROR("failed to present swap chain image!");
		}

		presentTimer.EndTimer();
		frameTimer.EndTimer();

		m_prevFrameDebugTimer.totalFrameTime = frameTimer.GetMicroseconds() / 1000;
		m_prevFrameDebugTimer.getSwapchainImage = getNextImageTimer.GetMicroseconds() / 1000;
		m_prevFrameDebugTimer.presentImage = presentTimer.GetMicroseconds() / 1000;
		m_prevFrameDebugTimer.fillCommandBuffer = fillCommandBufferTimer.GetMicroseconds() / 1000;
	}

	void GraphicsLayer::DrawGuiFrame()
	{
		ImGui::SeparatorText("Debug info");
		ImGui::Text("Device name: %s", m_physicalDevice->GetVulkanDeviceProperties().deviceName);
		ImGui::Text("MSSA samples: %u", (u32)(m_physicalDevice->GetDeviceProperties().mssaSamples));

		ImGui::Separator();
		VkExtent2D se = m_swapchain->GetSwapchainExtent();
		ImGui::Text("Swapchain extent: [%u, %u]", se.width, se.height);
		ImGui::Text("Swapchain images: %u", m_swapchain->GetSwapchainImageViews().GetIndex());

		switch (m_swapchain->GetPresentMode())
		{
		case VK_PRESENT_MODE_FIFO_KHR:
			ImGui::Text("Swapchain present mode: FIFO");
			break;
		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			ImGui::Text("Swapchain present mode: Relaxed FIFO");
			break;
		case VK_PRESENT_MODE_MAILBOX_KHR:
			ImGui::Text("Swapchain present mode: Mailbox");
			break;
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			ImGui::Text("Swapchain present mode: Immediate");
			break;
		default:
			break;
		}

		ImGui::Separator();
		ImGui::Text("total frame time: %.2f (ms)", m_prevFrameDebugTimer.totalFrameTime);
		ImGui::Text("get next image: %.2f (ms)", m_prevFrameDebugTimer.getSwapchainImage);
		ImGui::Text("fill command buffer: %.2f (ms)", m_prevFrameDebugTimer.fillCommandBuffer);
		ImGui::Text("draw and present image: %.2f (ms)", m_prevFrameDebugTimer.presentImage);
	}
}
