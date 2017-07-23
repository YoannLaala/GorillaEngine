function execute_callback(callback) { callback(); }

$(document).on('dblclick', '.gorilla_script a', function() { Gorilla.openScript(""); });

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
            var input_new = Gorilla.Template.render("form_input", {button_type : "folder", button_default_path : Gorilla.File.getPath(Gorilla.EPath.Document), button_text : "Browse", place_holder : "Directory path" });
            var input_open = Gorilla.Template.render("form_input", {button_type : "file", button_accept : ".gorilla", button_default_path : "D:\\", button_text : "Browse", place_holder : "Project file" });
			Gorilla.Template.show($("body"), "modal_project_open", {is_optional: optional, input_new : input_new, input_open : input_open}, false);
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
            var input_new = Gorilla.Template.render("form_input", {button_type : "folder", button_default_path : Gorilla.File.getPath(Gorilla.EPath.Document), button_text : "Browse", place_holder : "Directory path" });
			Gorilla.Template.show($("body"), "modal_project_build", {input_new : input_new}, false);  
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
        Gorilla.Template.show($("body"), "menu_bar", 
        [
            {
                name: "FILE",
                actions:
                [
                    {
                        name: "New World",
                        func: function() { return function() { Gorilla.World.create(); } }
                    },
                    {
                        name: "Open World",
                        func: function() { return function() { Gorilla.World.open(); }}
                    },
                    {
                        name: "Save World",
                        func: function() { return function() { Gorilla.World.save(false); }}
                    },
                    {
                        name: "Save World as...",
                        func: function() { return function() { Gorilla.World.save(true); }},
                        separator : true
                    },
                    {
                        name: "New Project",
                        func: function() { return function() { Editor.selectProject(true, true); } }
                    },
                    {
                        name: "Open Project",
                        func: function() { return function() { Editor.selectProject(false, true); } }
                    },
                    {
                        name: "Save Project",
                        func: function() { return function() { Gorilla.Project.save(); } },
                        separator : true
                    },
                    {
                        name: "Build",
                        func: function() { return function() { Editor.buildProject(); } },
                        separator : true
                    },
                    {
                        name: "Exit",
                        func: function() { return function() { Gorilla.exit(); } }
                    }
                ]
            },
            {
                name: "ABOUT",
                actions:
                [
                    {
                        name: "Help"
                    },
                    {
                        name: "Version",
                    }
                ]
            }
        ]);

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

$(document).ready(function()
{		
    Gorilla.Template.initialize();
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
        Editor.panels.console.onError("foo0");
        Editor.panels.console.onError("foo1");
        Editor.panels.console.onError("foo2");
        Editor.panels.console.onError("foo3");
        Editor.panels.console.onError("foo4");
        Editor.panels.console.onError("foo5");
        Editor.panels.console.onError("foo6");
        Editor.panels.console.onError("foo7");
        Editor.panels.console.onError("foo8");
        Editor.panels.console.onError("foo9");
    }
    viewLoaded();
});