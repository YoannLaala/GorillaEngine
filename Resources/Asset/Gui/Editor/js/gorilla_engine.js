/*!
 * Gorilla Engine
 * Copyright 2013-2017 Yoann Laala
 */
// FileManager
class FileManager
{
    constructor()
    {
        this.openDirectory = function(path) {  window.console.log("Gorilla.File.openDirectory(", path, ")"); } 
        this.createDirectory = function(path) {  window.console.log("Gorilla.File.createDirectory(", path, ")"); } 
        this.rename = function(old_path, new_path) {  window.console.log("Gorilla.File.rename(", old_path, ",", new_path, ")"); } 
        this.getDefaultFileName = function(name, extension) { window.console.log("Gorilla.File.getDefaultFileName(", name, ",", extension, ")"); } 
        this.getPath = function(type) { window.console.log("Gorilla.File.getPath(", type, ")"); } 
    }
}

// WorldManager
class WorldManager
{
    constructor()
    {
        this.create = function() {  window.console.log("GorillaEngine::World::create()"); } 
        this.open = function(path) {  window.console.log("GorillaEngine::World::open(", path, ")"); } 
        this.save = function() {  window.console.log("GorillaEngine::World::save()"); } 
        this.saveAs = function(path) {  window.console.log("GorillaEngine::World::open(", path, ")"); } 
    }
}

// GameObjectManager
class GameObjectManager
{
    constructor()
    {
        this.create = function(parentId) { window.console.log("Gorilla.GameObject.create(", parentId, ")"); }
        this.destroy = function(id) { window.console.log("Gorilla.GameObject.destroy(", id, ")"); }
        this.rename = function(id, name) { window.console.log("Gorilla.GameObject.rename(", id, ",", name, ")"); }
        this.select = function(id) { window.console.log("Gorilla.GameObject.select(", id, ")"); }
    }
}

// ComponentManager
class ComponentManager
{
    constructor()
    {
        this.add = function(gameObject, name) { window.console.log("Gorilla.Component.add(", gameObject, ",", name, ")"); }
        this.remove = function(gameObject, component) { window.console.log("Gorilla.Component.remove(", gameObject, ",", component, ")"); }
        this.set = function(component, offset, type, value) { window.console.log("Gorilla.Component.set(", component, ",", offset, ",", type, ",", value, ")"); }
    }
}

// ProjectManager
class ProjectManager
{
    constructor()
    {
        this.getName = function() { window.console.log("Gorilla.Project.getName()"); }

        this.create = function(path) { window.console.log("Gorilla.Project.create(",path,")"); }
        this.open = function(path) { window.console.log("Gorilla.Project.open(",path,")"); }
        this.save = function() { window.console.log("Gorilla.Project.save()"); }
        this.build = function() { window.console.log("Gorilla.Project.build()"); }
    }
}

// ProfilerManager
class ProfilerManager
{
    constructor()
    {
        this.activate = function() { window.console.log("Gorilla.Profiler.activate()"); }
        this.deactivate = function() { window.console.log("Gorilla.Profiler.deactivate()"); }
    }
}

// Engine
class GorillaEngine 
{ 
    constructor()
    {
        this.File = new FileManager();
        this.World = new WorldManager();
        this.GameObject = new GameObjectManager();
        this.Component = new ComponentManager();
        this.Project = new ProjectManager();
        this.Profiler = new ProfilerManager();

        // generic action
        this.isDebug = false;
        this.exit = function() { window.console.log("Gorilla.exit"); }
        this.play = function() { window.console.log("Gorilla.play"); }
        this.pause = function() { window.console.log("Gorilla.pause"); }
        this.stop = function() { window.console.log("Gorilla.stop"); }
        this.createWindow = function(title, width, height) { window.console.log("Gorilla.createWindow", title, width, height); }
        this.createViewport = function(left, top, width, height) { window.console.log("Gorilla.createViewport", left, top, width, height); }
        this.openScript = function(name) { window.console.log("Gorilla.openScript(", name, ")"); }
        this.createScript = function(name) { window.console.log("Gorilla.createScript(", name, ")"); }
        this.openDialog = function(mode, callback, accept, defaultPath) { window.console.log("Gorilla.openDialog(", mode, ",", callback, ",", accept, ",", defaultPath, ")"); callback([]); }

        this.getAssetExtension = function(name) { window.console.log("Gorilla.getAssetExtension(", name, ")"); }
    }
}

if (navigator.userAgent != "Gorilla") 
{
    Gorilla = new GorillaEngine();
    Gorilla.isDebug = true;
}

// Enumeration
Gorilla.EDialog = 
{
	File            : 0,
	FileMultiple    : 1,
	Folder          : 2,
	Save            : 3
};

// Enumeration
Gorilla.EPath = 
{
	Document        : 0,
	Asset           : 1,
};

// Enumeration
Gorilla.EType = 
{
    Bool            :  0,
    UInt8           :  1,
    UInt16          :  2,
    UInt32          :  3,
    UInt64          :  4,
    Int8            :  5,
    Int16           :  6,
    Int32           :  7,
    Int64           :  8,
    Float32         :  9,
    Float64         :  10,
    String          :  11,
    Asset           :  12,
};

// Default event
function openDialogInternal(button, type) 
{
    Gorilla.openDialog(type, function(files)
    {
        if(files.length == 0) return;

        var input = button.next();
        input.val(files);
        input.change();
    }, button.attr("accept"), button.attr("default_path"));
}
$(document).on('DOMNodeInserted', 'input', function() 
{ 
    window.console.log("ok");
});
$(document).on('click', 'button[type=\'folder\']', function() { openDialogInternal($(this), Gorilla.EDialog.Folder); });
$(document).on('click', 'button[type=\'file\']', function() { openDialogInternal($(this), Gorilla.EDialog.File); });


// Asset
$(document).on('change', 'input.gorilla_asset', function() 
{ 
    var self = $(this);
    var info = self.closest(".gorilla_component");
    var type = parseInt(self.attr("gorilla_type"));
    var value = self.val();
    switch(type)
    {
        case Gorilla.EType.UInt8:
        case Gorilla.EType.UInt16:
        case Gorilla.EType.UInt32:
        case Gorilla.EType.UInt64:
        case Gorilla.EType.Int8:
        case Gorilla.EType.Int16:
        case Gorilla.EType.Int32:
        case Gorilla.EType.Int64:
        case Gorilla.EType.Float32:
        case Gorilla.EType.Float64:
        {
            value = parseInt(value);
            break;
        }

        case Gorilla.EType.Asset:
        {
            var asset = self.attr("gorilla_asset");
            value = asset + "@" + value;
            break;
        }
    }
    Gorilla.Component.set(info.attr("component"), parseInt(self.attr("offset")), type, value);
});