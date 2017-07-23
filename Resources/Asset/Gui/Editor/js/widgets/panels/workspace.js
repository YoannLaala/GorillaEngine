class WorkspacePanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Workspace", 
        [
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
            this.onChanged("");
        }

        this.onChanged = function(string)
        {
            //var json = JSON.parse(string);
            var json = 
            [
                {
                    id : "87ef07b0-f0c7-4cb2-9bf5-1e2a66c08a45",
                    name : "fooa",
                    childs: 
                    [
                        {
                            id : "87ef07b0-f0c7-4cb2-9bf5-1e2a66c08a40",
                            name : "fooa.a",
                        },
                        {
                            id : "87ef07b0-f0c7-4cb2-9bf5-1e2a66c08a41",
                            name : "fooa.b",
                            childs: 
                            [
                                {
                                    id : "87ef07b0-f0c7-4cb2-9bf5-1e2a66c08a42",
                                    name : "fooa.b.a",
                                } 
                            ]
                        }  
                    ]
                },
                {
                    id : "87ef07b0-f0c7-4cb2-9bf5-1e2a66c08a43",
                    name : "foob",
                },
                {
                    id : "87ef07b0-f0c7-4cb2-9bf5-1e2a66c08a44",
                    name : "fooc",
                }
            ];
            _self.treeview.set(json);
        }
    }
}