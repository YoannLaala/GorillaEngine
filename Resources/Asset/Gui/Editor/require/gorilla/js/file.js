/*!
 * FileManager
 * Copyright 2013-2017 Yoann Laala
 */
class FileManager
{
    constructor()
    {
        this.open = function(path) {  window.console.log("Gorilla.File.open(", path, ")"); } 
        this.create = function(path) {  window.console.log("Gorilla.File.create(", path, ")"); } 
        this.rename = function(old_path, new_path) {  window.console.log("Gorilla.File.rename(", old_path, ",", new_path, ")"); } 
        this.delete = function(path) {  window.console.log("Gorilla.File.delete(", path, ")"); } 
        this.getDefaultFileName = function(name, extension) { window.console.log("Gorilla.File.getDefaultFileName(", name, ",", extension, ")"); } 
        this.getPath = function(type) { window.console.log("Gorilla.File.getPath(", type, ")"); } 
    }
}