
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
    void FillImGuiStyle();

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

        FillImGuiStyle();

        // Path p = m_graphicsLayer->m_context->currentProject.projectPath += "systems/Graphics/fonts/Firacode-Light.ttf";
        Path p = m_graphicsLayer->m_context->currentProject.projectPath += "systems/Graphics/fonts/PixelifySans-VariableFont_wght.ttf";
        io.Fonts->AddFontFromFileTTF(p.GetString().c_str(), 16);

        // Setup Dear ImGui style
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

        // Dock space is enabled by default :3
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::DockSpaceOverViewport();
        ImGui::PopStyleColor();

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

    ImVec4 ImGuiIntRGBToFloatRGB(int r, int g, int b)
    {
        return ImVec4(r / 256.0f, g / 256.0f, b / 256.0f, 1.0f);
    }

    void FillImGuiStyle()
    {
        ImGuiStyle *style = &ImGui::GetStyle();
        style->WindowMinSize = ImVec2(160, 20);
        style->FramePadding = ImVec2(4, 4);
        style->ItemSpacing = ImVec2(6, 4);
        style->PopupRounding = 6.0f;
        style->ItemInnerSpacing = ImVec2(6, 4);
        style->Alpha = 0.95f;
        style->WindowRounding = 4.0f;
        style->FrameRounding = 6.0f;
        style->IndentSpacing = 6.0f;
        style->ColumnsMinSpacing = 50.0f;
        style->GrabMinSize = 14.0f;
        style->GrabRounding = 16.0f;
        style->ScrollbarSize = 12.0f;
        style->ScrollbarRounding = 16.0f;
        style->DockingSeparatorSize = 6.0f;

        ImVec4 darkWindowBackground = ImGuiIntRGBToFloatRGB(29, 32, 33);
        ImVec4 windowBackground = ImGuiIntRGBToFloatRGB(40, 40, 40);
        ImVec4 textColor = ImGuiIntRGBToFloatRGB(235, 219, 178);
        ImVec4 textDisabled = ImGuiIntRGBToFloatRGB(235, 219, 178);

        // backgrounds for the things that are not windows
        // buttons and tabs and such
        ImVec4 background1 = ImGuiIntRGBToFloatRGB(60, 56, 54);
        ImVec4 background2 = ImGuiIntRGBToFloatRGB(80, 73, 69);
        ImVec4 background3 = ImGuiIntRGBToFloatRGB(102, 92, 84);

        ImVec4 active = ImGuiIntRGBToFloatRGB(152, 151, 26); // green
        ImVec4 hovered1 = ImGuiIntRGBToFloatRGB(184, 187, 38);
        ImVec4 hovered2 = ImGuiIntRGBToFloatRGB(184, 187, 38);

        style->Colors[ImGuiCol_Tab] = active;
        style->Colors[ImGuiCol_TabActive] = windowBackground;
        style->Colors[ImGuiCol_TabHovered] = hovered2;
        style->Colors[ImGuiCol_TabDimmedSelected] = active;
        style->Colors[ImGuiCol_TabDimmedSelectedOverline] = active;
        style->Colors[ImGuiCol_TabSelectedOverline] = background1;

        style->Colors[ImGuiCol_FrameBg] = background1;
        style->Colors[ImGuiCol_FrameBgHovered] = hovered2;
        style->Colors[ImGuiCol_FrameBgActive] = active;

        style->Colors[ImGuiCol_Text] = textColor;
        style->Colors[ImGuiCol_TextDisabled] = textDisabled;
        style->Colors[ImGuiCol_WindowBg] = windowBackground;
        style->Colors[ImGuiCol_Border] = darkWindowBackground;
        style->Colors[ImGuiCol_BorderShadow] = darkWindowBackground;

        style->Colors[ImGuiCol_TitleBg] = background1;
        style->Colors[ImGuiCol_TitleBgCollapsed] = background3;
        style->Colors[ImGuiCol_TitleBgActive] = active;

        style->Colors[ImGuiCol_MenuBarBg] = background3;
        style->Colors[ImGuiCol_ScrollbarBg] = background1;
        style->Colors[ImGuiCol_ScrollbarGrab] = darkWindowBackground;
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = hovered2;
        style->Colors[ImGuiCol_ScrollbarGrabActive] = active;
        style->Colors[ImGuiCol_CheckMark] = active;

        style->Colors[ImGuiCol_SliderGrab] = background3;
        style->Colors[ImGuiCol_SliderGrabActive] = active;

        style->Colors[ImGuiCol_Button] = background1;
        style->Colors[ImGuiCol_ButtonHovered] = hovered1;
        style->Colors[ImGuiCol_ButtonActive] = active;

        style->Colors[ImGuiCol_Header] = background1;
        style->Colors[ImGuiCol_HeaderHovered] = hovered1;
        style->Colors[ImGuiCol_HeaderActive] = active;

        style->Colors[ImGuiCol_Separator] = darkWindowBackground;
        style->Colors[ImGuiCol_SeparatorHovered] = darkWindowBackground;

        style->Colors[ImGuiCol_SeparatorActive] = active;
        style->Colors[ImGuiCol_ResizeGrip] = background1;
        style->Colors[ImGuiCol_ResizeGripHovered] = hovered2;
        style->Colors[ImGuiCol_ResizeGripActive] = active;
        style->Colors[ImGuiCol_PlotLines] = active;
        style->Colors[ImGuiCol_PlotLinesHovered] = active;
        style->Colors[ImGuiCol_PlotHistogram] = active;
        style->Colors[ImGuiCol_PlotHistogramHovered] = active;
        style->Colors[ImGuiCol_TextSelectedBg] = textColor;
        style->Colors[ImGuiCol_PopupBg] = background2;

        style->Colors[ImGuiCol_DockingPreview] = active;
        style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0, 0, 0, 0);
    }
} // namespace SXC
