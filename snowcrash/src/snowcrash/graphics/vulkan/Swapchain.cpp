
#include "Swapchain.hpp"

#include <snowcrash/core/Logging.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/PhysicalDevice.hpp>
#include <snowcrash/graphics/vulkan/Instance.hpp>

#include <algorithm>
#include <limits>

namespace SC
{
    namespace vulkan
    {
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const ArrayList<VkSurfaceFormatKHR> &availableFormats)
        {
            // for (const auto &availableFormat : availableFormats)
            for (int i = 0; i < availableFormats.GetIndex(); i++)
            {
                if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
                    availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return availableFormats[i];
                }
            }

            return availableFormats[0];
        }

        VkPresentModeKHR chooseSwapPresentMode(const ArrayList<VkPresentModeKHR> &availablePresentModes)
        {
            // for (const auto &availablePresentMode : availablePresentModes)
            for (int i = 0; i < availablePresentModes.GetIndex(); i++)
            {
                if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    return availablePresentModes[i]; // prefer mailbox (cuz it fast and no tearing)
                }

                if (availablePresentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
                {
                    return availablePresentModes[i];
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window)
        {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return capabilities.currentExtent;
            }
            else
            {
                int width, height;

                glfwGetFramebufferSize(window, &width, &height);

                VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)};

                actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }

        Swapchain::Swapchain(Instance *instance, GLFWwindow *window, PhysicalDevice *physicalDevice, LogicalDevice *device)
            : m_device(device), m_physicalDevice(physicalDevice), m_instance(instance), m_window(window)
        {
            CreateSwapchain(physicalDevice);
            GetSwapchainImages();
            CreateSwapchainImageViews();
        }

        void Swapchain::CreateSwapchain(PhysicalDevice *physicalDevice)
        {
            SC_TRACE("Creating swapchain");

            SwapChainSupportDetails swapChainSupport = physicalDevice->QuerySwapChainSupport();

            VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
            m_presentMode = chooseSwapPresentMode(swapChainSupport.presentModes); // VK_PRESENT_MODE_IMMEDIATE_KHR; //
            VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, m_window);
            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // :3

            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
            {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            /*
                It is also possible that you’ll render images to a separate image first to perform operations like post-processing.
                In that case you may use a value like VK_IMAGE_USAGE_TRANSFER_DST_BIT instead and use a memory operation
                to transfer the rendered image to a swap chain image.
            */

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = m_instance->GetSurface();
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = physicalDevice->FindQueueFamilies();
            uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            if (indices.graphicsFamily != indices.presentFamily)
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0;
                createInfo.pQueueFamilyIndices = nullptr;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = m_presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(m_device->GetHandle(), &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
            {
                SC_ERROR("failed to create swapchain");
            }

            m_swapChainImageFormat = surfaceFormat.format;
            m_swapChainExtent = extent;
        }

        void Swapchain::GetSwapchainImages()
        {
            // Get swapchain images
            // ! memeory leak????
            uint32_t imageCount;
            vkGetSwapchainImagesKHR(m_device->GetHandle(), m_swapchain, &imageCount, nullptr);
            m_swapchainImages.Resize(imageCount);
            m_swapchainImages.SetIndex(imageCount);
            vkGetSwapchainImagesKHR(m_device->GetHandle(), m_swapchain, &imageCount, m_swapchainImages.GetArray());
        }

        void Swapchain::CreateSwapchainImageViews()
        {
            for (int i = 0; i < m_swapchainImages.GetIndex(); i++)
            {
                ImageView *view = new ImageView(m_device, m_swapchainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
                m_swapchainViews.Add(view);
            }
        }

        void Swapchain::RecreateSwapchain()
        {
            for (u32 i = 0; i < m_swapchainViews.GetIndex(); i++)
            {
                delete m_swapchainViews[i];
            }

            m_swapchainViews.SetIndex(0);

            vkDestroySwapchainKHR(m_device->GetHandle(), m_swapchain, nullptr);

            CreateSwapchain(m_physicalDevice);
            GetSwapchainImages();
            CreateSwapchainImageViews();
        }

        Swapchain::~Swapchain()
        {
            vkDestroySwapchainKHR(m_device->GetHandle(), m_swapchain, nullptr);

            for (u32 i = 0; i < m_swapchainViews.GetIndex(); i++)
            {
                delete m_swapchainViews[i];
            }
        }

        u32 Swapchain::AcquireNextImage(Semaphore *semaphore) const
        {
            u32 imageIndex;
            VkResult result = vkAcquireNextImageKHR(m_device->GetHandle(), m_swapchain,
                                                    UINT64_MAX, semaphore->GetHandle(), VK_NULL_HANDLE, &imageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                SC_WARN("Out of date KHR");
                return 0;
            }
            else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            {
                SC_ERROR("failed to acquire swap chain image!");
            }

            return imageIndex;
        }
    } // namespace vulkan
}
