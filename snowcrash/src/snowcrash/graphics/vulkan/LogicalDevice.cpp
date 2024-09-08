
#include "LogicalDevice.hpp"

#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/graphics/vulkan/Instance.hpp>
#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        LogicalDevice::LogicalDevice(PhysicalDevice *physicalDevice)
        {
            SC_TRACE("Creating Logical device");

            // we only want one queue with graphics capabilities
            ArrayList<VkDeviceQueueCreateInfo> queueCreateInfos;

            ArrayList<uint32_t> uniqueQueueFamilies;
            uniqueQueueFamilies.Add(physicalDevice->GetQueueFamilyIndices().graphicsFamily.value());
            uniqueQueueFamilies.Add(physicalDevice->GetQueueFamilyIndices().presentFamily.value());

            m_graphicsFamily = physicalDevice->GetQueueFamilyIndices().graphicsFamily.value();

            float queuePriority = 1.0f;
            for (int i = 0; i < uniqueQueueFamilies.GetIndex(); i++)
            {
                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = uniqueQueueFamilies[i];
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;
                queueCreateInfos.Add(queueCreateInfo);
            }

            VkPhysicalDeviceFeatures deviceFeatures{};
            deviceFeatures.samplerAnisotropy = VK_TRUE;
            deviceFeatures.sampleRateShading = VK_TRUE;
            deviceFeatures.fillModeNonSolid = VK_TRUE;
            deviceFeatures.wideLines = VK_TRUE;

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.GetIndex());
            createInfo.pQueueCreateInfos = queueCreateInfos.GetArray();
            createInfo.queueCreateInfoCount = 1;
            createInfo.pEnabledFeatures = &deviceFeatures;

            // you set this even though you dont have to for backwards compatability

            const ArrayList<const char *> deviceExtensions = PhysicalDevice::GetRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.GetIndex());
            createInfo.ppEnabledExtensionNames = deviceExtensions.GetArray();

            if (SC_ENABLE_VALIDATION)
            {
                ArrayList<const char *> validationLayers;
                Instance::GetValidationLayers(validationLayers);
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.GetIndex());
                createInfo.ppEnabledLayerNames = validationLayers.GetArray();
            }
            else
            {
                createInfo.enabledLayerCount = 0;
            }

            if (vkCreateDevice(physicalDevice->GetHandle(), &createInfo, nullptr, &m_device) != VK_SUCCESS)
            {
                SC_ERROR("failed to create logical device");
            }

            // Get the queues
            vkGetDeviceQueue(m_device, physicalDevice->GetQueueFamilyIndices().graphicsFamily.value(), 0, &m_graphicsQueue);
            vkGetDeviceQueue(m_device, physicalDevice->GetQueueFamilyIndices().presentFamily.value(), 0, &m_presentQueue);
        }

        void LogicalDevice::DeviceWaitIdle() const
        {
            vkDeviceWaitIdle(m_device);
        }

        LogicalDevice::~LogicalDevice()
        {
            vkDestroyDevice(m_device, nullptr);
        }
    } // namespace vulkan
}
