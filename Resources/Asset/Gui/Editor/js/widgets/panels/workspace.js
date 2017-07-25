class WorkspacePanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Workspace", null);
		
		
		
		
		
		// {
			// fetchElementData: function() 
			// {
				// return Editor.panels.workspace.treeview.selection;
			// },
			// actions : 
			// [
				// {
					// name: 'Create Script',
					// onClick: function (node) 
					// { 
						// var parent = node != null ? node.id : "";
						// Gorilla.createScript(parent);
					// }
				// },
				// {
					// name: 'Create Folder',
					// onClick: function (node) 
					// { 
						// var parent = node != null ? node.id : "";
						// Gorilla.File.create(parent);
					// }
				// },
				// {
					// name: 'Show In Folder',
					// onClick: function (node) 
					// { 
						// if(node) Gorilla.File.openFolder(node.id);
					// },
					// isShown: function(node) { return node != null; }
				// },
				// {
					// name: 'Rename',
					// onClick: function (node) 
					// { 
						// if(node) Gorilla.File.rename(node.id, "newname");
					// },
					// isShown: function(node) { return node != null; }
				// },
				// {
					// name: 'Delete',
					// onClick: function (node) 
					// { 
						// if(node) Gorilla.File.delete(node.id);
					// },
					// isShown: function(node) { return node != null; }
				// },
				// {
					// name: 'test',
					// actions: [{
					  // name: 'Action',
					  // onClick: function() {
						// // run when the action is clicked
					  // }
					// }, {
					  // name: 'Another action',
					  // onClick: function() {
						// // run when the action is clicked
					  // }
					// }, {
					  // name: 'A third action',
					  // onClick: function() {
						// // run when the action is clicked
					  // }
				  // }]
				// },
			// ]
		// });
		
        var _self = this;
        _self.onLoaded = function()
        {
            _self.treeview = new TreeView(_self.dom);
			$.contextMenu(
			{
				selector: '#gorilla_layout_workspace', 
				autoHide: true,
				zIndex: 2000,
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
									var parent = node != null ? node.id : "";
									Gorilla.createScript(parent);
								},
							},
							folder:
							{
								name: "Folder"
							}
						}
						
					},
					separator: "---------",
					showInFolder:
					{
						name: "Show in folder"
					},
					rename:
					{
						name: "Rename"
					},
					delete:
					{
						name: "Delete"
					}
				}
			});
			
			_self.onChanged("");
        }

        this.onChanged = function(string)
        {
            //var json = JSON.parse(string);
			var json = 
			[
				{
					id: "/FolderA/",
					name : "FolderA",
					childs:
					[
						{
							id: "/FolderA/FolderAA/",
							name : "FolderAA",
							childs:
							[
								{
									id: "/FolderA/FolderAA/FileC.ext",
									name : "FileC.ext"
								},
							]
						},
						{
							id: "/FolderA/FolderAB/",
							name : "FolderAB",
							childs:
							[
								
							]
						},
					]
				},
				{
					id: "/FolderB/",
					name : "FolderB",
					childs:
					[
						{
							id: "/FolderB/FileD.ext",
							name : "FileD.ext"
						},
					]
				},
				{
					id: "/FileA.a",
					name : "FileA.a"
				},
				{
					id: "/FileB.b",
					name : "FileB.b"
				}
			]
            _self.treeview.set(json);
        }
    }
}