
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

namespace SC
{

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

		Window *m_window;
	};

}
