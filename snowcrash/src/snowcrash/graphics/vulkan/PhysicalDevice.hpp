
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/graphics/vulkan/Instance.hpp>
#include <snowcrash/types/ArrayList.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>

namespace SC
{
    namespace vulkan
    {
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily{std::nullopt};
            std::optional<uint32_t> presentFamily{std::nullopt};

            bool isComplete()
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            ArrayList<VkSurfaceFormatKHR> formats;
            ArrayList<VkPresentModeKHR> presentModes;
        };

        struct PhysicalDeviceProperties
        {
            VkSampleCountFlagBits mssaSamples;
            uint32_t apiVersion;
            uint32_t driverVersion;
            char *deviceName;
        };

        class PhysicalDevice
        {
        public:
            PhysicalDevice(Instance *instance);
            ~PhysicalDevice();

            const VkPhysicalDevice GetHandle() const { return m_physicalDevice; }

            static const ArrayList<const char *> GetRequiredExtensions();

            QueueFamilyIndices GetQueueFamilyIndices() { return m_queueFamilyIndices; }
            const PhysicalDeviceProperties GetDeviceProperties() const { return m_properties; }

        public:
            SwapChainSupportDetails QuerySwapChainSupport() { return QuerySwapChainSupport(this->m_physicalDevice); }
            QueueFamilyIndices FindQueueFamilies() { return FindQueueFamilies(this->m_physicalDevice); }

        private:
            VkPhysicalDevice PickDeviceFunctionFirstCompatible(ArrayList<VkPhysicalDevice> &devices);
            bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
            SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
            bool IsDeviceSuitable(VkPhysicalDevice device);
            VkSampleCountFlagBits GetMaxUsableSampleCount();

        private:
            Instance *m_instance;
            VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
            QueueFamilyIndices m_queueFamilyIndices;
            PhysicalDeviceProperties m_properties;
        };
    } // namespace vulkan
}