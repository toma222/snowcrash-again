
#include "PhysicalDevice.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        PhysicalDevice::PhysicalDevice(Instance *instance)
            : m_instance(instance)
        {
            SC_TRACE("Creating Physical device");
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(m_instance->GetHandle(), &deviceCount, nullptr);
            if (deviceCount == 0)
                SC_ERROR("There are zero devices that support vulkan on this machine");

            ArrayList<VkPhysicalDevice> devices(deviceCount);
            devices.SetIndex(deviceCount);
            vkEnumeratePhysicalDevices(m_instance->GetHandle(), &deviceCount, devices.GetArray());

            m_physicalDevice = PickDeviceFunctionFirstCompatible(devices);
            m_properties.mssaSamples = GetMaxUsableSampleCount();
        }

        PhysicalDevice::~PhysicalDevice()
        {
        }

        VkPhysicalDevice PhysicalDevice::PickDeviceFunctionFirstCompatible(ArrayList<VkPhysicalDevice> &devices)
        {
            for (int d = 0; d < devices.GetIndex(); d++)
            {
                if (IsDeviceSuitable(devices.Get(d)))
                {
                    m_queueFamilyIndices = FindQueueFamilies(devices.Get(d));
                    return devices.Get(d);
                }
            }

            SC_WARN("function did not find a suitible device.\n we are going to use the first device in the list and roll with it :3");
            m_queueFamilyIndices = FindQueueFamilies(devices[0]);
            return devices[0];
        }

        bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device)
        {
            QueueFamilyIndices indices = FindQueueFamilies(device);

            bool extensionsSupported = CheckDeviceExtensionSupport(device);
            bool swapChainAdequate = false;
            if (extensionsSupported)
            {
                SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
                swapChainAdequate = !swapChainSupport.formats.Empty() && !swapChainSupport.presentModes.Empty();
            }

            VkPhysicalDeviceFeatures supportedFeatures;
            vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

            // VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(device, &m_vulkanDeviceProperties);

            SC_TRACE("Physical Device Option [%s]", m_vulkanDeviceProperties.deviceName);

            return m_queueFamilyIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
        }

        QueueFamilyIndices PhysicalDevice::FindQueueFamilies(VkPhysicalDevice device)
        {
            QueueFamilyIndices indices;
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            ArrayList<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            queueFamilies.SetIndex(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.GetArray());

            int i = 0;
            for (int qf = 0; qf < queueFamilies.GetIndex(); qf++)
            {
                if (queueFamilies[qf].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    indices.graphicsFamily = i;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_instance->GetSurface(), &presentSupport);

                if (presentSupport)
                {
                    indices.presentFamily = i;
                }

                if (indices.isComplete())
                {
                    break;
                }

                i++;
            }

            return indices;
        }

        SwapChainSupportDetails PhysicalDevice::QuerySwapChainSupport(VkPhysicalDevice device)
        {
            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_instance->GetSurface(), &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_instance->GetSurface(), &formatCount, nullptr);
            if (formatCount != 0)
            {
                details.formats.Resize(formatCount);
                details.formats.SetIndex(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_instance->GetSurface(),
                                                     &formatCount, details.formats.GetArray());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_instance->GetSurface(), &presentModeCount, nullptr);
            if (presentModeCount != 0)
            {
                details.presentModes.Resize(presentModeCount);
                details.presentModes.SetIndex(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_instance->GetSurface(),
                                                          &presentModeCount, details.presentModes.GetArray());
            }

            return details;
        }

        bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
        {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            ArrayList<VkExtensionProperties> availableExtensions(extensionCount);
            availableExtensions.SetIndex(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.GetArray());

            ArrayList<const char *> requiredExtensions = PhysicalDevice::GetRequiredExtensions();
            int numberOfMatchedExtensions = 0;

            for (int e = 0; e < availableExtensions.GetIndex(); e++)
            {
                for (int r = 0; r < requiredExtensions.GetIndex(); r++)
                {
                    if (strcmp(availableExtensions.Get(e).extensionName, requiredExtensions.Get(r)) == 0)
                    {
                        SC_TRACE("Found device extension [%s]", availableExtensions.Get(e).extensionName);
                        numberOfMatchedExtensions++;
                    }
                }
            }

            return (numberOfMatchedExtensions == requiredExtensions.GetIndex());
        }

        VkSampleCountFlagBits PhysicalDevice::GetMaxUsableSampleCount()
        {
            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProperties);

            VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

            if (counts & VK_SAMPLE_COUNT_64_BIT)
            {
                return VK_SAMPLE_COUNT_64_BIT;
            }
            if (counts & VK_SAMPLE_COUNT_32_BIT)
            {
                return VK_SAMPLE_COUNT_32_BIT;
            }
            if (counts & VK_SAMPLE_COUNT_16_BIT)
            {
                return VK_SAMPLE_COUNT_16_BIT;
            }
            if (counts & VK_SAMPLE_COUNT_8_BIT)
            {
                return VK_SAMPLE_COUNT_8_BIT;
            }
            if (counts & VK_SAMPLE_COUNT_4_BIT)
            {
                return VK_SAMPLE_COUNT_4_BIT;
            }
            if (counts & VK_SAMPLE_COUNT_2_BIT)
            {
                return VK_SAMPLE_COUNT_2_BIT;
            }

            SC_WARN("Max usable sample count is 1; :|");
            return VK_SAMPLE_COUNT_1_BIT;
        }

        const ArrayList<const char *> PhysicalDevice::GetRequiredExtensions()
        {
            ArrayList<const char *> requiredExtensions;
            requiredExtensions.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            return requiredExtensions;
        }
    } // namespace vulkan
}
