
#pragma once

#include <snowcrash/core/Core.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <snowcrash/graphics/vulkan/descriptors/UniformBuffer.hpp>
#include <snowcrash/graphics/vulkan/image/Image.hpp>
#include <snowcrash/graphics/vulkan/image/ImageView.hpp>
#include <snowcrash/graphics/vulkan/descriptors/TextureSampler.hpp>

namespace SC
{
    namespace vulkan
    {
        class Descriptor
        {
        public:
            enum DescriptorType
            {
                DescriptorType_Uniform,
                DescriptorType_TextureSampler
            };

        public:
            Descriptor() = default;
            virtual ~Descriptor() = default;

            virtual DescriptorType GetType() = 0;
            virtual VkDescriptorSetLayoutBinding GetDescriptorBinding(int binding) = 0;
            virtual VkWriteDescriptorSet GetWriteSet(VkDescriptorSet set, int binding) = 0;
        };

        // TODO we do want to add support for the stage flag paramater
        class DescriptorUniform : public Descriptor
        {
        public:
            DescriptorUniform(UniformBuffer *buffer)
            {
                m_bufferInfo.buffer = buffer->GetBuffer()->Get();
                m_bufferInfo.offset = 0;
                m_bufferInfo.range = buffer->GetSize();
            }

            Descriptor::DescriptorType GetType() { return Descriptor::DescriptorType_Uniform; }

            VkDescriptorSetLayoutBinding GetDescriptorBinding(int binding)
            {
                VkDescriptorSetLayoutBinding uboLayoutBinding{};
                uboLayoutBinding.binding = binding;
                uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                uboLayoutBinding.descriptorCount = 1;
                uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
                uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
                return uboLayoutBinding;
            }

            VkWriteDescriptorSet GetWriteSet(VkDescriptorSet set, int binding)
            {
                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = set;
                write.dstBinding = binding;
                write.dstArrayElement = 0;
                write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                write.descriptorCount = 1;
                write.pBufferInfo = &m_bufferInfo; // ! this might cause an issue

                return write;
            }

            VkDescriptorBufferInfo m_bufferInfo;
        };

        class DescriptorTextureSampler : public Descriptor
        {
            DescriptorTextureSampler(TextureSampler *ts, ImageView *view, int count = 1)
                : m_textureSampler(ts), m_imageView(view)
            {
                // VkDescriptorImageInfo imageInfo{};
                m_info = new VkDescriptorImageInfo();
                m_info->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_info->imageView = m_imageView->GetHandle();
                m_info->sampler = m_textureSampler->GetHandle();
            }

            ~DescriptorTextureSampler() override
            {
                delete m_info;
            }

            DescriptorType GetType() override { return Descriptor::DescriptorType_TextureSampler; }

            VkDescriptorSetLayoutBinding GetDescriptorBinding(int binding) override
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding{};
                samplerLayoutBinding.binding = binding;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                return samplerLayoutBinding;
            }

            VkWriteDescriptorSet GetWriteSet(VkDescriptorSet set, int binding) override
            {

                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = set;
                write.dstBinding = binding;
                write.dstArrayElement = 0;
                write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                write.descriptorCount = 1;
                write.pImageInfo = m_info;

                return write;
            }

        public:
            TextureSampler *m_textureSampler;
            ImageView *m_imageView;
            VkDescriptorImageInfo *m_info;
        };

        constexpr VkDescriptorType GetVkDescriptorType(Descriptor::DescriptorType type)
        {
            switch (type)
            {
            case Descriptor::DescriptorType_Uniform:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

            case Descriptor::DescriptorType_TextureSampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

            default:
                break;
            }

            return VkDescriptorType(0);
        }
    } // namespace vulkan

} // namespace SC
