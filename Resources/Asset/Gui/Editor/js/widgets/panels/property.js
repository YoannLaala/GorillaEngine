class PropertyPanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Property", null);
        var self = this;

        this.module = {};
        this.onModuleChanged = function(string)
        {
            self.descriptors = JSON.parse(string);

            var group = [];
            var actions = 
            {
                createScript:
                {
                    name: "Create Script",
                    onClick: function () 
                    { 
                        var string = Gorilla.File.getDefaultFileName("Script", "hpp");
                        var info = JSON.parse(string);
                        var data = { name : info.name, create : true };
						Gorilla.Template.show(self.dom, "property_component", data, false);

                        var component = self.dom.children().last();
                        var input = new GorillaInput(
                        {
                            dom : component,
                            isValid : function(value)
                            {
                                if(!value.match(/\s/g))
                                {
                                    if(info.list.indexOf(value) === -1) 
                                    {
                                        return true;
                                    }
                                }
                                return false;
                            },
                            onSuccess : function(value)
                            {
                                component.remove();
                                var data = { name : value, display : true };
								Gorilla.Template.show(self.dom, "property_component", data, false);
                                Gorilla.createScript(value); 
                            },
                            getError : function()
                            {
                                return "Invalid Component name ([a-Z][0-9])";
                            }
                        });
                    }
                }  
            };
            
            for(var name in self.descriptors)
            {
                group.push(name);
                actions[name] =
                {
                    name: name,
                    onClick: function () 
                    { 
                        var selection = Editor.panels.world.getSelection();
                        Gorilla.Component.add(selection, this.name);     
                    }
                };
            }   
            
            self.context_menu_data = 
            { 
                actions: actions, 
                actionsGroups: 
                [
                    ['createScript'],
                    group
                ] 
            };

            self.deactivate();
            self.activate();
        }

        this.isActivated = false;
        this.activate = function() 
        { 
            if(!self.isActivated)
            {
                self.context_menu = new BootstrapMenu("#" + this.id, self.context_menu_data); 
                self.isActivated = true; 
            }
        }
        this.deactivate = function() 
        { 
            delete self.context_menu; 
            self.isActivated = false; 
            self.dom.html("");
        }

        // Triggered when a new GameObject is selected in the world panel
        this.onSelectionChanged = function(string)
        {
            var html = "";
            if(string.length)
            {
                var data = { default_path : Gorilla.File.getPath(Gorilla.EPath.Asset) };
                var components = JSON.parse(string).components;
                for(var component_name in components)
                {
                    var descriptor = this.descriptors[component_name]; 
                    if(descriptor != null)
                    {
                        var component = components[component_name];   
                        var namespace_name = component_name.split('::');
                        data.name = namespace_name[namespace_name.length-1];
                        data.class = component_name;
                        data.display = true;
                        data.fields = [];

                        for (var field_name in component)
                        {
                            var field = descriptor.fields[field_name];
                            var value = component[field_name];
                            var field_data = { type: field.element_type, name : field_name, offset: field.offset, value: value };
                            if(field.type === "Gorilla::Engine::AssetHandle")
                            {
                                field_data.extension = Gorilla.getAssetExtension(field.element_type);  
                            }
                            field_data[field.type] = true;
                            data.fields.push(field_data);
                        }
                        html += Gorilla.Template.render("property_component", data);
                    }
                }
            }
            this.dom.html(html);
        }
    }
}