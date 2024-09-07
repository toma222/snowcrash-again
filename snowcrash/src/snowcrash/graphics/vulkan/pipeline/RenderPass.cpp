
#include "RenderPass.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {

        RenderPass::RenderPass(Swapchain *swapchain, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, CommandPool *pool)
            : m_device(logicalDevice), m_physicalDevice(physicalDevice), m_swapchain(swapchain), m_commandPool(pool)
        {
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = swapchain->GetSwapchainFormat();
            colorAttachment.samples = physicalDevice->GetDeviceProperties().mssaSamples;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            VkAttachmentReference colorRef;
            colorRef.attachment = 0;
            colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription depthAttachment{};
            VkFormat depthFormat = Image::FindSupportedFormat(physicalDevice,
                                                              {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                                              VK_IMAGE_TILING_OPTIMAL,
                                                              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
            depthAttachment.format = depthFormat;
            depthAttachment.samples = physicalDevice->GetDeviceProperties().mssaSamples;
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            VkAttachmentReference depthAttachmentRef{};
            depthAttachmentRef.attachment = 1;
            depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription colorAttachmentResolve{};
            colorAttachmentResolve.format = swapchain->GetSwapchainFormat();
            colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            VkAttachmentReference colorResolveRef;
            colorResolveRef.attachment = 2;
            colorResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDependency dependency{};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            // dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorRef;
            subpass.pResolveAttachments = &colorResolveRef;
            subpass.pDepthStencilAttachment = &depthAttachmentRef;

            ArrayList<VkAttachmentDescription> descriptionArray;
            descriptionArray.Add(colorAttachment);
            descriptionArray.Add(depthAttachment);
            descriptionArray.Add(colorAttachmentResolve);

            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 3;
            renderPassInfo.pAttachments = descriptionArray.GetArray();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;

            renderPassInfo.dependencyCount = 1;
            renderPassInfo.pDependencies = &dependency;

            if (vkCreateRenderPass(m_device->GetHandle(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
            {
                SC_WARN("failed to create render pass!");
            }

            CreateFramebuffers();
        }

        void RenderPass::DestroyFramebuffers()
        {
            for (int i = 0; i < m_framebuffers.GetIndex(); i++)
            {
                delete m_framebuffers[i];
            }

            delete m_colorImage;
            delete m_colorImageView;

            delete m_depthImage;
            delete m_depthImageView;

            m_framebuffers.SetIndex(0);
        }

        void RenderPass::CreateFramebuffers()
        {
            CreateRenderPassResources(m_physicalDevice, m_device, m_swapchain, m_commandPool);

            // Create framebuffers
            const VkExtent2D scExtent = m_swapchain->GetSwapchainExtent();
            const u32 swapchainImages = m_swapchain->GetSwapchainImageViews().GetIndex();
            SC_TRACE("%i, %i", scExtent.width, scExtent.height);
            for (int i = 0; i < swapchainImages; i++)
            {
                // define the framebuffer attachments
                ArrayList<VkImageView> attachments;
                attachments.Add(m_colorImageView->GetHandle());
                attachments.Add(m_depthImageView->GetHandle());
                attachments.Add(m_swapchain->GetSwapchainImageViews()[i]->GetHandle());

                m_framebuffers.Add(
                    new Framebuffer(m_device, scExtent.width, scExtent.height,
                                    m_renderPass, attachments.GetArray(), attachments.GetIndex()));
            }
        }

        inline void RenderPass::CreateRenderPassResources(PhysicalDevice *physicalDevice, LogicalDevice *device, Swapchain *swapchain, CommandPool *commandPool)
        {
            // --==========================--
            //        COLOR RESOURCES
            // --==========================--
            VkFormat colorFormat = swapchain->GetSwapchainFormat();

            m_colorImage = new Image(physicalDevice, device, swapchain->GetSwapchainExtent().width, swapchain->GetSwapchainExtent().height, 1, physicalDevice->GetDeviceProperties().mssaSamples,
                                     colorFormat,
                                     VK_IMAGE_TILING_OPTIMAL,
                                     VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            m_colorImageView = new ImageView(
                device, m_colorImage->GetHandle(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

            // --==========================--
            //        DEPTH RESOURCES
            // --==========================--
            VkFormat depthFormat = Image::FindSupportedFormat(physicalDevice,
                                                              {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                                              VK_IMAGE_TILING_OPTIMAL,
                                                              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

            m_depthImage = new Image(physicalDevice, device, swapchain->GetSwapchainExtent().width, swapchain->GetSwapchainExtent().height, 1,
                                     physicalDevice->GetDeviceProperties().mssaSamples, depthFormat,
                                     VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            m_depthImageView = new ImageView(device, m_depthImage->GetHandle(),
                                             depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
            m_depthImage->TransitionImageLayout(
                commandPool, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
        }

        RenderPass::~RenderPass()
        {
            for (int i = 0; i < m_framebuffers.GetIndex(); i++)
            {
                delete m_framebuffers[i];
            }

            delete m_colorImage;
            delete m_colorImageView;

            delete m_depthImage;
            delete m_depthImageView;

            vkDestroyRenderPass(m_device->GetHandle(), m_renderPass, nullptr);
        }
    } // namespace vulkan

} // namespace SC
