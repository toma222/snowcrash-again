
#include "GraphicsLayer.hpp"
#include "snowcrash/application/Layer.hpp"

#include <snowcrash/resource/ResourceRawFile.hpp>
#include <snowcrash/resource/ResourceImage.hpp>

#include <snowcrash/graphics/vulkan/descriptors/Descriptor.hpp>

namespace SC
{

	GraphicsLayer::GraphicsLayer(Context *context, Window *window)
		: Layer(context), m_window(window)
	{
	}

	GraphicsLayer::~GraphicsLayer()
	{
		SC_TRACE("Clean graphics layer");
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

		m_renderPass = new vulkan::RenderPass(m_swapchain, m_logicalDevice, m_physicalDevice);

		const auto vertexShader = GetResource<ResourceRawFile>("frag.spv");
		const auto fragmentShader = GetResource<ResourceRawFile>("vert.spv");

		m_shaderModules.Add(new ShaderModule(m_logicalDevice, ShaderModule::ShaderType_Vertex, vertexShader->GetBytes(), vertexShader->GetSize()));
		m_shaderModules.Add(new ShaderModule(m_logicalDevice, ShaderModule::ShaderType_Fragment, fragmentShader->GetBytes(), fragmentShader->GetSize()));

		ArrayList<VkPushConstantRange> pushConstants;
		VkPushConstantRange pushConstant;
		pushConstant.size = sizeof(float) * 4 * 4;
		pushConstant.offset = 0;
		pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstants.Add(pushConstant);

		m_vertexDescription = new vulkan::VertexDescription();
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec2);
		m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);

		ArrayList<DescriptorPool::DescriptorBinding> poolBindings;
		poolBindings.Add(DescriptorPool::DescriptorBinding{Descriptor::DescriptorType::DescriptorType_Uniform, 1});
		poolBindings.Add(DescriptorPool::DescriptorBinding{Descriptor::DescriptorType::DescriptorType_TextureSampler, 1});
		m_descriptorPool = new vulkan::DescriptorPool(m_logicalDevice, poolBindings);

		m_uniformBuffer = new UniformBuffer(m_physicalDevice, m_logicalDevice, m_commandPool, sizeof(UniformBufferData));

		const ResourceImage *image = GetResource<ResourceImage>("checker-map_tho.png");
		m_textureImage = new TextureImage2D(m_physicalDevice, m_logicalDevice, m_commandPool, image->GetImage(),
											TextureData{
												image->GetWidth(),
												image->GetHeight(),
											});
		m_textureSampler = new TextureSampler(m_physicalDevice, m_logicalDevice);

		ArrayList<Descriptor *> m_descriptors;
		m_descriptors.Add(new DescriptorUniform(m_uniformBuffer));
		m_descriptors.Add(new DescriptorTextureSampler(m_textureSampler, m_textureImage->GetImageView()));
		DescriptorLayout *layout = new DescriptorLayout(m_logicalDevice, m_descriptors);
		m_descriptorSet = new DescriptorSet(m_physicalDevice, m_logicalDevice, m_descriptorPool, layout, 1);
		delete layout;

		m_pipeline = new vulkan::RenderPipeline(m_swapchain, m_physicalDevice, m_logicalDevice,
												vulkan::RenderPipeline::RenderPipelineDef{vulkan::RenderPipeline::Mode_Polygon,
																						  vulkan::RenderPipeline::Depth_ReadWrite,
																						  m_renderPass,
																						  m_shaderModules,
																						  pushConstants,
																						  m_vertexDescription,
																						  m_descriptorSet});
	}

	void GraphicsLayer::Update()
	{
	}
}
