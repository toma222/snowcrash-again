
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
#include <snowcrash/graphics/vulkan/sync/Fence.hpp>
#include <snowcrash/graphics/vulkan/sync/Semaphore.hpp>
#include <snowcrash/graphics/vulkan/buffer/IndexBuffer.hpp>
#include <snowcrash/graphics/vulkan/buffer/VertexBuffer.hpp>

#include <glm/glm.hpp>

namespace SC
{
	class ImGuiSubrender;
	class Subrender;
	// This is for debug info
	struct DebugTimers
	{
		float totalFrameTime;
		float presentImage;
		float getSwapchainImage;
		float fillCommandBuffer;
	};

	// TODO add a mode to remove the timers
	class GraphicsLayer : public Layer
	{
		SC_OBJECT(GraphicsLayer, Layer)

	public:
		explicit GraphicsLayer(Context *context, Window *window);
		~GraphicsLayer();

		void Init() override;
		void Update() override;

		void DrawGuiFrame() override;

		// TODO make this ish const
		vulkan::RenderPass *GetRenderPass() const { return m_renderPass; };

		template <class S, typename... Args>
		S *AddSubrender(Args &&...args)
		{
			S *s = new S(this, std::forward<Args>(args)...);
			m_subrenders.Add(dynamic_cast<Subrender *>(s));
			return s;
		}

		void InitSubrenders();
		void CleanSubrenders();

	public:
		vulkan::Instance *GetInstance() { return m_instance; }
		vulkan::PhysicalDevice *GetPhysicalDevice() { return m_physicalDevice; }
		vulkan::LogicalDevice *GetLogicalDevice() { return m_logicalDevice; }
		vulkan::Swapchain *GetSwapchain() { return m_swapchain; }
		vulkan::CommandPool *GetCommandPool() { return m_commandPool; }
		vulkan::DescriptorPool *GetDescriptorPool() { return m_descriptorPool; }

	private:
		vulkan::Instance *m_instance;
		vulkan::PhysicalDevice *m_physicalDevice;
		vulkan::LogicalDevice *m_logicalDevice;
		vulkan::Swapchain *m_swapchain;
		vulkan::CommandPool *m_commandPool;

		vulkan::RenderPass *m_renderPass;
		// vulkan::RenderPipeline *m_pipeline;

		// vulkan::VertexDescription *m_vertexDescription;

		// vulkan::DescriptorSet *m_descriptorSet;
		vulkan::DescriptorPool *m_descriptorPool;

		// vulkan::UniformBuffer *m_uniformBuffer;

		// vulkan::TextureSampler *m_textureSampler;
		// vulkan::TextureImage2D *m_textureImage;

		// vulkan::DescriptorLayout *m_layout;

		// vulkan::IndexBuffer *m_indexBuffer;
		// vulkan::VertexBuffer *m_vertexBuffer;

		// ArrayList<vulkan::ShaderModule *> m_shaderModules;

		vulkan::Fence *m_fence;
		vulkan::Semaphore *m_imageAvailableSemaphore;
		vulkan::Semaphore *m_renderFinishedSemaphore;

		// vulkan::Framebuffer *m_renderTarget;

		Window *m_window;

		ArrayList<Subrender *> m_subrenders;

		friend ImGuiSubrender;

	private:
		DebugTimers m_prevFrameDebugTimer;
	};

}
