/*!
 * Gorilla Engine
 * Copyright 2013-2017 Yoann Laala
 */
if (navigator.userAgent == "Gorilla") Gorilla.isDebug = false;
else
{
    Gorilla = 
    {
        isDebug : true,

        // managers
        Component : new ComponentManager(),
        File : new FileManager(),
        GameObject : new GameObjectManager(),
        Profiler : new ProfilerManager(),
        Project : new ProjectManager(),
        World : new WorldManager(),
    
        // generic action
        exit : function() { window.console.log("Gorilla.exit"); },
        play : function() { window.console.log("Gorilla.play"); },
        pause : function() { window.console.log("Gorilla.pause"); },
        stop : function() { window.console.log("Gorilla.stop"); },
        createWindow : function(title, width, height) { window.console.log("Gorilla.createWindow", title, width, height); },
        createViewport : function(left, top, width, height) { window.console.log("Gorilla.createViewport", left, top, width, height); },
        openScript : function(name) { window.console.log("Gorilla.openScript(", name, ")"); },
        createScript: function (path) { window.console.log("Gorilla.createScript(", path, ")"); },
        openDialog : function(mode, callback, accept, defaultPath) { window.console.log("Gorilla.openDialog(", mode, ",", callback, ",", accept, ",", defaultPath, ")"); callback([]); },
    
        getAssetExtension : function(name) { window.console.log("Gorilla.getAssetExtension(", name, ")"); },
    };
}

// enums
Gorilla.EDialog = 
{
	File            : 0,
	FileMultiple    : 1,
	Folder          : 2,
	Save            : 3,
};

Gorilla.EPath = 
{
	Document        : 0,
	Asset           : 1,
};

    
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

// javascript managers
Gorilla.Template = new TemplateManager();

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