
#pragma once

#include "snowcrash/application/Layer.hpp"
#include "snowcrash/core/Object.hpp"
#include "snowcrash/graphics/Window.hpp"
#include <snowcrash/core/Core.hpp>
#include <snowcrash/core/Context.hpp>

#include <snowcrash/graphics/vulkan/Instance.hpp>
#include <snowcrash/graphics/vulkan/PhysicalDevice.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/Swapchain.hpp>

#include <snowcrash/graphics/vulkan/pipeline/RenderPipeline.hpp>
#include <snowcrash/graphics/vulkan/image/TextureImage2D.hpp>

#include <glm/glm.hpp>

namespace SC
{
	struct UniformBufferData
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};

	class GraphicsLayer : public Layer
	{
		SC_OBJECT(GraphicsLayer, Layer)

	public:
		explicit GraphicsLayer(Context *context, Window *window);
		~GraphicsLayer();

		void Init() override;
		void Update() override;

	private:
		vulkan::Instance *m_instance;
		vulkan::PhysicalDevice *m_physicalDevice;
		vulkan::LogicalDevice *m_logicalDevice;
		vulkan::Swapchain *m_swapchain;
		vulkan::CommandPool *m_commandPool;

		vulkan::RenderPass *m_renderPass;
		vulkan::RenderPipeline *m_pipeline;

		vulkan::VertexDescription *m_vertexDescription;

		vulkan::DescriptorSet *m_descriptorSet;
		vulkan::DescriptorPool *m_descriptorPool;

		vulkan::UniformBuffer *m_uniformBuffer;

		vulkan::TextureSampler *m_textureSampler;
		vulkan::TextureImage2D *m_textureImage;

		ArrayList<vulkan::ShaderModule *> m_shaderModules;

		Window *m_window;
	};

}
