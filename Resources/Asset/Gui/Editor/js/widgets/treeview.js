class TreeView
{
    constructor(dom)
    {
    // public
        this.selection = null;

    // private
        var _self = this;
        var _dom = dom;
        var _data = {};
        var _template_input = "<input type='text' value='{{text}}' style='height:17px'/>";
        var _template = "                                                                           \
        <ul class='gorilla_tree'>                                                                   \
            {{#.}}                                                                                  \
            <li node_id='{{id}}'>                                                                   \
                <div class='gorilla_tree_header'>                                                   \
                    {{#childs.length}}                                                              \
                    <span class='glyphicon glyphicon-triangle-right gorilla_tree_cursor'/>          \
                    {{/childs.length}}                                                              \
                    {{^childs.length}}                                                              \
                    <span class='glyphicon glyphicon-triangle-right' style='visibility:hidden'/>    \
                    {{/childs.length}}                                                              \
                    <span>{{name}}</span>                                                           \
                </div>                                                                              \
            </li>                                                                                   \
            {{/.}}                                                                                  \
        </ul>";
        Mustache.parse(_template_input);
        Mustache.parse(_template);

        // initialize internal data
        function _setdata(childs)
        {
            for(var index in childs)
            {
                var node = childs[index];
                _data[node.id] = node;
                if(node.childs != null) _setdata(node.childs);
            }
        }

        // get node from dom
        function _getNodeFromDom(dom)
        {
            var item = dom.closest("li");
            var node_id = item.attr("node_id");
            var node = _data[node_id];
            if(node.states == null) node.states = {};
            return node;
        }

        // create display for a sub tree
        function _createTreeLevel(dom, items)
        {
            dom.append(Mustache.render(_template, items)); 
        }

    // public
        // set data
        this.set = function(items)
        {
            _setdata(items);
            dom.html(Mustache.render(_template, items));
        }

        // select a node
        this.select = function(node)
        {
            if(node != _self.selection)
            {
                // remove old one
                if(_self.selection != null)
                {
                    var header_old = $(".gorilla_tree li[node_id=\"" + _self.selection.id + "\"] .gorilla_tree_header:first");
                    header_old.removeClass("gorilla_tree_selected");
                }

                // select new one
                if(node)
                {
                    var header_new = $(".gorilla_tree li[node_id=\"" + node.id + "\"] .gorilla_tree_header:first"); 
                    header_new.addClass("gorilla_tree_selected");
                }
                _self.selection = node;
            }
        }

        this.edit = function(node, callback)
        {
            // keep old value
            var span = $(".gorilla_tree li[node_id=\"" + node.id + "\"] .gorilla_tree_header:first>span:eq(1)");
            var text = span.text();

            // generate input 
            span.html(Mustache.render(_template_input, {text:text}));
            var input = span.find("input");
            input[0].setSelectionRange(0, text.lastIndexOf("."));
            input.focus();

            // function to render the result
            function finalize() 
            {
                node.name = input.val();
                span.html(node.name);
                if(text != node.name) callback();
            }

            // wait & propagate modification
            input.focusout(finalize);
            input.keyup(function(e) 
            {
              if (e.keyCode === 13 || e.keyCode === 27) finalize();
            });
        }

    // events
        // unselect event
        $(document).on("click", function(event)
        {
            var isTreeClicked = $(event.target).closest("ul.gorilla_tree").length > 0;
            if(!isTreeClicked) _self.select(null);
        });

        // select event
        $(document).on("click", ".gorilla_tree .gorilla_tree_header", function()
        {
            var header = $(this);
            var node = _getNodeFromDom(header);
            _self.select(node);
        });

        // select on context menu
        _dom.on("contextmenu", function(event)
        {
            var target = $(event.target);
            var isItemSelected = target.closest(".gorilla_tree_header").length > 0;
            if(isItemSelected) _self.select(_getNodeFromDom(target));
            else _self.select(null);
        });

        // expand/collapse event
        $(document).on("click", ".gorilla_tree .gorilla_tree_cursor", function()
        {
            var icon = $(this);
            var item = icon.closest("li");
            var dom = item.find("ul:first");
            var node = _getNodeFromDom(item);
            
            if(node.states.expanded)
            {
                icon.removeClass("glyphicon-triangle-bottom").addClass("glyphicon-triangle-right");
                dom.css("display", "none");
            }
            else
            {
                icon.removeClass("glyphicon-triangle-right").addClass("glyphicon-triangle-bottom");
                if(dom.is("ul")) dom.css("display", "block");
                else _createTreeLevel(item, node.childs);
            }
            node.states.expanded = !node.states.expanded;
        });
    }
}