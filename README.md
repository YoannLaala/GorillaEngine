# Gorilla Engine
Gorilla is a 3D Game Engine designed to be performant, easy to use & extend. It simplifies all technical aspects so you can focus on the creation itself and iterates really fast. The engine is not only limited to games - it can also be used to make desktop application.

**This is in early alpha stage - Work in progress**

## System Requirements
- Windows is the only platform supported at the moment.
- Visual Studio 2013 is the only IDE supported at the moment.

## Source organization
* Premake/        -> Generate and copy runtime depdendencies
  * Windows/      -> Specific platform helpers (.bat)
  * Solution/     -> lua files used by premake to build solutions
* Resources/      -> Assets, Database, Template, etc..
* Sources/
  * Cooker/       -> All cpp source files for the cookers
  * Engine/       -> All cpp source files for the engine

## Solution Generation
Gorilla is using premake5 to generate the differents solutions. You can use directly the solution files provided in Premake/Solution/
```
premake5 --file=Premake/Solution/Engine.lua vs2013
premake5 --file=Premake/Solution/Cooker.lua vs2013
```
Alternatively you can use directly the batch located in Premake/Windows/
```
Cooker.VS2013.UnityBuild.bat
Engine.VS2013.UnityBuild.bat
```
The unity build is based on [UnityMake][https://github.com/YoannLaala/UnityMake] to reduce compilation time.

## Engine Solution
The Engine solution is composed of severals projects:
* _SolutionPostBuild (Custom)   -> Generate and copy runtime depdendencies
* Component (Static)            -> Intrinsic Components
* Core (Static)                 -> Fast and simple data structure, io, math, etc
* Editor (App)                  -> Editor to create games & applications
* Game (App)                    -> Executable when build a project from the Editor
* Renderer (Static)             -> Renderer with DX11 implementation
* Viewer (App)                  -> Sandbox for the Engine without the Editor
* Web (Static)                  -> Handle Web views logic and rendering

## Cooker Solution
The Cooker solution contains all the applications to transform the asset from source to Gorilla format. The Engine will be using those applications at runtime to generate and load the asset. *_SolutionPostBuild* is also using them to generate intrinsic assets for example.

## Dependencies
In order to compile, you will need to download a ThirdParty SDK located here.

## Road Map
You can follow the roadmap on [Trello](https://trello.com/b/hlFdhJCv/gorilla-engine)