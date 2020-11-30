-- Premake5 file to generate project files for all supported platforms
-- Author: Steven Savold

-- workspace (Solution) for the entire enigne and other addons/plugins 
workspace "HazmatBrowser"
    architecture "x86_64"
    startproject "HazmatBrowser"

    configurations
    {
         "Debug",
         "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

IncludeDir["hazmat_renderer"] = "HazmatRenderEngine/src"
IncludeDir["spdlog"] = "vendor/lib/hazel/Hazel/vendor/spdlog/include"
IncludeDir["gumbo"] = "vendor/lib/gumbo-parser/src"
IncludeDir["hazel"] = "vendor/lib/hazel/Hazel/src"
IncludeDir["GLFW"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/vendor/lib/hazel/Hazel/vendor/yaml-cpp/include"

-- Definition of Browser Application Project
project "HazmatBrowser"
    location "HazmatBrowser"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.hazmat_renderer}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.gumbo}",
        "%{IncludeDir.hazel}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"
    }

    links
    {
        "HazmatRenderEngine",
        "Hazel"
    }

    filter "system:windows"
        defines
        {
            "HAZMAT_PLATFORM_WINDOWS"
        }

    filter "system:linux"
        defines
        {
            "HAZMAT_PLATFORM_LINUX"
        }

    filter "system:macosx"
        defines
        {
            "HAZMAT_PLATFORM_MACOS"
        }

    -- Filter for Debug configuration
    filter "configurations:Debug*"
        defines 
        {
            "HAZMAT_DEBUG"
        }
        runtime "Debug"
        symbols "on" -- turn debug symbols ON

    -- Filter for Release configuration
    filter "configurations:Release*"
        defines 
        {
            "HAZMAT_RELEASE"
        }
        runtime "Release"
        optimize "Full" -- turn on Full optimizations



-- Definition of Browser Application Project
project "HazmatRenderEngine"
    location "HazmatRenderEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.gumbo}",
    }

    links
    {
        "Gumbo"
    }

    filter "system:windows"
        defines
        {
            "HAZMAT_PLATFORM_WINDOWS"
        }

    filter "system:linux"
        defines
        {
            "HAZMAT_PLATFORM_LINUX"
        }

    filter "system:macosx"
        defines
        {
            "HAZMAT_PLATFORM_MACOS"
        }

    -- Filter for Debug configuration
    filter "configurations:Debug*"
        defines 
        {
            "HAZMAT_DEBUG"
        }
        runtime "Debug"
        symbols "on" -- turn debug symbols ON

    -- Filter for Release configuration
    filter "configurations:Release*"
        defines 
        {
            "HAZMAT_RELEASE"
        }
        runtime "Release"
        optimize "Full" -- turn on Full optimizations



group "Depends"
    -- Definition of the Gumbo Parser Library
    project "Gumbo"
        location "vendor/lib/gumbo-parser"
        kind "StaticLib"
        language "C"
        systemversion "latest"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "vendor/lib/gumbo-parser/src/**.c"
        }

        includedirs
        {
            "vendor/lib/gumbo-parser/visualc/include"
        }

    -- include the Hazel Renderer in this project
    -- Include directories for Hazel relative to root folder (solution directory)
    include "vendor/lib/hazel/Hazel"
    include "vendor/lib/hazel/Hazel/vendor/GLFW"
    include "vendor/lib/hazel/Hazel/vendor/Glad"
    include "vendor/lib/hazel/Hazel/vendor/imgui"
    include "vendor/lib/hazel/Hazel/vendor/yaml-cpp"

group ""
