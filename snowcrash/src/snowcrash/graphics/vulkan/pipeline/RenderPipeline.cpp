
#include "RenderPipeline.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        VertexDescription::VertexDescription() = default;
        VertexDescription::~VertexDescription() = default;

        void VertexDescription::Add(Type t)
        {
            VkVertexInputAttributeDescription description;

            switch (t)
            {
            case VertexDescription::Type_vec2:
                description.binding = 0;
                description.format = VK_FORMAT_R32G32_SFLOAT;
                description.location = count;
                description.offset = size;
                size += (sizeof(float) * 2);
                break;

            case VertexDescription::Type_vec3:
                description.binding = 0;
                description.format = VK_FORMAT_R32G32B32_SFLOAT;
                description.location = count;
                description.offset = size;
                size += (sizeof(float) * 2);
                break;
            }

            m_descriptions.Add(description);

            count++;
        }

        RenderPipeline::RenderPipeline(Swapchain *swapchain, PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice, const RenderPipelineDef &def)
            : m_device(logicalDevice)
        {
            CreateShaderStages(def.shaderModules);

            VkPipelineDynamicStateCreateInfo dynamicState = CreateDynamicState();

            auto bindingDescription = def.vertexDescription->GetBinding();
            auto attributeDescriptions = def.vertexDescription->GetAttributes();

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(def.vertexDescription->GetAttributesCount());
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)swapchain->GetSwapchainExtent().width;
            viewport.height = (float)swapchain->GetSwapchainExtent().height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swapchain->GetSwapchainExtent();

            VkPipelineViewportStateCreateInfo viewportState{};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.pViewports = &viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            rasterizer.depthBiasEnable = VK_FALSE;
            rasterizer.depthBiasConstantFactor = 0.0f; // Optional
            rasterizer.depthBiasClamp = 0.0f;          // Optional
            rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

            VkPipelineMultisampleStateCreateInfo multisampling{};
            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = physicalDevice->GetDeviceProperties().mssaSamples;
            multisampling.minSampleShading = 1.0f;          // Optional
            multisampling.pSampleMask = nullptr;            // Optional
            multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
            multisampling.alphaToOneEnable = VK_FALSE;      // Optional
            multisampling.sampleShadingEnable = VK_TRUE;    // enable sample shading in the pipeline
            multisampling.minSampleShading = .2f;           // min fraction for sample shading; closer to one is smoother

            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachment;
            colorBlending.blendConstants[0] = 0.0f; // Optional
            colorBlending.blendConstants[1] = 0.0f; // Optional
            colorBlending.blendConstants[2] = 0.0f; // Optional
            colorBlending.blendConstants[3] = 0.0f; // Optional

            VkPipelineDepthStencilStateCreateInfo depthStencil{};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = VK_TRUE;
            depthStencil.depthWriteEnable = VK_TRUE;
            depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencil.depthBoundsTestEnable = VK_FALSE;
            depthStencil.minDepthBounds = 0.0f; // Optional
            depthStencil.maxDepthBounds = 1.0f; // Optional
            depthStencil.stencilTestEnable = VK_FALSE;
            depthStencil.front = {}; // Optional
            depthStencil.back = {};  // Optional

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = m_shaderStages.GetIndex();
            pipelineInfo.pStages = m_shaderStages.GetArray();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisampling;
            pipelineInfo.pDepthStencilState = &depthStencil;
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = &dynamicState;
            pipelineInfo.layout = m_layout;
            pipelineInfo.renderPass = def.renderPass->GetHandle();
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
            pipelineInfo.basePipelineIndex = -1;              // Optional

            if (vkCreateGraphicsPipelines(m_device->GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
            {
                SC_WARN("failed to create graphics pipeline!");
            }
        }

        void RenderPipeline::CreatePipelineLayout(const RenderPipelineDef &def)
        {
            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            auto &layouts = def.descriptorSet->GetLayouts();

            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = layouts.GetIndex(); // Optional
            pipelineLayoutInfo.pSetLayouts = layouts.GetArray();    // Optional
            pipelineLayoutInfo.pushConstantRangeCount = def.pushConstants.GetIndex();
            pipelineLayoutInfo.pPushConstantRanges = def.pushConstants.GetArray();

            if (vkCreatePipelineLayout(m_device->GetHandle(), &pipelineLayoutInfo, nullptr, &m_layout) != VK_SUCCESS)
            {
                SC_WARN("failed to create pipeline layout!");
            }
        }

        void RenderPipeline::CreateShaderStages(ArrayList<ShaderModule *> shaderModules)
        {
            for (int i = 0; i < shaderModules.GetIndex(); i++)
            {
                VkPipelineShaderStageCreateInfo shaderStageInfo{};
                shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                switch (shaderModules[i]->GetShaderType())
                {
                case ShaderModule::ShaderType_Fragment:
                    shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                    break;

                case ShaderModule::ShaderType_Vertex:
                    shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                    break;
                }
                shaderStageInfo.module = shaderModules[i]->GetHandle();
                shaderStageInfo.pName = "main";
                m_shaderStages.Add(shaderStageInfo);
            }
        }

        VkPipelineDynamicStateCreateInfo RenderPipeline::CreateDynamicState()
        {
            ArrayList<VkDynamicState> dynamicStates;
            dynamicStates.Add(VK_DYNAMIC_STATE_VIEWPORT);
            dynamicStates.Add(VK_DYNAMIC_STATE_SCISSOR);

            VkPipelineDynamicStateCreateInfo dynamicState{};
            dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.GetIndex());
            dynamicState.pDynamicStates = dynamicStates.GetArray();

            return dynamicState;
        }

        RenderPipeline::~RenderPipeline()
        {
            vkDestroyPipeline(m_device->GetHandle(), m_pipeline, nullptr);
        }
    } // namespace vulkan

} // namespace SC
