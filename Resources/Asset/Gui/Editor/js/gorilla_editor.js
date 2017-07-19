function execute_callback(callback) { callback(); }



class GorillaPanel
{
    constructor(layoutManager, name, actions)
    {
        var self = this;
        layoutManager.registerComponent(name, function( container, state )
        {
            self.id = 'gorilla_layout_' + name.toLowerCase();
            self.dom = container._element.find(".lm_content");
            self.dom.attr('id', self.id);
            if(actions != null) this.context_menu = new BootstrapMenu('#' + self.id, { actions: actions });
            if(self.onLoaded != null) self.onLoaded();
            if(self.onResized != null) container.on('resize', function (e) { self.onResized(); });
        });
    }
}

class WorldPanel extends GorillaPanel
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

class WorkspacePanel extends GorillaPanel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Workspace",
        [
            {
                name: 'Create Directory',
                onClick: function () { Gorilla.FileManager.createDirectory("test"); }
            },
             {
                name: 'Show In Folder',
                onClick: function () { Gorilla.FileManager.openDirectory("."); }
            },
            {
                name: 'Rename',
                onClick: function () { window.console.log("Rename"); }
            },
            {
                name: 'Delete',
                onClick: function () { window.console.log("Delete"); }
            },
        ]);

        this.onChanged = function(string)
        {
            //window.console.log("workspace.onChanged");
        }
    }
}

class ViewportPanel extends GorillaPanel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Viewport", null);
        
        var self = this;
        this.onLoaded = function()
        {
            self.dom.addClass("gorilla_viewport");
        }
    }
}

class GorillaInput
{
    // dom
    // isValid
    // onSuccess
    // getError
    constructor(configuration)
    {
        var input = configuration.dom.find("input"); 
        var icon = configuration.dom.find("span");
        function update()
        {
            configuration.dom.removeClass("has-error has-success");
            icon.removeClass("glyphicon-ok glyphicon-remove");
            configuration.dom.tooltip('destroy')

            // update color & icon
            if(configuration.isValid(input.val()))
            {
                configuration.dom.addClass("has-success");
                icon.addClass("glyphicon-ok");
            }
            else
            {
                configuration.dom.addClass("has-error");
                configuration.dom.tooltip({title : configuration.getError()});
                icon.addClass("glyphicon-remove");
            }
        }
        input.on("change paste keyup", update);
        input.focusout(function() 
        { 
            var value = input.val();
            if(!configuration.isValid(value))
            {
                input.select();
                return;
            }
            configuration.onSuccess(value);
        });
        update();
        input.select();
    }
}

class PropertyPanel extends GorillaPanel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Property", null);
        var self = this;

        this.module = {};
        this.onModuleChanged = function(string)
        {
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
                        var html = Mustache.render(Template.property.component, data);
                        self.dom.append(html);

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
                                var html = Mustache.render(Template.property.component, data);
                                self.dom.append(html);
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
            this.descriptors = JSON.parse(string);
            for(var name in this.descriptors)
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
            
            this.context_menu_data = 
            { 
                actions: actions, 
                actionsGroups: 
                [
                    ['createScript'],
                    group
                ] 
            };
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
                        html += Mustache.render(Template.property.component, data);
                    }
                }
            }
            this.dom.html(html);
        }
    }
}

class ConsolePanel extends GorillaPanel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Console",
        [
            {
                name: 'Clear',
                onClick: function () 
                { 
                    Editor.panels.console.dom.html(""); 
                    Editor.panels.console.logs = {};
                }
            },
        ]);

        var self = this;

        this.logs = {};
        function logInternal(template, ...args)
        {
            // Collapse logs
            var message = args.join(" ");
            var hash = md5(message);
            var info = self.logs[hash];
            if(info == null)
            {
                // add new info instance
                self.logs[hash] =
                {
                    count : 1,
                    index: self.dom.children().size()
                }

                // add html to console
                var html = Mustache.render(template, {message:message});
                self.dom.append(html);
                self.dom.scrollTop(self.dom.height());
            }
            else
            {
                ++info.count;
                self.dom.find("span:eq("+info.index+")").html(info.count);
            }
        }
        this.onPrint = function(...args) { logInternal(Template.console.print, args); }
        this.onWarning = function(...args) { logInternal(Template.console.warning, args); }
        this.onError = function(...args) { logInternal(Template.console.error, args); }
    }
}

