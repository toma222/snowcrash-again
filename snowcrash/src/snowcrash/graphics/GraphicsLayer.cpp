
#include "GraphicsLayer.hpp"
#include "snowcrash/application/Layer.hpp"

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
		m_instance = new vulkan::Instance(m_window->GetHandle());
		m_physicalDevice = new vulkan::PhysicalDevice(m_instance);
		m_logicalDevice = new vulkan::LogicalDevice(m_physicalDevice);
		m_swapchain = new vulkan::Swapchain(m_instance, m_window->GetHandle(), m_physicalDevice, m_logicalDevice);
	}

	void GraphicsLayer::Update()
	{
	}

}
