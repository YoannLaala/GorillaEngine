class ViewportPanel extends Panel
{
    constructor(layoutManager)
    {
        super(layoutManager, "Viewport", null);
        
        var _self = this;
        var _viewport = -1;

        _self.onLoaded = function()
        {
            _self.dom.addClass("gorilla_viewport"); 
        }

        _self.onResized = function()
        {
            if(_viewport == -1) Gorilla.Viewport.create(_self.dom.offset().left, _self.dom.offset().top, _self.dom.width(), _self.dom.height(), function(viewport) { _viewport = viewport; });
            else Gorilla.Viewport.set(_viewport, _self.dom.offset().left, _self.dom.offset().top, _self.dom.width(), _self.dom.height());
        }
    }
}