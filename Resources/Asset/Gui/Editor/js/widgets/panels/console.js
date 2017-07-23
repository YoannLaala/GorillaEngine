class ConsolePanel extends Panel
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
        self.onLoaded = function()
        {
            self.dom.css("overflow-y", "auto");
        }

        this.logs = {};
        function logInternal(template, ...args)
        {
            // Collapse logs
            var message = args.join(" ");
            //var hash = md5(message);
            //var info = self.logs[hash];
            //if(info == null)
            {
                // add new info instance
                /*self.logs[hash] =
                {
                    count : 1,
                    index: self.dom.children().size()
                }*/

                // add html to console
                Gorilla.Template.show(self.dom, template, {message:message}, false);
                self.dom.scrollTop(self.dom.height());
            }
            /*else
            {
                ++info.count;
                self.dom.find("span:eq("+info.index+")").html(info.count);
            }*/
        }
        this.onPrint = function(...args) { logInternal("console_print", args); }
        this.onWarning = function(...args) { logInternal("console_warning", args); }
        this.onError = function(...args) { logInternal("console_error", args); }
    }
}