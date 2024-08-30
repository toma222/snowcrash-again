
project "snowcrash"
    kind "StaticLib"

    files "src/**"

    includedirs
    {
        "src",
        "%{wks.location}/external",
		"%{wks.location}/external/glfw/include",
        "%{wks.location}/external/glm",
        "%{wks.location}/external/VulkanMemoryAllocator/include",
        "%{IncludeDir.VulkanSDK}"
    }

	links
	{
		"glfw",
        "%{Library.Vulkan}"
	}

