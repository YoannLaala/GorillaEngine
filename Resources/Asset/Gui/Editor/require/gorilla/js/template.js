/*!
 * TemplateManager
 * Copyright 2013-2017 Yoann Laala
 */
class TemplateManager
{
    constructor()
    {
        this.files = {}
	
	    // parse all templates
	    this.initialize = function()
	    {
		    for(var file in this.files) Mustache.parse(file);
	    };
	
        // render a template
	    this.render = function(name, data)
	    {
		    var template = this.files[name];
		    if(template != null) return Mustache.render(template, data); 

            window.console.log("Template '", name, "' not found!");
		    return "[TEMPLATE '" + name + "' NOT FOUND]";
	    }

	    // show a template on specific dom
	    this.show = function(dom, name, data, clear)
	    {
            if(clear == null) clear = true;

            var html = this.render(name, data);
            if(clear) dom.html(html);
            else dom.append(html);
	    }
    }
}