class ProfilerPanel extends GorillaPanel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Profiler",
        [
            {
                name: 'Activate',
                onClick: function () 
                { 
                    Gorilla.Profiler.activate();
                }
            },
            {
                name: 'Deactivate',
                onClick: function () 
                { 
                    Gorilla.Profiler.deactivate();
                }
            },
            {
                name: 'Clear',
                onClick: function () 
                { 
                    Editor.panels.profiler.dom.html(""); 
                    Editor.panels.profiler.logs = {};
                }
            },
        ]);

        var self = this;
        this.frame = 0;

        function addChartValue(chart, value, label, color)
        {
            window.console.log(color);
            if(color == null) color = 'rgba(255, 255, 255, 1.0f)';
            window.console.log(color);

            // Limit
            var dataset = chart.data.datasets[0];
            if(dataset.data.length > chart.options.scales.xAxes[0].ticks.maxTicksLimit)
            {
                chart.data.labels.shift();
                dataset.data.shift();
                dataset.backgroundColor.shift();
            }

            // Add new value
            chart.data.labels.push(label);
            dataset.data.push(value);
            dataset.backgroundColor.push(color);
   
            chart.update();
        }

        this.onLoaded = function()
        {
            var html = Mustache.render(Template.menu.vitem, 
            {
                items:
                [
                    { name: "Frame Rate", active: "active" },
                    { name: "Frame Time" },
                    { name: "Scoped Time" },
                    { name: "Memory" },
                ]
            });
            self.dom.append(html);

		    var canvas = document.createElement('canvas');
		    var ctx = canvas.getContext('2d');
            self.dom.append(canvas);

            self.memory = new Chart(ctx, 
            {
			    type: 'doughnut',
			    data: 
                {
				    datasets: 
                    [
                        {
                            data : [],
                            backgroundColor: [],
                            fill: false,
				        }
                    ],
                    labels: [],
                       
			    },
			    options: 
                {
				    responsive: true,
                    maintainAspectRatio: false,
                    legend: 
                    {
    	                display: true
                    },
                    scales: 
                    {
                        xAxes: 
                        [
                            {
                                display : false,
                            }
                        ],
                        yAxes: 
                        [
                            {
                                display : false,
                            }
                        ]
                    },
                    tooltips :
                    {
                        enabled : true   
                    }
			    }
		    })
            var domains = 
            [
                {label: "Animation", value: 30, color: "rgba(255,0,0,1.0)"},
                {label: "Physics", value: 10},
                {label: "User Interface", value: 10},
                {label: "Textures", value: 50},
            ];
            for(var index in domains)
            {
                var domain = domains[index];
                addChartValue(self.memory, domain.value, domain.label, domain.color);
            }

		    /*self.charts = 
            {
                framerate : new Chart(ctx, 
                {
			        type: 'bar',
			        data: 
                    {
				        datasets: 
                        [
                            {
                                data : [],
                                backgroundColor: [],
                                fill: false,
				            }
                        ],
                        labels: [],
                       
			        },
			        options: 
                    {
				        responsive: true,
                        maintainAspectRatio: false,
                        legend: 
                        {
    	                    display: false
                        },
                        scales: 
                        {
                            xAxes: 
                            [
                                {
                                    barPercentage: 1.0,
                                    categoryPercentage: 1.0,
                                    ticks: 
                                    {
                                        maxTicksLimit: 59,
                                        autoSkip: false,
					                    beginAtZero: false,
                                        callback: function(value, index, values) 
                                        {     
                                            var date = new Date((self.frame + index) * 1000);
                                            var minute = date.getMinutes().toString();
                                            if(minute.length == 1) minute = "0" + minute;
                                            var second = date.getSeconds().toString();
                                            if(second.length == 1) second = "0" + second;

                                            return minute + ":" + second;
                                        }
                                    } 
                                }
                            ],
                            yAxes: 
                            [
                                {
                                    ticks: 
                                    {
                                        stepSize : 5,
                                        min : 0,
                                        max : 80,
					                    beginAtZero: true
                                    }
                                }
                            ]
                        },
                        tooltips :
                        {
                            enabled : false   
                        }
			        }
		        })
            }*/
        }

        var colors = ["rgba(75, 192, 192, 0.2)", "rgba(255, 159, 64, 0.2)", "rgba(255, 99, 132, 0.2)"];
        this.onChanged = function(string)
        {
            //var chart = self.charts.framerate;
            //var profile = JSON.parse(string); 
            //var fps = profile.fps;

            //var color = null;
            //if(fps>28) color = colors[0];
            //else if(fps>20) color = colors[1];
            //else color = colors[2];
                
            //// Limit charts
            //var values =  chart.data.datasets[0];
            //if(values.data.length > chart.options.scales.xAxes[0].ticks.maxTicksLimit)
            //{
            //    chart.data.labels.shift();
            //    values.data.shift();
            //    values.backgroundColor.shift();
            //    ++self.frame;
            //}

            //// Add new value
            //chart.data.labels.push(0);
            //values.data.push(fps);
            //values.backgroundColor.push(color);
   
            //chart.update();
        }
    }
}

