
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/ArrayList.hpp>

#define SC_ENABLE_VALIDATION true

namespace SC
{
    namespace vulkan
    {

        class Instance
        {
        public:
            Instance(GLFWwindow *window);
            ~Instance();

            VkInstance GetHandle() const { return m_instance; }
            VkSurfaceKHR GetSurface() const { return m_surface; }

            void DestroyDebugUtils() const;

            static ArrayList<const char *> GetValidationLayers();

        private:
            void CreateDebugUtils();

        private:
            VkInstance m_instance;
            VkSurfaceKHR m_surface;
            VkDebugUtilsMessengerEXT m_debugMessenger;
        };
    }
}
