
project "Runtime"
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
        "%{IncludeDir.VulkanSDK}"
    }

    links
    {
        "snowcrash",
		"glfw",
        "mingw32",
        "opengl32",
        "gdi32",
        "%{Library.Vulkan}"
    }

    linkoptions
    {
        "-static-libstdc++",
        "-static-libgcc"
    }