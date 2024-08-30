
#include "Instance.hpp"

#include <snowcrash/core/Logging.hpp>

#include <assert.h>

namespace SC
{
    namespace vulkan
    {
        ArrayList<const char *> Instance::GetValidationLayers()
        {
            static ArrayList<const char *> validationLayers;
            validationLayers.Add("VK_LAYER_KHRONOS_validation");
            return validationLayers;
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData)
        {
            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
                SC_WARN("vulkan debug callback [%s]", pCallbackData->pMessage);
            if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
                SC_ERROR("vulkan debug callback [%s]", pCallbackData->pMessage);

            return VK_FALSE;
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
        {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

        void Instance::CreateDebugUtils()
        {
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            populateDebugMessengerCreateInfo(debugCreateInfo);

            auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr)
            {
                func(m_instance, &debugCreateInfo, nullptr, &m_debugMessenger);
            }
            else
            {
                SC_ERROR("CreateDebugUtilsMessengerEXT failed");
            }
        }

        bool CheckValidationLayerSupport()
        {
            static ArrayList<const char *> validationLayers = Instance::GetValidationLayers();

            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            ArrayList<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.GetArray());

            for (int vl = 0; vl < validationLayers.GetIndex(); vl++)
            {
                bool layerFound = false;

                for (int layer = 0; layer < layerCount; layer++)
                {
                    if (strcmp(validationLayers.Get(vl), availableLayers.Get(layer).layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound)
                    return false;
            }

            return true;
        }

        ArrayList<const char *> GetRequiredExtensions()
        {
            uint32_t glfwExtensionCount = 0;
            const char **glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            ArrayList<const char *> extensions;
            for (int i = 0; i < glfwExtensionCount; i++)
            {
                extensions.Add(glfwExtensions[i]);
                SC_TRACE("Adding glfw extension [%s]", glfwExtensions[i]);
            }

            if (SC_ENABLE_VALIDATION)
            {
                extensions.Add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        Instance::Instance(GLFWwindow *window)
        {
            SC_TRACE("Creating vk instance");
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            ArrayList<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.GetArray());
            SC_TRACE("Checking validation layers");
            if (SC_ENABLE_VALIDATION && !CheckValidationLayerSupport())
            {
                assert("Vulkan validation layers requested, but not available");
            }

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Vulkan Engine";
            appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
            appInfo.pEngineName = "Snowcrash";
            appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            ArrayList<const char *> reqExtensions = GetRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(reqExtensions.GetIndex());
            createInfo.ppEnabledExtensionNames = reqExtensions.GetArray();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            if (SC_ENABLE_VALIDATION)
            {
                SC_TRACE("Creating validation layers");
                const ArrayList<const char *> validationLayers = GetValidationLayers();
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.GetIndex());
                createInfo.ppEnabledLayerNames = validationLayers.GetArray();

                populateDebugMessengerCreateInfo(debugCreateInfo);

                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
            }
            else
            {
                createInfo.enabledLayerCount = 0;
                createInfo.pNext = nullptr;
            }

            VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
            if (result != VK_SUCCESS)
            {
                assert("vkCreateInstance did not return success");
            }

            CreateDebugUtils();

            if (glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface) != VK_SUCCESS)
            {
                SC_ERROR("failed to create window surface");
            }
        }

        Instance::~Instance()
        {
            vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
            vkDestroyInstance(m_instance, nullptr);
        }

        void Instance::DestroyDebugUtils() const
        {
            if (SC_ENABLE_VALIDATION)
            {
                auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
                if (func != nullptr)
                    func(m_instance, m_debugMessenger, nullptr);
            }
        }
    } // namespace vulkan

} // namespace SC
