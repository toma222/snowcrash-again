
#include "ShaderModule.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        ShaderModule::ShaderModule(LogicalDevice *device, ShaderType type, char *code, size_t size)
            : m_device(device), m_type(type)
        {
            VkShaderModuleCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = size;
            createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

            if (vkCreateShaderModule(m_device->GetHandle(), &createInfo, nullptr, &m_module) != VK_SUCCESS)
            {
                SC_WARN("failed to create shader module");
            }
        }

        ShaderModule::~ShaderModule()
        {
            vkDestroyShaderModule(m_device->GetHandle(), m_module, nullptr);
        }

    } // namespace vulkan

} // namespace SC