/*!
 * Gorilla Editor
 * Copyright 2013-2017 Yoann Laala
 */
class GorillaEditor
{
    constructor()
    {
        var self = this;
        this.selectProject = function(create, optional)
        {
            var input_new = Mustache.render(Template.form.input, {button_type : "folder", button_default_path : Gorilla.File.getPath(Gorilla.EPath.Document), button_text : "Browse", place_holder : "Directory path" });
            var input_open = Mustache.render(Template.form.input, {button_type : "file", button_accept : ".gorilla", button_default_path : "D:\\", button_text : "Browse", place_holder : "Project file" });
            $("body").append(Mustache.render(Template.modal.project, {is_optional: optional, input_new : input_new, input_open : input_open}));   
            var modal = $("#myModal");
            modal.modal({backdrop: 'static'});
            modal.find("button[type='submit']").click(function()
            {
                var result = false;
                var section = modal.find("li.active").find("a").attr("href");
                if (section == "#create_project")
                {
                    var name = $(section).find("input:eq(0)").val();
                    var path = $(section).find("input:eq(1)").val();
                    result = Gorilla.Project.create(name, path);
                }
                else
                {
                    var path = $(section).find("input:eq(0)").val();
                    result = Gorilla.Project.open(path);
                }
                
                // Remove modal only if operation succeeded
               // if(result)
                {
                    modal.remove();
                    $(".modal-backdrop").remove();
                }
            });
        }

        this.buildProject = function()
        {
            var input_new = Mustache.render(Template.form.input, {button_type : "folder", button_default_path : Gorilla.File.getPath(Gorilla.EPath.Document), button_text : "Browse", place_holder : "Directory path" });
            $("body").append(Mustache.render(Template.modal.build, {input_new : input_new}));   
            var modal = $("#myModal");
            modal.modal({backdrop: 'static'});

            modal.find("input").change(function()
            {
                var input = $(this);
                input.val(input.val() + Gorilla.Project.getName() + "\\");
            });

            modal.find("button[type='submit']").click(function()
            {
                var input = modal.find("input:eq(0)");
                Gorilla.Project.build(input.val());

                modal.remove();
                $(".modal-backdrop").remove();
            });
        }

        // Render menu
        var menu = Mustache.render(Template.menu.bar, 
        {
            items_level0 : 
            [
                {
                    name: "FILE",
                    items_level1:
                    [
                        {
                            name: "New World",
                            action: function() { return function() { Gorilla.World.create(); } }
                        },
                        {
                            name: "Open World",
                            action: function() { return function() { Gorilla.World.open(); }}
                        },
                        {
                            name: "Save World",
                            action: function() { return function() { Gorilla.World.save(false); }}
                        },
                        {
                            name: "Save World as...",
                            action: function() { return function() { Gorilla.World.save(true); }},
                            separator : true
                        },
                        {
                            name: "New Project",
                            action: function() { return function() { Editor.selectProject(true, true); } }
                        },
                        {
                            name: "Open Project",
                            action: function() { return function() { Editor.selectProject(false, true); } }
                        },
                        {
                            name: "Save Project",
                            action: function() { return function() { Gorilla.Project.save(); } },
                            separator : true
                        },
                        {
                            name: "Build",
                            action: function() { return function() { Editor.buildProject(); } },
                            separator : true
                        },
                        {
                            name: "Exit",
                            action: function() { return function() { Gorilla.exit(); } }
                        }
                    ]
                },
                {
                    name: "ABOUT",
                    items_level1:
                    [
                        {
                            name: "Help"
                        },
                        {
                            name: "Version",
                        }
                    ]
                }
            ]
        }, {template : Template.menu.item});
        $("body").append(menu)

        this.onNotification = function(string)
        {
            var notification = JSON.parse(string);
            var notify = $.notify(
            {
	            message: notification.message,
            },
            {
	            type: notification.type
            });

            /* notify.update(
            {
                icon: 'glyphicon glyphicon-remove',
	            message: 'Cooking ' + value + ' Failed!',
            });

            notify.update(
            {
                icon: 'glyphicon glyphicon-ok',
	            message: 'Cooking ' + value + ' Succeed!',
            });*/
        }

        // Prepare notifycation system
         $.notifyDefaults(
         {
             placement : 
             {
                 from : "bottom",    
                 align : "right",
             },
             showProgressbar : true,
             offset :
             {
                 x : 10,
                 y : 10,
             },
             space : 5,
         });

        // Generate Layout
        var config = 
        {
            settings:
            {
                hasHeaders: true,
                constrainDragToContainer: true,
                reorderEnabled: true,
                selectionEnabled: false,
                popoutWholeStack: false,
                blockedPopoutsThrowError: true,
                closePopoutsOnUnload: true,
                showPopoutIcon: true,
                showMaximiseIcon: false,
                showCloseIcon: true
            },
            content: 
            [
                {
                    type: 'row',
                    content: 
                    [
                        {
                            type: 'stack',
                            width: 12.5,
                            content: 
                            [
                                {
                                    type: 'component',
                                    componentName: 'World',
                                    title: 'World',
                                },
                                {
                                    type: 'component',
                                    componentName: 'Workspace',
                                    title: 'Workspace',
                                }
                            ]
                        }, 
                        {
                            type: 'column',
                            content: 
                            [
                                {
                                    type: 'stack',
                                    content: 
                                    [
                                        {
                                            type: 'component',
                                            componentName: 'Viewport',
                                            title: 'Viewport 1',
                                        }
                                    ]
                                },
                                {
                                    type: 'stack',
                                    content: 
                                    [
                                         {
                                            type: 'component',
                                            componentName: 'Console',
                                            height : 30.0,
                                            title: 'Console',
                                        },
                                        {
                                            type: 'component',
                                            componentName: 'Profiler',
                                            height : 30.0,
                                            title: 'Profiler', 
                                        }
                                    ]
                                }
                               
                            ]
                        },
                        {
                            type: 'stack',
                            width: 12.5,
                            content: 
                            [
                                {
                                    type: 'component',
                                    componentName: 'Property',
                                    title: 'Property',
                                }
                            ]
                        }
                    ]
                }
            ]
        };

        // Panels
        var myLayout = new GoldenLayout( config );
        this.panels = 
        {
            world : new WorldPanel(myLayout),
            workspace : new WorkspacePanel(myLayout),
            viewport : new ViewportPanel(myLayout),
            property : new PropertyPanel(myLayout),
            console : new ConsolePanel(myLayout),
            profiler : new ProfilerPanel(myLayout),
        }
        myLayout.init();

        // Display world
	    var viewport_dom = this.panels.viewport.dom;     
	    Gorilla.createViewport(viewport_dom.offset().left, viewport_dom.offset().top, viewport_dom.width(), viewport_dom.height());
    }
}

