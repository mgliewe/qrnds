
RED.filebrowser = (function() {

    const VFS_PATH = "vfs/ls";
    const PATTERN = "*.json"

    var load_filename = null;

    var load_callback = null;
    var save_callback = null;

    var load_ok_button;
    function init_load_dialog() {

        $( "#node-dialog-load" ).dialog({
            title: "Load Workspace",
            modal: true,
            autoOpen: false,
            width: 530,
            height: 400,
            buttons: [
                {
                    text: "Ok",
                    click: function() {
                        $( this ).dialog( "close" );
                        RED.storage.load_file(load_filename);
                    }
                },
                {
                    text: "Cancel",
                    click: function() {
                        $( this ).dialog( "close" );
                    }
                }
            ]
        });
    
        var w = $( "#node-dialog-load" ).dialog("widget");
        var btns = $(".ui-dialog-buttonset button", w)
        load_ok_button = $(btns[0])
    
        $("#node-dialog-load-browser").simpleFileBrowser({
            ls_func: lib.vfs.ls,
            pattern: PATTERN,
            path: '/',
            view: 'icon',
            select: true,
            breadcrumbs: true,
            onSelect: function (obj, file, folder, type) {
                if (type=='file') {
                    folder = folder=='/' ? '': folder;
                    load_filename = folder+'/'+file;
                    load_ok_button.button("enable")
                } else {
                    load_ok_button.button("disable")
                }
            },
            onOpen: function (obj,file, folder, type) {
                if (type=='file') {
                    $( "#node-dialog-load" ).dialog("close");
    
                    folder = folder=='/' ? '': folder;
                    load_filename = folder+'/'+file;
                    //$( this ).dialog( "close" );
                    RED.storage.load_file(load_filename);
                } else {
                    load_ok_button.button("disable")
                }
            }
        });
    }

    function load_dialog(callback) {
        load_filename = null;
        load_callback = callback;
        $( "#node-dialog-load" ).dialog("open");
        load_ok_button.button("disable");
        $("#node-dialog-load-browser").simpleFileBrowser("redraw")
    }


    var save_ok_button;

    function init_save_dialog() {

        $( "#node-dialog-save" ).dialog({
            title: "Save Workspace",
            modal: true,
            autoOpen: false,
            width: 530,
            height: 400,
            buttons: [
                {
                    text: "Ok",
                    click: function() {
                        $( this ).dialog( "close" );
                        var folder = $("#node-dialog-save-browser").simpleFileBrowser("getPath");
                        var file = $('#node-dialog-save-filename').val();
                        folder = folder=='/' ? folder : folder+'/';
                        save_callback && save_callback(folder + file);
                    }
                },
                {
                    text: "Cancel",
                    click: function() {
                        $( this ).dialog( "close" );
                    }
                }
            ]
        });

        var w = $( "#node-dialog-save" ).dialog("widget");
        var btns = $(".ui-dialog-buttonset button", w)
        save_ok_button = $(btns[0])

        $("#node-dialog-save-browser").simpleFileBrowser({
            ls_func: lib.vfs.ls,
            pattern: PATTERN,
            path: '/',
            view: 'icon',
            select: true,
            breadcrumbs: true,
            onSelect: function (obj, file, folder, type) {
                if (type!='folder') {
                    $('#node-dialog-save-filename').val(file);
                    save_ok_button.button("enable");
                }
            },
            onOpen: function (obj,file, folder, type) {
                if (type!='folder') {
                    folder = folder=='/' ? '': folder;
                    $( "#node-dialog-save" ).dialog("close")
                    save_callback(folder+'/'+file);
                }
            }
        });

        $("#node-dialog-save-newfolder")
            .button()
            .on("click", function() {

            });
        $("#node-dialog-save-delete")
            .button()
            .on("click", function() {

            })
            .button("disable");

        $('#node-dialog-save-filename').on("keyup", 
            function () {
                if ($(this).val()=='') {
                    save_ok_button.button("disable");
                } else {
                    save_ok_button.button("enable");
                }
            }
        );
        $('#node-dialog-save-filename').on("change", 
            function () {
                if ($(this).val()=='') {
                    save_ok_button.button("disable");
                } else {
                    save_ok_button.button("enable");
                }
            }
        );
    }

	function save_dialog(filename, callback) {
        filename = filename || "";
        let p = lib.path.parse(filename);

        save_callback = callback;
		$('#node-dialog-save-filename').val(p.name)
		$("#node-dialog-save-browser").simpleFileBrowser("setPath",p.dir)
		$( "#node-dialog-save" ).dialog("open");
		if ($('#node-dialog-save-filename').val()=='') {
			save_ok_button.button("disable");
		} else {
			save_ok_button.button("enable");
		}	
        $("#node-dialog-save-browser").simpleFileBrowser("redraw")
	}


    // initialize when document loaded
    $(() => {
        init_load_dialog();
        init_save_dialog();
    }); 

    return {

        load_dialog: load_dialog,
        save_dialog: save_dialog
    }

})();