
#include "ImGuiSubrender.hpp"

#include <windows.h>
#include <imgui/imgui.h>

// #if 0
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// #endif

#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace SC
{
    void ImGuiVulkanDebugCallback(VkResult result)
    {
        // TODO get this working lol
        // SC_WARN("ImGuiVulkanDebugCallback");
    }

    ImGuiSubrender::ImGuiSubrender(GraphicsLayer *layer)
        : Subrender(layer) {}

    void ImGuiSubrender::CreateImGuiRenderPass()
    {
        VkAttachmentDescription attachment = {};
        attachment.format = m_graphicsLayer->m_swapchain->GetSwapchainFormat();
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0; // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;
        if (vkCreateRenderPass(m_graphicsLayer->m_logicalDevice->GetHandle(), &info, nullptr, &m_imguiRenderPass) != VK_SUCCESS)
        {
            SC_ERROR("Could not create Dear ImGui's render pass");
        }
    }

    void ImGuiSubrender::Init()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Path p = m_graphicsLayer->m_context->currentProject.projectPath += "systems/Graphics/fonts/Firacode-Light.ttf";
        Path p = m_graphicsLayer->m_context->currentProject.projectPath += "systems/Graphics/fonts/PixelifySans-VariableFont_wght.ttf";
        io.Fonts->AddFontFromFileTTF(p.GetString().c_str(), 16);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForVulkan(m_graphicsLayer->m_window->GetHandle(), true);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_graphicsLayer->m_instance->GetHandle();
        init_info.PhysicalDevice = m_graphicsLayer->m_physicalDevice->GetHandle();
        init_info.Device = m_graphicsLayer->m_logicalDevice->GetHandle();
        init_info.QueueFamily = m_graphicsLayer->m_logicalDevice->GetGraphicsQueueFamily();
        init_info.Queue = m_graphicsLayer->m_logicalDevice->GetGraphicsQueue();
        // init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = m_graphicsLayer->m_descriptorPool->Get();
        // init_info.Subpass = 0;
        init_info.MinImageCount = 2;
        init_info.ImageCount = 2;
        init_info.MSAASamples = m_graphicsLayer->m_physicalDevice->GetDeviceProperties().mssaSamples;
        init_info.RenderPass = m_graphicsLayer->m_renderPass->GetHandle();

        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = ImGuiVulkanDebugCallback;
        ImGui_ImplVulkan_Init(&init_info);

        vulkan::CommandBuffer command_buffer = m_graphicsLayer->m_commandPool->StartSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture();
        command_buffer.EndSingleTimeCommands();

        m_graphicsLayer->m_logicalDevice->DeviceWaitIdle();
    }

    void ImGuiSubrender::Render(VkCommandBuffer buffer)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        RenderPasses(buffer);

        ImGui::Render();
        ImDrawData *main_draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(main_draw_data, buffer);
    }

    void ImGuiSubrender::RecreateSwapchain(VkExtent2D size)
    {
    }

    ImGuiSubrender::~ImGuiSubrender()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
} // namespace SXC
