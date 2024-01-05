/** Modified from original Node-Red source, for audio system visualization
 * vim: set ts=4:
 * Copyright 2013 IBM Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/
var RED = (function() {


	function hideDropTarget() {
		$("#dropTarget").hide();
		RED.keyboard.remove(/* ESCAPE */ 27);
	}

	$('#chart').on("dragenter",function(event) {
		if ($.inArray("text/plain",event.originalEvent.dataTransfer.types) != -1) {
			$("#dropTarget").css({display:'table'});
			RED.keyboard.add(/* ESCAPE */ 27,hideDropTarget);
		}
	});

	$('#dropTarget').on("dragover",function(event) {
		if ($.inArray("text/plain",event.originalEvent.dataTransfer.types) != -1) {
			event.preventDefault();
		}
	})
	.on("dragleave",function(event) {
		hideDropTarget();
	})
	.on("drop",function(event) {
		var data = event.originalEvent.dataTransfer.getData("text/plain");
		hideDropTarget();
		RED.view.importNodes(data);
		event.preventDefault();
	});

	// Menubur

	var position = {my: "left top", at: "left bottom"};

	$("#menubar").menu({
		position: position,
		blur: function() { 
			$(this).menu("option", "position", position);
		},
		focus: function(ev, ui) {
			if ($("#menubar").get(0) !== $(ui).get(0).item.parent().get(0)) {
				$(this).menu("option", "position", {my: "left top", at: "right top"});
			}
		}
	});
	$("#menubar").on("menuselect", function(ev, ui) {
		let item = $(ui.item).attr("data-menu");
		console.log(item)
		switch(item) {
			case 'file-new':
				break;
			case 'file-load':
				RED.filebrowser.load_dialog(RED.storage.load_file)
				break;
			case 'file-save':
				RED.storage.save_file(RED.storage.filename)
				break;
			case 'file-save-as':
				RED.filebrowser.save_dialog(RED.storage.filename, RED.storage.save_file)
				break;

			case 'edit-cut':
				RED.view.cutSelection();
				break;
			case 'edit-copy':
				RED.view.copySelection();
				break;
			case 'edit-paste':
				RED.view.paste();
				break;
			case 'edit-delete':
				RED.view.deleteSelection();
				break;
			case 'edit-select-all':
				RED.view.selectAll();
				break;
		
			case 'zoom-in':
				RED.view.zoomIn();
				console.log('zoom in')
				break;
			case 'zoom-out':
				RED.view.zoomOut();
				break;
			case 'zoom-zero':
				RED.view.zoomZero();
				break;

			case 'worksheet-new':
				RED.view.addNewWorkspace();
				break;
			case 'worksheet-delete':
				RED.view.removeActiveWorkspace();
				break;
			case 'worksheet-rename':
				RED.view.renameActiveWorkspace();
				break;

			case 'run-deploy':
				RED.runtime.run();
				break;
			case 'run-pause':
				break;
			case 'run-stop':
				RED.runtime.shutdown();
				break;
			case 'run-show-log':
				RED.runtime.show_terminal();
				break;

			case 'help-about':
				break;
				
		}
	});


	function enable_menu(id, en) {
		let item = $("#menubar li[data-menu='" + id + "']");
		if (en) {
			item.removeClass("ui-state-disabled");
		} else {
			item.addClass("ui-state-disabled");
		}
	}







	$('#btn-keyboard-shortcuts').click(function(){showHelp();});

	function make_name(n) {
		var name = (n.name ? n.name : n.id);
		name = name.replace(/[^a-zA-Z0-9_$]/g, "_");
		return name
	}

	$( "#node-dialog-confirm-deploy" ).dialog({
			title: "Confirm deploy",
			modal: true,
			autoOpen: false,
			width: 530,
			height: 230,
			buttons: [
				{
					text: "Confirm deploy",
					click: function() {
						save(true);
						$( this ).dialog( "close" );
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

	// from http://css-tricks.com/snippets/javascript/get-url-variables/
	function getQueryVariable(variable) {
		var query = window.location.search.substring(1);
		var vars = query.split("&");
		for (var i=0;i<vars.length;i++) {
			var pair = vars[i].split("=");
			if(pair[0] == variable){return pair[1];}
		}
		return(false);
	}

	function loadNodes() {
			$(".palette-scroll").show();
			$("#palette-search").show();
			RED.storage.load();
			RED.view.redraw();
			setTimeout(function() {
				$("#btn-deploy").removeClass("disabled").addClass("btn-danger");
				$("#btn-import").removeClass("disabled").addClass("btn-success");
			}, 1500);
			$('#btn-deploy').click(function() { save(); });
			// if the query string has ?info=className, populate info tab
			var info = getQueryVariable("info");
			if (info) {
				RED.sidebar.info.setHelpContent('', info);
			}
	}

	$('#btn-node-status').click(function() {toggleStatus();});

	var statusEnabled = false;
	function toggleStatus() {
		var btnStatus = $("#btn-node-status");
		statusEnabled = btnStatus.toggleClass("active").hasClass("active");
		RED.view.status(statusEnabled);
	}
	
	function showHelp() {

		var dialog = $('#node-help');

		//$("#node-help").draggable({
		//        handle: ".modal-header"
		//});

		dialog.on('show',function() {
			RED.keyboard.disable();
		});
		dialog.on('hidden',function() {
			RED.keyboard.enable();
		});

		dialog.modal();
	}

	$(function() {
		$(".palette-spinner").show();

		// server test switched off - test purposes only
		var patt = new RegExp(/^[http|https]/);
		var server = false && patt.test(location.protocol);

		if (!server) {
			
			var metaData = $.parseJSON($("script[data-container-name|='InputOutputCompatibilityMetadata']").html());
			requirements = metaData["requirements"];

			for (let n in RED.node_defs) {
				let def = RED.node_defs[n]
				RED.nodes.registerType(def["classname"], def);
			}
			/*
			let var_definitions = $('script[data-definition-name]')
			//console.log(var_definitions);
			$.each(var_definitions, function(key, value) {
				let json = $.parseJSON($(value).html())
				//console.log(json)
				RED.nodes.registerType(json["type"], json["data"]);
			})

			
			/*
			var data = $.parseJSON($("script[data-container-name|='NodeDefinitions']").html());
			var nodes = data["nodes"];
			$.each(nodes, function (key, val) {
				RED.nodes.registerType(val["type"], val["data"]);
			});
			*/
			RED.keyboard.add(/* ? */ 191, {shift: true}, function () {
				showHelp();
				d3.event.preventDefault();
			});

			loadNodes();

			RED.view.addConsole();
		
			$(".palette-spinner").hide();

		} else {
			$.ajaxSetup({beforeSend: function(xhr){
				if (xhr.overrideMimeType) {
					xhr.overrideMimeType("application/json");
				}
			}});
			$.getJSON( "resources/nodes_def.json", function( data ) {
				var nodes = data["nodes"];
				$.each(nodes, function(key, val) {
					RED.nodes.registerType(val["type"], val["data"]);
				});
				RED.keyboard.add(/* ? */ 191,{shift:true},function(){showHelp();d3.event.preventDefault();});
				loadNodes();
				$(".palette-spinner").hide();
			})
		}
	});

	return {
		enable_menu: enable_menu
	};
})();
