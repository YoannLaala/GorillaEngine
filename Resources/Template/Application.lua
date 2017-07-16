-----------------------------------------------------------
--	Application Solution Generation
--	2015-04-04
-----------------------------------------------------------
_SOLUTION_DIR_ = "${SOLUTION_DIR}"
_SOLUTION_NAME_ = "${SOLUTION_NAME}"
_TARGET_DIR_ = "${TARGET_DIR}"
_TARGET_NAME_ = "${TARGET_NAME}"
solution(_SOLUTION_NAME_)
    configurations{ "Debug", "Release", "Master" }
    location(_SOLUTION_DIR_)
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

	-- Script
	project(_SOLUTION_NAME_)
		defines { "GORILLA_GAME" }	
		location(_SOLUTION_DIR_)
		kind("SharedLib")
		objdir (_TARGET_DIR_ .. "Temp/")
		debugdir(_TARGET_DIR_ .. "Temp/")
		targetdir(_TARGET_DIR_)
		targetname(_TARGET_NAME_)
		targetextension(".asset")
		files
		{ 
			${SCRIPT_FILE}
		}
		
		includedirs 
		{ 
			${ENGINE_INCLUDE}
		}
		
		links
		{ 
			${ENGINE_LINK}			
		}	