/*!
 * Gorilla Template
 * Copyright 2013-2017 Yoann Laala
 */
class GorillaTemplate
{
    constructor()
    {
        this.modal = 
        {
            project : "                                                                                       \
                <div class='modal fade' id='myModal' role='dialog'>                                                   \
                    <div class='modal-dialog'>                                                                              \
                        <div class='modal-content'>                                                                           \
                            <div class='modal-header'>                                             \
                                {{#is_optional}}\
                                <button type='button' class='close' data-dismiss='modal' aria-label='Close'><span aria-hidden='true'>&times;</span></button> \
                                {{/is_optional}}\
                                <h4>Define your project</h4>                                     \
                            </div>                                                                                                \
							<ul class='nav nav-tabs'>                                                           \
								<li class='active'><a data-toggle='tab' href='#create_project'><span class='glyphicon glyphicon-file'></span> Create a project</a></li>                                    \
								<li><a data-toggle='tab' href='#open_project'><span class='glyphicon glyphicon-folder-open'></span> Open an existing project</a></li>                                                 \
							</ul>                                                                              \
                            <div class='modal-body' style='padding:40px 50px;'>                                               \
								<div class='tab-content'>                                                                     \
									<div id='create_project' class='tab-pane fade in active'>                                           \
										<div class='form-group'>                                                                        \
											<label><span class='glyphicon glyphicon-tag'></span> Name</label>        \
											<input type='text' class='form-control' placeholder='Enter project name'>         \
										</div>                                                                                            \
										<br/>\
										<div class='form-group'>                                                                        \
											<label><span class='glyphicon glyphicon-hdd'></span> Output Directory</label>                      \
											{{{input_new}}}                                                                                   \
										</div>                                                                                            \
									</div>                                                                                    \
									<div id='open_project' class='tab-pane fade'>                                                    \
										<div class='form-group'>                                                                        \
											<label for='psw'><span class='glyphicon glyphicon-open-file'></span> Project file</label>        \
											{{{input_open}}}                                                                                   \
										</div>                                                                                            \
									</div>                                                                                    \
								</div>                                                                                        \
                            </div>                                                                                                \
                            <div class='modal-footer'>                                                                          \
                                <button type='submit' class='btn btn-default btn-small'>Continue</button>      \
                            </div>                                                                                                \
                        </div>                                                                                                  \
                    </div>                                                                                                    \
                </div>",                                                                                                      

            build : "                                                                                       \
                <div class='modal fade' id='myModal' role='dialog'>                                                   \
                    <div class='modal-dialog'>                                                                              \
                        <div class='modal-content'>                                                                           \
                            <div class='modal-header'>                                             \
                                <button type='button' class='close' data-dismiss='modal' aria-label='Close'><span aria-hidden='true'>&times;</span></button> \
                                <h4>Build your project</h4>                                     \
                            </div>                                                                                                \
                            <div class='modal-body''>                                               \
							    <div class='form-group'>                                                                        \
								    <label><span class='glyphicon glyphicon-hdd'></span> Directory</label>                      \
								    {{{input_new}}}                                                                                   \
							    </div>                                                                                            \
						    </div>                                                                                    \
                            <div class='modal-footer'>                                                                          \
                                <button type='submit' class='btn btn-default btn-small'>Build</button>      \
                            </div>                                                                                                \
                        </div>                                                                                                  \
                    </div>                                                                                                    \
                </div>"                                                                                                      
        }

        this.console = 
        {
            print : "<div class='alert'><span class='label label-info'>1</span>{{message}}</div>",
            warning : "<div class='alert alert-warning'><span class='label label-warning'>1</span>{{message}}</div>",
            error : "<div class='alert alert-danger'><span class='label label-danger'>1</span>{{message}}</div>",
        };

        this.property =
        {
            component : "                                                                                   \
                <div class='panel-group gorilla_component' component='{{class}}'>                           \
                    <div class='panel panel-default'>                                                       \
                        <div class='panel-heading'>  \
                            <h4 class='panel-title'>                                                        \
                                {{#create}}                                                                 \
                                <div class='form-group has-feedback'>                                       \
                                    <input type='text' class='form-control' placeholder='Script Name' value='{{name}}' maxLength='32' required='true'>       \
                                    <span class='glyphicon form-control-feedback'></span>\
                                </div>                                                                  \
                                {{/create}}                                                                 \
                                {{#display}}                                                                 \
                                <a data-toggle='collapse' href='#{{name}}'>{{name}}</a>                    \
                                {{/display}}                                                                 \
                            </h4>                                                                           \
                        </div>                                                                              \
                        <div id='{{name}}' class='panel-collapse collapse'>                                    \
                            <ul class='list-group'>                                                         \
                                {{#fields}}                                                                 \
                                <li class='list-group-item'>                                                \
                                    <div class='row'>                                                       \
                                        <div class='col-sm-6'>{{name}}</div>                                \
                                        <div class='col-sm-6 form-group'>                                   \
                                            {{#Bool}}                                                       \
                                            <input type='checkbox'>                                         \
                                            {{/Bool}}                                                       \
                                            {{#Gorilla::Math::Vector3}}                               \
                                            {{#value}} \
				                            <input type='number' class='gorilla_asset' gorilla_type='9' offset='{{offset}}' min='1' max='5' value='{{.}}'>                            \
                                            {{/value}}  \
                                            {{/Gorilla::Math::Vector3}}                               \
                                            {{#Gorilla::Engine::AssetHandle}}                               \
                                            <button type='file' default_path='{{default_path}}' accept='{{extension}}' class='btn btn-default btn-small'>...</button>                                                           \
				                            <input type='text' gorilla_type='12' gorilla_asset='{{type}}' class='form-control gorilla_asset' offset='{{offset}}' placeholder='Asset path' value='{{value.filepath}}'>                                                                 \
                                            {{/Gorilla::Engine::AssetHandle}}                               \
                                        </div>                                                              \
                                    </div>                                                                  \
                                </li>                                                                       \
                                {{/fields}}                                                                 \
                            </ul>                                                                           \
                        <div class='panel-footer'></div>                                                    \
                    </div>                                                                                  \
            </div>"
        }

        this.menu = 
        {
            bar : "                                                                                                                                            \
                <nav class='navbar navbar-default'>                                                                                                            \
                    <div class='container-fluid'>                                                                                                              \
                        <div class='collapse navbar-collapse' id='bs-example-navbar-collapse-1'>\
                            {{>template}}\
                            <span class='gorilla_bar glyphicon glyphicon-play' onclick='Gorilla.play();'/>    \
                            <span class='gorilla_bar glyphicon glyphicon-pause' onclick='Gorilla.pause();'/>    \
                            <span class='gorilla_bar glyphicon glyphicon-stop' onclick='Gorilla.stop();'/>    \
                        </div>                                            \
                    </div>                                                                                                                                     \
                </nav>",

            item : "                                                                                                                                           \
                <ul class='nav navbar-nav'>                                                                                                                    \
                    {{#items_level0}}                                                                                                                          \
                    <li class='dropdown'>                                                                                                                      \
                        <a href='#' class='dropdown-toggle' data-toggle='dropdown' role='button' aria-haspopup='true' aria-expanded='false'>{{name}}</a>       \
                        <ul class='dropdown-menu'>                                                                                                             \
                            {{#items_level1}}                                                                                                                  \
                            <li onclick='execute_callback({{action}});'><a href='#'>{{name}}</a></li>                                                          \
                            {{#separator}}                                                                                                                     \
                            <li role='separator' class='divider'></li>                                                                                         \
                            {{/separator}}                                                                                                                     \
                            {{/items_level1}}                                                                                                                  \
                        </ul>                                                                                                                                  \
                    </li>                                                                                                                                      \
                    {{/items_level0}}                                                                                                                          \
                </ul>",

            vitem : "                                                                                                                                          \
                <nav class='navbar navbar-default'>                                                                                                            \
                    <div class='container-fluid'>                                                                                                              \
                        <ul class='nav navbar-nav'>                                                                                                            \
                            {{#items}}                                                                                                                         \
                            <li class='{{active}}'><a href='#'>{{name}}</a></li>                                                                               \
                            {{/items}}                                                                                                                         \
                        </ul>                                                                                                                                  \
                    </div>                                                                                                                                     \
                </nav>",
        }

        this.form = 
        {
            input : "                                                                                                                                            \
                <div class='form-group'>                                                                                                                        \
                        <button type='{{button_type}}' default_path='{{button_default_path}}' accept='{{button_accept}}' class='btn btn-default btn-small'>{{button_text}}</button>                                                           \
				        <input type='text' class='form-control' placeholder='{{place_holder}}'>                                                                 \
			    </div>",
        }
 
        function prepreTemplate(object)
        {
            for(var template in object) Mustache.parse(template);
        }
        for(var object in this) prepreTemplate(object);
    }
}

