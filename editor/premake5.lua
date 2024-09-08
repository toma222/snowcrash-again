
project "Editor"
    kind "ConsoleApp"
    files "src/**"

    includedirs 
    {
        "%{wks.location}/snowcrash/src",
        "%{wks.location}/external",
        "%{wks.location}/external/glm",
		"%{wks.location}/external/glfw/include",
        "%{wks.location}/external/VulkanMemoryAllocator/include",
        "src",
        "%{IncludeDir.VulkanSDK}",
        "%{wks.location}/external/imgui",
        "%{wks.location}/external/imgui/backends"
    }

    links
    {
        "snowcrash",
		"glfw",
        "mingw32",
        "opengl32",
        "gdi32",
        "%{Library.Vulkan}",
        "imgui"
    }

    linkoptions
    {
        "-static-libstdc++",
        "-static-libgcc"
    }