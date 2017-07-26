/*!
 * ViewportManager
 * Copyright 2013-2017 Yoann Laala
 */
class ViewportManager
{
    constructor()
    {
        this.create = function(left, top, width, height, callback) {  window.console.log("Gorilla.Viewport.create(", [left, top, width, height, callback].join(","), ")"); }   
        this.set = function(viewport, left, top, width, height) {  window.console.log("Gorilla.Viewport.set(", [viewport, left, top, width, height].join(","), ")"); } 
        this.delete = function(viewport) {  window.console.log("Gorilla.Viewport.delete(", viewport, ")"); } 
    }
}