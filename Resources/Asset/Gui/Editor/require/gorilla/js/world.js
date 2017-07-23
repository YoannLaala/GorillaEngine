/*!
 * WorldManager
 * Copyright 2013-2017 Yoann Laala
 */
class WorldManager
{
    constructor()
    {
        this.create = function() {  window.console.log("GorillaEngine::World::create()"); } 
        this.open = function(path) {  window.console.log("GorillaEngine::World::open(", path, ")"); } 
        this.save = function() {  window.console.log("GorillaEngine::World::save()"); } 
        this.saveAs = function(path) {  window.console.log("GorillaEngine::World::open(", path, ")"); } 
    }
}