$(document).ready(function()
{		
    Template = new GorillaTemplate();
    Editor = new GorillaEditor();

    // Debug
    if(Gorilla.isDebug)
    {
        window.viewLoaded = function() { window.console.log("viewLoaded"); }
        Editor.panels.workspace.onChanged("[{\"id\":28734125536,\"text\":\"\",\"children\":[{\"id\":1210905551064,\"text\":\"\",\"children\":[{\"text\":\"hello_world.html\"}]},{\"id\":1210905551176,\"text\":\"\",\"children\":[{\"text\":\"Sphere.fbx\"},{\"text\":\"Torus.fbx\"}]},{\"id\":1210905551288,\"text\":\"\",\"children\":[{\"text\":\"Entry.world\"}]}]}]");
        Editor.panels.world.onChanged("[{\"id\":2549015595088,\"text\":\"Directional Light\",\"parent\":\"#\"},{\"id\":2549015596480,\"text\":\"Cube\",\"parent\":\"#\"},{\"id\":2549015600400,\"text\":\"Camera\",\"parent\":\"#\"},{\"id\":2549015602720,\"text\":\"GameObject\",\"parent\":\"#\"}]");
        Editor.panels.property.onModuleChanged("{\"Gorilla::Component::Camera\":{\"fields\":{\"Viewport\":{\"visible\":true,\"type\":\"Pointer\",\"offset\":24},\"m_pCamera\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":32},\"m_fFOV\":{\"visible\":false,\"type\":\"Float\",\"offset\":40},\"m_fZNear\":{\"visible\":false,\"type\":\"Float\",\"offset\":44},\"m_fZFar\":{\"visible\":false,\"type\":\"Float\",\"offset\":48},\"m_uiLayer\":{\"visible\":false,\"type\":\"UInt\",\"offset\":52},\"m_eProjection\":{\"visible\":false,\"type\":\"Enum\",\"offset\":56}},\"dependencies\":[\"Gorilla::Engine::Component\"]},\"Gorilla::Engine::Component\":{\"fields\":{\"m_pGameObject\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":8},\"m_eFlag\":{\"visible\":false,\"type\":\"UInt\",\"offset\":16},\"m_eState\":{\"visible\":false,\"type\":\"Enum\",\"offset\":20}},\"dependencies\":{}},\"Gorilla::Component::RenderableNode\":{\"fields\":{},\"dependencies\":[\"Gorilla::Engine::Component\",\"Gorilla::Renderer::IRenderable\"]},\"Gorilla::Renderer::IRenderable\":{\"fields\":{},\"dependencies\":{}},\"Gorilla::Component::Decal\":{\"fields\":{\"Asset\":{\"visible\":true,\"type\":\"Gorilla::Engine::AssetHandle\",\"offset\":32,\"element_type\":\"Gorilla::Engine::Material\"}},\"dependencies\":[\"Gorilla::Component::RenderableNode\"]},\"Gorilla::Component::EnvironmentMap\":{\"fields\":{\"Asset\":{\"visible\":true,\"type\":\"Gorilla::Engine::AssetHandle\",\"offset\":32,\"element_type\":\"Gorilla::Engine::CubeMap\"}},\"dependencies\":[\"Gorilla::Component::RenderableNode\"]},\"Gorilla::Component::Sprite\":{\"fields\":{\"Texture\":{\"visible\":true,\"type\":\"Pointer\",\"offset\":32}},\"dependencies\":[\"Gorilla::Component::RenderableNode\"]},\"Gorilla::Component::Renderable\":{\"fields\":{},\"dependencies\":[\"Gorilla::Engine::Component\",\"Gorilla::Renderer::IRenderable\"]},\"Gorilla::Component::WebView\":{\"fields\":{\"Document\":{\"visible\":true,\"type\":\"Gorilla::Engine::AssetHandle\",\"offset\":32,\"element_type\":\"Gorilla::Engine::WebDocument\"},\"RenderTexture\":{\"visible\":true,\"type\":\"Pointer\",\"offset\":40},\"Transparent\":{\"visible\":true,\"type\":\"Bool\",\"offset\":48},\"m_pInstance\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":53},\"m_pTexture\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":61}},\"dependencies\":[\"Gorilla::Component::Renderable\"]},\"Gorilla::Component::DirectionalLight\":{\"fields\":{\"m_pLight\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":32}},\"dependencies\":[\"Gorilla::Component::Renderable\"]},\"Gorilla::Component::PointLight\":{\"fields\":{\"m_pLight\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":32}},\"dependencies\":[\"Gorilla::Component::RenderableNode\"]},\"Gorilla::Component::Material\":{\"fields\":{\"m_vInstance\":{\"visible\":false,\"type\":\"Gorilla::Vector\",\"offset\":24,\"element_type\":\"MaterialInstance\"}},\"dependencies\":[\"Gorilla::Engine::Component\"]},\"Gorilla::Component::Mesh\":{\"fields\":{\"Asset\":{\"visible\":true,\"type\":\"Gorilla::Engine::AssetHandle\",\"offset\":32,\"element_type\":\"Gorilla::Engine::Mesh\"},\"Materials\":{\"visible\":true,\"type\":\"Gorilla::Vector\",\"offset\":40,\"element_type\":\"Gorilla::Engine::AssetHandle\"}},\"dependencies\":[\"Gorilla::Component::RenderableNode\"]},\"Gorilla::Component::Node\":{\"fields\":{\"Position\":{\"visible\":true,\"type\":\"Gorilla::Math::Vector3\",\"offset\":24,\"element_type\":\"\"},\"Rotation\":{\"visible\":true,\"type\":\"Gorilla::Math::Vector3\",\"offset\":36,\"element_type\":\"\"},\"Scale\":{\"visible\":true,\"type\":\"Gorilla::Math::Vector3\",\"offset\":48,\"element_type\":\"\"},\"m_vUp\":{\"visible\":false,\"type\":\"Gorilla::Math::Vector3\",\"offset\":60,\"element_type\":\"\"},\"m_vRight\":{\"visible\":false,\"type\":\"Gorilla::Math::Vector3\",\"offset\":72,\"element_type\":\"\"},\"m_vForward\":{\"visible\":false,\"type\":\"Gorilla::Math::Vector3\",\"offset\":84,\"element_type\":\"\"},\"m_pNode\":{\"visible\":false,\"type\":\"Pointer\",\"offset\":96}},\"dependencies\":[\"Gorilla::Engine::Component\"]},\"Gorilla::Component::CameraFree\":{\"fields\":{\"m_fRotationSpeed\":{\"visible\":false,\"type\":\"Float\",\"offset\":24},\"m_fMoveSpeed\":{\"visible\":false,\"type\":\"Float\",\"offset\":28}},\"dependencies\":[\"Gorilla::Engine::Component\"]}}");
        Editor.panels.property.onSelectionChanged("{\"name\":\"Cube\",\"components\":{\"Gorilla::Component::Material\":{},\"Gorilla::Component::Mesh\":{\"Asset\":{\"filepath\":\"@Mesh/Cube.fbx\",\"parameters\":\"\"},\"Materials\":{}},\"Gorilla::Component::Node\":{\"Position\":[0,0,0],\"Rotation\":[0,0,0],\"Scale\":[1,1,1]}}}");
        Editor.panels.profiler.onChanged("{\"structure\":{\"World\":{},\"Display\":{\"EditorPass\":{},\"GuiPass\":{},\"FullScreenPass\":{},\"VisibilityPass\":{},\"ShadowPass\":{},\"GeometryPass\":{},\"DecalPass\":{},\"LightPass\":{},\"PrimitivePass\":{},\"GuiPass\":{},\"CompositingPass\":{},\"FullScreenPass\":{}}},\"time\":[0.172839,1.31351,0.000310303,0,0,0.00341333,0,0.000310303,0,0,0,0,0,0.000310303]}");
    }
    viewLoaded();
});