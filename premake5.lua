--[[
            ~~ STEGPASS PROJECT BUILD SYSTEM ~~
 File: premake5.lua (base)
 
 This is the entry point for the premake build process, which
 will create the Visual Studio solution and project files. This
 file will reference other premake5 files through the project
 hierarchy, triggering their build processes.
 
 To run the build system, go the ./scripts/build-windows.bat
--]]

function script_path()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("(.*/)")
 end

 -- imports
package.path = package.path .. ";" .. script_path()
require "scripts.build_utils"


workspace "StegPass"
    architecture "x64"
    startproject "Stegpass"

    configurations 
    {
        "Debug",
        "Release",
    }

-- Visual Studio Projects
group "steg-backend"
    include "bmp-steg/premake5.lua"

group "tests"
    include "tests/premake5.lua"