--[[
            ~~ STEGPASS PROJECT BUILD SYSTEM ~~
 File: bmp-steg/premake5.lua 
 Defines the 'bmp-steg' Visual Studio project
 
 To run the build system, go the ~/scripts/build-windows.bat
--]]

project_name = "bmp-steg"

project "bmp-steg"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid(project_name .. "UUID"))
    targetdir (binFolder)
    objdir (binObjFolder)
    ignoredefaultlibraries { "LIBCMTD" }

    files { 
        "src/**.hpp",
        "src/**.cpp",
     }
    includedirs { 
        "src",

        -- core include folder
        "%{wks.location}/core/include"
     }

    links { 
        "core"
    }

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