class WorldPanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "World",
        [
            {
			    name: "Create GameObject",
			    onClick: function () 
                { 
                    var id = Editor.panels.world.getSelection();
                    Gorilla.GameObject.create(id); 
                }
            },
            {
			    name: "Rename",
			    onClick: function () 
                { 
                    var tree = self.dom.jstree(true);
                    var selection = tree.get_selected()[0];
                    tree.edit(selection, null, function(node)
                    {
                        var id = Editor.panels.world.getSelection();
                        Gorilla.GameObject.rename(id, node.text);
                    });
                }
            },
            {
                name: "Delete",
                onClick: function () 
                { 
                    var tree = self.dom.jstree(true);
                    var selected = tree.get_selected();
                    if(selected.length != 0)
                    {
                        var id = Editor.panels.world.getSelection();
                        tree.delete_node(selected[0]);
                        Gorilla.GameObject.destroy(id);
                    }
                }
            }
        ]);

        var self =this;
        this.onLoaded = function()
        {
            // triggered when a node is selected (or deselected
            self.dom.on('changed.jstree', function (e, data) 
            {
                var selection = self.getSelection();
                if(selection) Editor.panels.property.activate();
                else Editor.panels.property.deactivate();
                Gorilla.GameObject.select(selection);
            });

            // auto select when right clicking on tree item
            self.dom.on('ready.jstree', function(e, data) 
            {
                var tree = self.dom.jstree(true);
                var items = self.dom.find("a.jstree-anchor");
                items.contextmenu(function()
                {
                    tree.deselect_all(true);
                    tree.select_node($(this).parent().attr("id"));
                });
            })

            // deselect tree element when clicking outside
            self.dom.on("click contextmenu", function(e) 
            {        
                if($(e.target).is("#gorilla_layout_world"))
                {
                    var selection = self.getSelection();
                    if(selection)
                    {
                        var tree = self.dom.jstree(true);
                        tree.deselect_all();
                    }
                }
            })
        }

        this.getSelection = function()
        {
            var tree = self.dom.jstree(true);
            var selected = tree.get_selected();
            return selected.length != 0 ? parseInt(selected[0]) : 0;
        }

        this.onChanged = function(string)
        {
            var json = JSON.parse(string);
            this.dom.jstree(
            {
                "core" : 
                { 
                    "check_callback": true ,
                    "data": json,
                    "themes":
                    {
                        "icons": false
                    }
                }
            });
        }

        this.onGameObjectCreated = function (string) 
        {
            var tree = this.dom.jstree(true);
            var json = JSON.parse(string);
            var parent = tree.get_node(json["parent"]);
            tree.create_node(parent, json);
        }
    }
}