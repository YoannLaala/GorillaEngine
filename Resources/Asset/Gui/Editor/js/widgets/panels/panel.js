class Panel
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