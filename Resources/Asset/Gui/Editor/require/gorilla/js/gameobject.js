/*!
 * GameObjectManager
 * Copyright 2013-2017 Yoann Laala
 */
class GameObjectManager
{
    constructor()
    {
        this.create = function(parentId) { window.console.log("Gorilla.GameObject.create(", parentId, ")"); }
        this.destroy = function(id) { window.console.log("Gorilla.GameObject.destroy(", id, ")"); }
        this.rename = function(id, name) { window.console.log("Gorilla.GameObject.rename(", id, ",", name, ")"); }
        this.select = function(id) { window.console.log("Gorilla.GameObject.select(", id, ")"); }
    }
}