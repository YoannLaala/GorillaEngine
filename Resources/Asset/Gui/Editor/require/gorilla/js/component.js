/*!
 * ComponentManager
 * Copyright 2013-2017 Yoann Laala
 */
class ComponentManager
{
    constructor()
    {
        this.add = function(gameObject, name) { window.console.log("Gorilla.Component.add(", gameObject, ",", name, ")"); }
        this.remove = function(gameObject, component) { window.console.log("Gorilla.Component.remove(", gameObject, ",", component, ")"); }
        this.set = function(component, offset, type, value) { window.console.log("Gorilla.Component.set(", component, ",", offset, ",", type, ",", value, ")"); }
    }
}