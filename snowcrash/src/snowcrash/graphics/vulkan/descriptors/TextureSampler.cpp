
#include <snowcrash/graphics/vulkan/descriptors/TextureSampler.hpp>

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {

        TextureSampler::TextureSampler(PhysicalDevice *physicalDevice, LogicalDevice *device)
            : m_device(device)
        {
            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.anisotropyEnable = VK_TRUE;

            // TODO just add this to the struct in physical device and get it there
            VkPhysicalDeviceProperties properties{};
            vkGetPhysicalDeviceProperties(physicalDevice->GetHandle(), &properties);
            samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

            samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = VK_LOD_CLAMP_NONE;

            if (vkCreateSampler(m_device->GetHandle(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
            {
                SC_WARN("failed to create texture sampler!");
            }
        }

        TextureSampler::~TextureSampler()
        {
            vkDestroySampler(m_device->GetHandle(), m_sampler, nullptr);
        }

    } // namespace vulkan

} // namespace SC
