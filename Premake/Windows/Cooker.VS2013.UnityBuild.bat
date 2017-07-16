@echo off
call "Cooker.Clean.bat"
call "../../ThirdParty/Binaries/premake/premake5.exe" --file=../Solution/Cooker.lua --unity-build vs2013 