@echo off
call "Engine.Clean.bat"
call "../../ThirdParty/Binaries/premake/premake5.exe" --file=../Solution/Engine.lua --unity-build vs2013 