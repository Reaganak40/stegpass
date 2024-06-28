--[[
            ~~ STEGPASS PROJECT BUILD SYSTEM ~~
 File: app/premake5.lua 
 Defines the 'app' Visual Studio project
 
 To run the build system, go the ~/scripts/build-windows.bat
--]]

project_name = "stegpass-app"

project "stegpass-app"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid(project_name .. "UUID"))
    targetdir (binFolder)
    objdir (binObjFolder)
    ignoredefaultlibraries { "LIBCMTD" }

    files { 
        "pch.h",
        "pch.cpp",
        "src/**.hpp",
        "src/**.cpp",
     }
    includedirs {
        "", -- root folder
        "src",

        -- core include folder
        "%{wks.location}/utility/core/include",

        -- graphics include folders
        "%{wks.location}/thirdparty/glfw/glfw/include",
        "%{wks.location}/thirdparty/glad/glad/include",
        "%{wks.location}/thirdparty/imgui/imgui",
        "%{wks.location}/thirdparty/imgui/imgui/backends",

        -- logging include folder
         "%{wks.location}/thirdparty/spdlog/include"
     }

    links { 
        "core",
        "GLFW", "glad", "ImGui"
    }

    pchheader "pch.h"
    pchsource "pch.cpp"

    postbuildcommands{
        GetProjectDLL("core"),
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SP_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        symbols "On"
        staticruntime "off"
        runtime "Debug"
        defines
        {
            "SP_DEBUG",
        }   

    filter "configurations:Release"
        optimize "On"
        staticruntime "off"
        runtime "Release"
        defines
        {
            "SP_RELEASE",
        }