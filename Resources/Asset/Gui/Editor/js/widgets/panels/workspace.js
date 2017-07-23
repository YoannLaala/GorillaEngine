class WorkspacePanel extends Panel
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