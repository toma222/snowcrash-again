
#pragma once

#include <snowcrash/core/Core.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

namespace SC
{
    namespace vulkan
    {
        class ShaderModule
        {
        public:
            enum ShaderType
            {
                ShaderType_Vertex,
                ShaderType_Fragment
            };

            ShaderModule(LogicalDevice *device, ShaderType type, char *code, size_t size);
            ~ShaderModule();

            VkShaderModule GetHandle() { return m_module; }
            const ShaderType GetShaderType() { return m_type; }

        private:
            VkShaderModule m_module;
            LogicalDevice *m_device;
            const ShaderType m_type;
        };
    } // namespace vulkan
} // namespace SC
