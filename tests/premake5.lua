--[[
            ~~ STEGPASS PROJECT BUILD SYSTEM ~~
 File: tests/premake5.lua 
 Defines the 'UnitTests' Visual Studio project
 
 To run the build system, go the ~/scripts/build-windows.bat
--]]

project_name = "UnitTests"

project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid(project_name .. "UUID"))
    targetdir (binFolder)
    objdir (binObjFolder)

    files
    {
        "**.hpp",
        "**.cpp",

        -- gtest files
        "%{wks.location}/thirdparty/gtest/googletest/googletest/**.h",
        "%{wks.location}/thirdparty/gtest/googletest/googletest/**.hpp",
        "%{wks.location}/thirdparty/gtest/googletest/googletest/src/gtest-all.cc"
    }

    includedirs
    {
        "%{wks.location}/thirdparty/gtest/googletest/googletest/include/gtest", 
        "%{wks.location}/thirdparty/gtest/googletest/googletest/include", 
        "%{wks.location}/thirdparty/gtest/googletest/googletest",

        -- core include folder
        "%{wks.location}/utility/core/include"
    }

    links
    {
        "core"
    }

    postbuildcommands
    {
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

