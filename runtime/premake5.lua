
project "Runtime"
    kind "WindowedApp"
    files "src/**"

    includedirs 
    {
        "%{wks.location}/snowcrash/src",
        "src"
    }

    links
    {
        "snowcrash"
    }
