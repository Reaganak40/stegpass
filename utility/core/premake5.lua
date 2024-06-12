--[[
            ~~ STEGPASS PROJECT BUILD SYSTEM ~~
 File: core/premake5.lua 
 Defines the 'core' Visual Studio project
 
 To run the build system, go the ~/scripts/build-windows.bat
--]]

project_name = "core"

project "core"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid(project_name .. "UUID"))
    targetdir (libFolder)
    objdir (libObjFolder)
    ignoredefaultlibraries { "LIBCMTD" }

    files { 
        "pch.h",
        "pch.cpp",
        "src/**.h",
        "src/**.cpp",
        "src/**.c",
        "include/**.hpp"
     }
    includedirs { 
        "src",
        "include",
        ""
     }

    links { 
    }

    postbuildcommands{
    }

    pchheader "pch.h"
    pchsource "pch.cpp"

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SP_PLATFORM_WINDOWS",
            "SP_BUILD_CORE"
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