 /* public domain
 * vim: set ts=4:
 */

RED.storage = (function() {

	const empty_project = {
		"signature": "QRND",
		"name": "Untitled",
		"path": null,
		"nodes": [
			{
				"type": "tab",
				"id": "node-graph",
				"label": "Untitled"
			}
		]
	};

	function update() {
		// TOOD: use setTimeout to limit the rate of changes?
		if (localStorage) {
			var nns = RED.nodes.createCompleteNodeSet();
			var data = {
				"signature": "QRND",
				"name": RED.storage.name,
				"path": RED.storage.filename,
				"nodes": RED.nodes.createCompleteNodeSet()
			}
			localStorage.setItem("qrnd_composer", JSON.stringify(data));
			//console.log("localStorage write:", nns);
		}
	}

	function load() {
		if (localStorage) {
			var data = localStorage.getItem("qrnd_composer") 
			if (data)
				data = JSON.parse(data);
			else 
				data = empty_project;
			console.log("localStorage read: ", data);
			RED.storage.filename = data.path;
			RED.storage.name = data.name;
			RED.nodes.import(data.nodes, false);
			RED.view.redraw();
			RED.history.clear();
			RED.view.addConsole();
		}
	}
	function clear() {
		// TOOD: use setTimeout to limit the rate of changes?
		if (localStorage) {
			localStorage.removeItem("qrnd_composer");
			//console.log("localStorage write");
		}
	}

	function new_file() {
		RED.view.clear()
		localStorage.setItem("qrnd_composer", JSON.stringify(empty_project));
		load();
	}

	function load_file(filename) {
		lib.vfs.readFile(filename).then(
			(json) => {
				let js=JSON.parse(json);
				if (js.signature!='QRND') {
					return;
				} 
				console.log(js.nodes)
				RED.view.clear()
				localStorage.setItem("qrnd_composer", JSON.stringify(js));
				load();
			}
		);
	}
	
	function save_file(filename, callback) {
		filename = filename || '';
		if (filename=='') {
			RED.filebrowser.save_dialog(RED.storage.filename, 
				                        (fn) => { RED.storage.save_file(fn, callback); });
			return;
		}
		let path = lib.path.parse(filename);

		if (path.ext!='.json') {
			if (path.ext!='') {
				path.name = path.name+path.ext
			}
			filename += '.json';
		}
	
		console.log("save_file", path.name, filename)

		RED.nodes.workspace("node-graph").label = path.name;
		var nns = RED.nodes.createCompleteNodeSet();
		console.log(nns)

		nns[0].label = path.name;
		$('#workspace-tabs a:first').html(path.name);
		$('#workspace-tabs a:first').attr("title",path.name);

		var content = {
			signature: 'QRND',
			name: path.name,
			path: filename,	
			nodes: nns
		};
		lib.vfs.writeFile(filename, JSON.stringify(content)).then( () => {
			RED.storage.filename = content.path;
			RED.storage.name = content.name;
			callback && callback()
		});
	}


/**
 * 	RED.enable_menu('file-save', false);

	var load_filename = "Untitled.json";

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
					load_file(load_filename);
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
	let load_ok_button = $(btns[0])


	$("#node-dialog-load-browser").simpleFileBrowser({
		url: "vfs/ls",
		method: 'GET',
		pattern: '*.json',
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
				load_file(load_filename);
			} else {
				load_ok_button.button("disable")
			}
		}
	});

	function load_dialog() {
		$( "#node-dialog-load" ).dialog("open");
		load_ok_button.button("disable");
	}


	var save_filename='';

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
					folder = folder=='/' ? '': folder;
					save_file(folder+'/'+file);
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
	let save_ok_button = $(btns[0])


	$("#node-dialog-save-browser").simpleFileBrowser({
		url: "vfs/ls",
		method: 'GET',
		pattern: '*.json',
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
				save_file(folder+'/'+file);
			}
		}
	});

	$("#node-dialog-save-newfolder").button();
	$("#node-dialog-save-delete").button();
	$("#node-dialog-save-newfolder").tooltip();
	$("#node-dialog-save-delete").tooltip();

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

*/

	return {
		update: update,
		load: load,
		clear: clear,

		new_file: new_file,
		load_file: load_file,
		save_file: save_file,
	}
})();
