/*!
 * ProjectManager
 * Copyright 2013-2017 Yoann Laala
 */
class ProjectManager
{
    constructor()
    {
        this.getName = function() { window.console.log("Gorilla.Project.getName()"); }

        this.create = function(path) { window.console.log("Gorilla.Project.create(",path,")"); }
        this.open = function(path) { window.console.log("Gorilla.Project.open(",path,")"); }
        this.save = function() { window.console.log("Gorilla.Project.save()"); }
        this.build = function() { window.console.log("Gorilla.Project.build()"); }
    }
}