

'use strict';
var lib;
lib = lib || {};

lib.vfs = (function() {

    let VFS_URL = "xvfs";

    function ajax(mth, data) {
        let pr = jQuery.Deferred()

        jQuery.ajax({
            url: VFS_URL + "/" + mth,
            method: "GET",
            dataType: "json",
            async: true,
            data: data,
            success: (data) => {
                if (data.status=='success')
                    pr.resolve(data.result);
                else 
                    pr.reject();
            },
            error: () => {
                pr.reject();
            }

        })

        return pr;
    }


    function ls(path, pattern) {
        return ajax("ls", { path: path, pattern: pattern || '*' });
    }

    function rm(path) {
        return ajax("rm", { path: path });
    }

    function mkdir(path) {
        return ajax("mkdir", { path: path });
    }

    function stat(path) {
        return ajax("stat", { path: path });
    }

    function readFile(path) {
        return ajax("readfile", { path: path });
    }

    function writeFile(path, content) {
        return ajax("writefile", { path: path, content: content });
    }

    return {
        ls: ls, rm: rm, mkdir: mkdir, stat: stat,
        readFile: readFile, writeFile: writeFile 
    }

})();