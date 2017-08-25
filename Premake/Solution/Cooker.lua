-----------------------------------------------------------
--	Cooker Solution Generation
--	2015-04-04
-----------------------------------------------------------
dofile("Core.lua")

project_list = 
{
	Helper = "StaticLib",

	AssetCooker = "ConsoleApp",
	Effect = "ConsoleApp",
	Json2Binary = "ConsoleApp",
	Mesh = "ConsoleApp",
	Module = "ConsoleApp",
	Reflection = "ConsoleApp",
	Shader = "ConsoleApp",
	Texture = "ConsoleApp",
	Web = "ConsoleApp",
}
declare_solution("Cooker", project_list)

-- Helper
project "Helper"
	declare_dependency( "Engine", { "Core" })

-- AssetCooker
project "AssetCooker"
	declare_dependency( "Engine", { "Core", "Engine", "Renderer" })
	declare_link({ "Helper" })
	links({ "d3d11.lib" , "Shlwapi.lib" })

	declare_output( get_third_party_binaries_path() .. "premake/")

-- Effect
project "Effect"
	declare_dependency( "Engine", { "Core" })
	declare_link({ "Helper", "Shlwapi.lib" })

-- Json2Binary
project "Json2Binary"	
	declare_dependency( "Engine", { "Core" })
	declare_link({ "Helper" })
	links({ "Shlwapi.lib" })
	
-- Mesh
project "Mesh"
	declare_dependency( "Engine", { "Core" })
	declare_link({ "Helper", "Shlwapi.lib" })
	
	includedirs { get_third_party_libraries_path() .. "fbx/include/" }
	
	filter { "configurations:Master or Release"}
		links
		{
			get_third_party_libraries_path() .. "fbx/lib/vs2013/%{cfg.platform}/Release/libfbxsdk-md.lib",
		}
	filter {"configurations:Debug"}
		links 
		{ 
			get_third_party_libraries_path() .. "fbx/lib/vs2013/%{cfg.platform}/%{cfg.buildcfg}/libfbxsdk-md.lib",
		}

-- Module
project "Module"
	declare_dependency( "Engine", { "Core" })
	links({ "Shlwapi.lib"})
	copy_file("ThirdParty/Binaries/premake/*", "Cooker/Module/")
	copy_file("Resources/Template/Application.lua", "Cooker/Module/")
	
-- Reflection
project "Reflection"	
	declare_dependency( "Engine", { "Core" })
	declare_link({ "Helper" })
	links({ get_third_party_libraries_path() .. "clang-c/libclang.lib", "Shlwapi.lib" })
	
	copy_file("ThirdParty/Libraries/clang-c/libclang.dll", "Cooker/Reflection/")
		
-- Shader
project "Shader"
	declare_dependency( "Engine", { "Core" })
	declare_link({ "Helper" })
	links 
	{ 
		"dxguid.lib",
		"d3dcompiler.lib",
		"Shlwapi.lib"
	}
	copy_file("ThirdParty/Binaries/fxc/*", "Cooker/Shader/")
	
-- Texture
project "Texture"
	declare_dependency( "Engine", { "Core", "Renderer" })
	declare_link({ "Helper" })
	links({ "d3d11.lib", "Shlwapi.lib" })
	
	copy_file("ThirdParty/Binaries/nvdxt/*", "Cooker/Texture/")
	
-- Web
project "Web"
	declare_dependency( "Engine", { "Core" })
	declare_link({ "Helper" })
	filter { "configurations:Master or Release"}
		links
		{
			get_third_party_libraries_path() .. "gumbo-parser/lib/%{cfg.platform}/Release/gumbo.lib",
			"Shlwapi.lib"
		}
	filter {"configurations:Debug"}
		links 
		{ 
			get_third_party_libraries_path() .. "gumbo-parser/lib/%{cfg.platform}/%{cfg.buildcfg}/gumbo.lib",
			"Shlwapi.lib"
		}