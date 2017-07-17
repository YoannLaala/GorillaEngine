-- Custom option
_ENGINE_PATH_ = "../../"
newoption
{
	trigger     = "unity-build",
	description = "Set the project dor a unity build"
}

function isUnityBuild()
	return _OPTIONS["unity-build"] ~= nil
end

-- Get Project Name
function get_project_name()
	return project().name
end

-- Get Solution Name
-- Workaround avoiding to loose the project context
function get_solution_name()
	local project_name = get_project_name()
	local solution_name = solution().name
	project(project_name)
	return solution_name
end

-- Get Solution Source
function get_solution_source(name)
	local solution_name = name or get_solution_name()
	return _ENGINE_PATH_ .. "Sources/" .. solution_name .. "/"
end

-- Get Project Source
function get_project_source()
	return get_solution_source() .. get_project_name() .. "/"
end

-- Get Solution Path
function get_solution_path(name)
	local solution_name = name or get_solution_name()
	return _ENGINE_PATH_ .. "Solution/" .. solution_name .. "/"
end

-- Get Project path
function get_project_path()
	return get_solution_path() .. get_project_name() .. "/"
end

-- The path for the libraries
function get_binaries_path()
	return _ENGINE_PATH_ .. "Binaries/%{cfg.buildcfg}/"
end

-- The path for the resources
function get_resources_path()
	return _ENGINE_PATH_ .. "Resources/"
end

-- The path for the libraries
function get_libraries_path()
	return _ENGINE_PATH_ .. "Libraries/"
end

-- The path for the third party libraries
function get_third_party_libraries_path()
	return _ENGINE_PATH_ .. "ThirdParty/Libraries/"
end

-- The path for the third party binaries
function get_third_party_binaries_path()
	return _ENGINE_PATH_ .. "ThirdParty/Binaries/"
end

-- The path for the Cookers
function get_cooker_path()
	return "../" .. get_binaries_path() .. "Cooker/"
end

-- The AssetCooker Executable filepath
function get_asset_cooker()
	return "../" .. get_third_party_binaries_path() .. "premake/AssetCooker.exe"
end

-- Get a Static lib path from the solution
function get_library_path(name)
	local project_name = name or get_project_name()
	local solution_name = get_solution_name()
	return get_libraries_path() .. solution_name .. "/" .. project_name .. "/%{cfg.buildcfg}/%{cfg.platform}/"
end

-- Get a Static lib filepath from the solution
function get_library(name)
	local project_name = name or get_project_name()
	return get_library_path(project_name) .. project_name .. ".lib"
end

-- execute a script
function execute_script(relative_path, arguments)
	python = ""
	if os.is("windows") then python = path.getabsolute(_ENGINE_PATH_ .. "Scripts/Pc/Python276/python.exe") end
	if os.is("macosx") then python = "python" end
	file_absolute = path.getabsolute(relative_path)

	os.execute(python .. " " .. file_absolute .. ' ' .. table.concat(arguments, " "))
end

-- Exclude a file
function exclude(relative_path)
	relative_path_from_project = get_project_source() .. relative_path
	excludes { relative_path_from_project }
	
	if isUnityBuild() then
		execute_script(_ENGINE_PATH_ .. "Scripts/ExcludeFile.py", {"--project_path", get_project_path(), "--file_to_remove", path.getabsolute(relative_path_from_project)})
	end
end

-- Copy file during prebuild command
function copy_file(input, output)
    prebuildcommands { "{COPY} ../../../" .. input .. " ../" .. get_binaries_path() .. output }
end

-- cook an asset
function cook_asset(asset_type, asset_path)
	prebuildcommands {"call \"" .. get_asset_cooker() .. "\" -type \"" .. asset_type .. "\" -input \"" .. asset_path .."\" -configuration \"%{cfg.buildcfg}\""}
end

-- cook a module
function cook_reflection(input_path, output_path)
	local absolute_path  = path.getabsolute(".")
	prebuildcommands {"call \"" .. get_cooker_path() .. "Reflection/Reflection.exe\" -input \"" .. absolute_path .. "/" .. input_path .."\" -output \"" .. absolute_path .. "/" .. output_path .. "\""}
end

-- Declare a project
function declare_project(project_name, project_kind)
project(project_name)
	local project_path = get_project_path()
	location(project_path)
	kind(project_kind)

	declare_files(get_project_source())
	
	includedirs 
	{ 
		get_libraries_path(), 
		get_third_party_libraries_path(),
		get_solution_source() 
	}
	
	filter { "kind:StaticLib or SharedLib" }
		declare_output(get_library_path())
		
	-- Windowed	App
	filter { "kind:WindowedApp" }
		flags { "WinMain" }
		declare_output(get_binaries_path())
		targetsuffix "64"
		
	filter { "kind:ConsoleApp" }
		declare_output(get_binaries_path() .. "/Cooker/" .. project_name)
		
end

-- Declare a solution
function declare_solution(solution_name, project_list)
solution(solution_name)
    configurations{ "Debug", "Release", "Master" }
    local solution_path = get_solution_path(solution_name)
    location(solution_path)
    platforms { "x64" }
	characterset ("MBCS")

    filter { "platforms:x64" }
        architecture "x64"

    filter { "system:macosx" }
        defines { "PLATFORM_MAC" }

    filter { "system:windows" }
        defines { "PLATFORM_PC" }

	flags { "ExtraWarnings", "FatalWarnings" }
	
	-- Define + Optimization per configuration
	filter { "configurations:Debug" }
		defines { "DEBUG" }
		optimize "Off"
		symbols "On"
		
	filter { "configurations:Release or Master"}
		defines { "NDEBUG" }
	
	filter { "configurations:Release" }
		optimize "On"
	
	filter { "configurations:Master" }
		defines { "MASTER" }
		optimize "Full"

	-- Declare all Project to the solution
	for project_name, project_kind in pairs(project_list) do
		declare_project(project_name, project_kind)
	end
end

-- Declare all files to include for the project
function declare_files(directory)
	local project_path = get_project_path()	
	files
	{ 
		directory .. "**.h**",
		directory .. "**.c**",
	}
	
	if isUnityBuild() then
		local input_path = path.getabsolute(directory) .. "/"
		local output_path = path.getabsolute(project_path) .. "/"
		local unity_make = path.getabsolute(_ENGINE_PATH_ .. "ThirdParty/Binaries/premake/UnityMake.exe")
		os.execute(unity_make .. " --input " .. input_path .. " --output " .. output_path)
		files
		{ 
			project_path .. "_unity_files_/**.cpp",
		}
		
		-- remove from the build all source cpp
		filter { 'files:' .. directory .. '**.c**' }
			flags { "ExcludeFromBuild" }
	end
end

-- Declare all link for a project
function declare_link(lib_list)
	for i=1,table.getn(lib_list) do
		links { lib_list[i] }
	end
end

-- Declare all link for a project in a different
function declare_dependency(solution_name, lib_list)
	for i=1,table.getn(lib_list) do
		includedirs { get_solution_source(solution_name) }
		links { get_libraries_path() .. solution_name .. "/" .. lib_list[i] .. "/%{cfg.buildcfg}/%{cfg.platform}/" .. lib_list[i] }
	end
end

function declare_output(output)
	debugdir(output)
	targetdir(output)
end
