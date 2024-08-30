
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/types/ArrayList.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/image/ImageView.hpp>

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
            Instance *m_instance;
            GLFWwindow *m_window;

            VkFormat m_swapChainImageFormat{};
            VkExtent2D m_swapChainExtent{};
        };
    } // namespace vulkan
}
