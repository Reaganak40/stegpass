--[[
            ~~ STEGPASSS PROJECT BUILD SYSTEM ~~
 File: build_utils.lua
 
 Contains the utility functions used by the build process, as
 well as any macros to locate and define modules and projects.
 
 To run the build system, go the ./scripts/build-windows.bat
--]]


--[[ *******************************************************************************
                                Output Paths
                Defines the publish paths for executibles and libraries
******************************************************************************* --]]
outputFolder = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

binFolder = ("%{wks.location}/bin/" .. outputFolder .. "/%{prj.name}")
libFolder = ("%{wks.location}/lib/" .. outputFolder .. "/%{prj.name}")

binObjFolder = ("%{wks.location}/bin/obj/" .. outputFolder .. "/%{prj.name}")
libObjFolder = ("%{wks.location}/lib/obj/" .. outputFolder .. "/%{prj.name}")

-- Copies the dll from the given Project Name to the target directory of the in-scope project
function GetProjectDLL(projName)
    return ("{COPY} %{wks.location}/lib/" .. outputFolder .. "/" .. projName .. "/" .. projName ..  ".dll %{cfg.targetdir}")
end