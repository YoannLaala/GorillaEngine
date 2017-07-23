class Input
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