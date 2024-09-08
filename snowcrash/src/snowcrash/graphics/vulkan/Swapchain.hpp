
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/types/ArrayList.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/image/ImageView.hpp>
#include <snowcrash/graphics/vulkan/sync/Semaphore.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class Swapchain
        {
        public:
            Swapchain(Instance *instance, GLFWwindow *window, PhysicalDevice *physicalDevice, LogicalDevice *device);
            ~Swapchain();

            VkFormat GetSwapchainFormat() const { return m_swapChainImageFormat; }
            VkExtent2D GetSwapchainExtent() const { return m_swapChainExtent; }
            ArrayList<ImageView *> &GetSwapchainImageViews() { return m_swapchainViews; }
            VkPresentModeKHR GetPresentMode() const { return m_presentMode; }

            VkSwapchainKHR GetHandle() const { return m_swapchain; }

            u32 AcquireNextImage(Semaphore *semaphore) const;

            // does not call vkDeviceIdle
            void RecreateSwapchain();

        private:
            void CreateSwapchain(PhysicalDevice *physicalDevice);
            void GetSwapchainImages();
            void CreateSwapchainImageViews();

        private:
            VkSwapchainKHR m_swapchain;
            ArrayList<VkImage> m_swapchainImages;
            ArrayList<ImageView *> m_swapchainViews;

            LogicalDevice *m_device;
            PhysicalDevice *m_physicalDevice;
            Instance *m_instance;
            GLFWwindow *m_window;

            VkFormat m_swapChainImageFormat{};
            VkExtent2D m_swapChainExtent{};
            VkPresentModeKHR m_presentMode;
        };
    } // namespace vulkan
}
