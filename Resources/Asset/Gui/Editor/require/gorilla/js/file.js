/*!
 * FileManager
 * Copyright 2013-2017 Yoann Laala
 */
class FileManager
{
    constructor()
    {
        this.openDirectory = function(path) {  window.console.log("Gorilla.File.openDirectory(", path, ")"); } 
        this.createDirectory = function(path) {  window.console.log("Gorilla.File.createDirectory(", path, ")"); } 
        this.rename = function(old_path, new_path) {  window.console.log("Gorilla.File.rename(", old_path, ",", new_path, ")"); } 
        this.getDefaultFileName = function(name, extension) { window.console.log("Gorilla.File.getDefaultFileName(", name, ",", extension, ")"); } 
        this.getPath = function(type) { window.console.log("Gorilla.File.getPath(", type, ")"); } 
    }
}