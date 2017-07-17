-----------------------------------------------------------
--	Engine Solution Generation
--	2015-04-04
-----------------------------------------------------------
dofile("Core.lua")

project_list = 
{	
	Core = "StaticLib",
	Component = "StaticLib",
	Engine = "StaticLib",
	Renderer = "StaticLib",
	Web = "StaticLib",
	Game = "WindowedApp",
	Editor = "WindowedApp",
	Viewer = "WindowedApp",
}
declare_solution("Engine", project_list)

-- Common Editor Configuraton
function editor_configuration()
	debugdir(get_binaries_path())
	declare_link({ "d3d11.lib", "Shlwapi.lib", "Core", "Component", "Renderer", "Web"})
		
	-- Chromium Embedded Framework
	filter { "configurations:Master or Release" }
		links
		{
			get_third_party_libraries_path() .. "cef/Release/libcef.lib",
			get_third_party_libraries_path() .. "cef/Release/libcef_dll_wrapper.lib"
		}
		
	filter { "configurations:Debug" }
		links
		{
			get_third_party_libraries_path() .. "cef/Debug/libcef.lib",
			get_third_party_libraries_path() .. "cef/Debug/libcef_dll_wrapper.lib"
		}
end

-- Component
project("Component")
	includedirs { get_third_party_libraries_path() .. "cef/" }
	
-- Engine
project("Engine")
	
-- Renderer
project("Renderer")
	-- filter { "system:windows" }
		-- exclude("Metal/MetalRenderer.cpp")
		
-- Web
project("Web")
	includedirs { get_third_party_libraries_path() .. "cef/" }

-- Game
project("Game")
	declare_link({ "Engine" })
	editor_configuration()
	
-- Editor
project("Editor")
	defines { "GORILLA_EDITOR" }
	editor_configuration()
	declare_files(get_solution_source() .. "Engine/")
	
-- Viewer
project("Viewer")
	defines { "GORILLA_EDITOR" }
	editor_configuration()
	declare_files(get_solution_source() .. "Engine/")
	
-- _SolutionPostBuild
project("_SolutionPostBuild")
	location(get_project_path())
	kind "Utility"
	
	files
	{ 
		_ENGINE_PATH_ .. "Resources/Asset/Effect/**",
		_ENGINE_PATH_ .. "Resources/Asset/Gui/**",
	}
	
	filter { "files:**.fx" }
		buildcommands { "call \"" .. get_asset_cooker() .. "\" -type \"Effect\" -input \"%{file.relpath}\" -configuration \"%{cfg.buildcfg}\"" }
		buildoutputs { "%{cfg.objdir}/%{file.basename}.asset" }
		
	filter { "files:**.vs or **.gs or **.ps or **.cs" }
		buildcommands { "call \"" .. get_asset_cooker() .. "\" -type \"Shader\" -input \"%{file.relpath}\" -configuration \"%{cfg.buildcfg}\"" }
		buildcommands { "call \"" .. get_asset_cooker() .. "\" -type \"Shader\" -input \"%{file.relpath} -technique Instancing\" -configuration \"%{cfg.buildcfg}\"" }
		buildoutputs { "%{cfg.objdir}/%{file.basename}.asset" }
	
	filter { "files:**_**_.html" }
		buildcommands { "call \"" .. get_asset_cooker() .. "\" -type \"WebDocument\" -input \"%{file.relpath}\" -configuration \"%{cfg.buildcfg}\"" }
		
	buildoutputs { "%{cfg.objdir}/%{file.basename}.asset" }
	
	-- Chromium Embedded Framework
	filter { "configurations:Master or Release" }
		copy_file("ThirdParty/Libraries/cef/Release/chrome_elf.dll", "")
		copy_file("ThirdParty/Libraries/cef/Release/libcef.dll", "")
		copy_file("ThirdParty/Libraries/cef/Release/libEGL.dll", "")
		copy_file("ThirdParty/Libraries/cef/Release/libGLESv2.dll", "")
		copy_file("ThirdParty/Libraries/cef/Release/natives_blob.bin", "")

	filter { "configurations:Debug" }
		copy_file("ThirdParty/Libraries/cef/Debug/chrome_elf.dll", "")
		copy_file("ThirdParty/Libraries/cef/Debug/libcef.dll", "")
		copy_file("ThirdParty/Libraries/cef/Debug/libEGL.dll", "")
		copy_file("ThirdParty/Libraries/cef/Debug/libGLESv2.dll", "")
		copy_file("ThirdParty/Libraries/cef/Debug/natives_blob.bin", "")
	
	filter {}
	
	-- Libraries
	copy_file("Libraries/Engine/Core/%{cfg.buildcfg}/%{cfg.platform}/*", "Resources/Libraries/")
	copy_file("Libraries/Engine/Renderer/%{cfg.buildcfg}/%{cfg.platform}/*", "Resources/Libraries/")
	copy_file("Libraries/Engine/Engine/%{cfg.buildcfg}/%{cfg.platform}/*", "Resources/Libraries/")
	copy_file("Libraries/Engine/Web/%{cfg.buildcfg}/%{cfg.platform}/*", "Resources/Libraries/")
	
	-- Copy files
	copy_file("Resources/Database/*", "Resources/Database/")
	copy_file("ThirdParty/Libraries/cef/Resources/icudtl.dat", "")
	copy_file("ThirdParty/Libraries/cef/Resources/cef.pak", "Resources/")
	copy_file("ThirdParty/Libraries/cef/Resources/cef_100_percent.pak", "Resources/")
	copy_file("ThirdParty/Libraries/cef/Resources/cef_200_percent.pak", "Resources/")
	copy_file("ThirdParty/Libraries/cef/Resources/cef_extensions.pak", "Resources/")
	copy_file("ThirdParty/Libraries/cef/Resources/devtools_resources.pak", "Resources/")
	copy_file("ThirdParty/Libraries/cef/Resources/locales/*", "Resources/locales/")
	
	-- Includes
	copy_file("Sources/Engine/Engine/System/*", "Resources/Include/Engine/System/")
	
	--Asset required
	cook_reflection(get_solution_source() .. "Component/", get_binaries_path() .. "Resources/Database/ComponentDescriptor.json")
	cook_asset("Mesh", "Mesh/Cube.fbx")
	cook_asset("Mesh", "Mesh/Quad.fbx")
	cook_asset("Mesh", "Mesh/Torus.fbx")
	copy_file("Resources/Template/Script/*", "Resources/Template/Script/")
