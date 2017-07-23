/*!
 * ProfilerManager
 * Copyright 2013-2017 Yoann Laala
 */
class ProfilerManager
{
    constructor()
    {
        this.activate = function() { window.console.log("Gorilla.Profiler.activate()"); }
        this.deactivate = function() { window.console.log("Gorilla.Profiler.deactivate()"); }
    }
}