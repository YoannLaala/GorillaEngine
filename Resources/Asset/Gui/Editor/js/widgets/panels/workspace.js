class WorkspacePanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Workspace", 
        [
            {
                name: 'Create Script',
                onClick: function () 
                { 
                    var node = Editor.panels.workspace.treeview.selection;
                    var parent = node != null ? node.id : "";
                    Gorilla.createScript(parent);
                }
            },
            {
                name: 'Create Directory',
                onClick: function () 
                { 
                    var node = Editor.panels.workspace.treeview.selection;
                    var parent = node != null ? node.id : "";
                    Gorilla.File.create(parent);
                }
            },
            {
                name: 'Show In Folder',
                onClick: function () 
                { 
                    var node = Editor.panels.workspace.treeview.selection;
                    if(node) Gorilla.File.open(node.id);
                }
            },
            {
                name: 'Rename',
                onClick: function () 
                { 
                    var node = Editor.panels.workspace.treeview.selection;
                    if(node) Gorilla.File.rename(node.id, "newname");
                }
            },
            {
                name: 'Delete',
                onClick: function () 
                { 
                    var node = Editor.panels.workspace.treeview.selection;
                    if(node) Gorilla.File.delete(node.id);
                }
            },
        ]);

        var _self = this;
        _self.onLoaded = function()
        {
            _self.treeview = new TreeView(_self.dom);
        }

        this.onChanged = function(string)
        {
            var json = JSON.parse(string);
            _self.treeview.set(json);
        }
    }
}