class WorkspacePanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Workspace", null);
		
        var _self = this;
        _self.onLoaded = function()
        {
            _self.treeview = new TreeView(_self.dom);
			$.contextMenu(
			{
				selector: '#gorilla_layout_workspace', 
				autoHide: true,
				zIndex: 2000,
                animation: { duration: 0 },
                events: 
                {
                    show : function(options)
                    {
                        if(!options.items.create.visible())
                        {
                            options.items.separator = { visible: false };
                        }
                        return true;
                    }            
                },
				items: 
				{
					create:
					{
						name: "Create",
						items:
						{
							script:
							{
								name: "Script",
								callback: function() 
								{
									var node = Editor.panels.workspace.treeview.selection;
									var path = node != null ? node.id : "";
									Gorilla.createScript(path);
								},
							},
							folder:
							{
								name: "Folder", 
                                callback: function() 
								{
									var node = Editor.panels.workspace.treeview.selection;
									var path = node != null ? node.id : "";
									Gorilla.File.create(path);
								},
							}
						},
                        visible: function()
                        {
                            var node = Editor.panels.workspace.treeview.selection;
							return node == null || node.childs != null;
                        },
					},
					separator: "---------",
					showInFolder:
					{
						name: "Show in folder",
                        callback: function() 
						{
							var node = Editor.panels.workspace.treeview.selection;
							var path = node != null ? node.id : "";
							Gorilla.File.show(path);
						},
					},
					rename:
					{
						name: "Rename",
                        callback: function() 
						{
							var node = Editor.panels.workspace.treeview.selection;
                            var path_old = node.id;
                            _self.treeview.edit(node, function()
                            {
                                Gorilla.File.rename(path_old, node.id);    
                            });
							
						},
                        visible: function()
                        {
                            var node = Editor.panels.workspace.treeview.selection;
							return node != null;
                        },
					},
					delete:
					{
						name: "Delete",
                        callback: function() 
						{
							var node = Editor.panels.workspace.treeview.selection;
							Gorilla.File.delete(node.id);
						},
                        visible: function()
                        {
                            var node = Editor.panels.workspace.treeview.selection;
							return node != null;
                        },
					}
				}
			});
        }

        _self.onChanged = function(string)
        {
            var json = JSON.parse(string);
            _self.treeview.set(json);
        }
    }
}