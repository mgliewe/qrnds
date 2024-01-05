/* public domain
 * vim: set ts=4:
 */

RED.runtime = (function() {

    var terminal;
    var terminal_socket;
    var runtime_socket;

    terminal_host = 'localhost:3000';

    terminal = new Terminal({
    });
    let fit_addon = new FitAddon.FitAddon();
    terminal.loadAddon(fit_addon);
    terminal.fit = () => fit_addon.fit();
    terminal.open(document.getElementById('node-dialog-console-terminal'));
    terminal.clear();
    connect_terminal();

    function connect_terminal() {
        terminal.write("\x1b[3;92m");
        terminal.writeln("connecting to terminal host at "+terminal_host)
        terminal.write("\x1b[0m");
        terminal_socket = new WebSocket("ws://localhost:3000/console");

        terminal_socket.onerror = () => {
            terminal.writeln("error on terminal server connection.")    
            terminal_socket = null;        
        };
        terminal_socket.onclose = () => {
            terminal.write("\x1b[3;92m");
            terminal.writeln("terminal server connection closed.")            
            terminal.write("\x1b[0m");
            terminal_socket = null;        
        };

        terminal_socket.onopen = () => {
            terminal.write("\x1b[3;92m");
            terminal.writeln("terminal server connection established.")            
            terminal.write("\x1b[0m");

            terminal_socket.send( JSON.stringify(
                { action: "resize", cols: terminal.cols, rows: terminal.rows }
            ));
        };

        terminal_socket.onmessage = (ev) => {
            let json = JSON.parse(ev.data);
            // console.log(json)
            switch(json.status) {
                case 'stderr':
                    terminal.write("\x1b[3;92m");
                    terminal.writeln(json.msg);
                    terminal.write("\x1b[0m");
                    break;
                case 'stdout':
                    terminal.write(json.msg);
                    break;
                case 'app_startup':
                    runtime_socket = new WebSocket("ws://localhost:"+json.port);
                    terminal.write("\x1b[3;92m");
                    terminal.writeln("connecting application on port "+json.port)            
                    terminal.write("\x1b[0m");
        
                    runtime_socket.onopen = () => {
                        terminal.write("\x1b[3;92m");
                        terminal.writeln("application connection established");
                        terminal.write("\x1b[0m");    
                        hide_terminal();
                        set_runstate(true);
                    };
                    runtime_socket.onclose = () => {
                        terminal.write("\x1b[3;92m");
                        terminal.writeln("application connection closed.");
                        terminal.write("\x1b[0m");    
                        set_runstate(false);
                    };
                    runtime_socket.onerror = () => {

                    };

                    runtime_socket.onmessage = (ev) => {
                        let json=JSON.parse(ev.data);
                        console.log(json);
                        promise = callbacks[json.id];
                        if (json.type=="ERROR") {
                            promise.rejectWith(json, [json.msg]);
                        } else {
                            promise.resolveWith(json, [json.result]);
                        }
                    }

                    //hide_terminal();

                    break;
            }
        }
    }

    let callbacks = {};
    let rpc_counter = 1;

    function rpc(obj, method, parm) {
        let id = String(rpc_counter++);
        runtime_socket.send(JSON.stringify({
            id: id, object: obj, method: method, 
            arguments: parm || []
        }));

        let promise = $.Deferred()
        callbacks[id] = promise;
        promise.always(() => delete callbacks[id] );

        return promise;
    }

    function show_terminal() {
        RED.view.workspace_tabs.activateTab("console")
    }

    function hide_terminal() {
        RED.view.workspace_tabs.activateTab("node-graph")
    }

    function fit_terminal() {
        setTimeout(terminal.fit,100);
    }

    var diagrams;
    var data_timeout;

    function open_diagrams() {

        RED.diagram.destroyDiagrams();
        let nodes = RED.nodes.createCompleteNodeSet();
        for(let i=0; i<nodes.length; ++i) {
            if (nodes[i].type == 'LineDiagram') {
                RED.diagram.createDiagram(nodes[i]);
            }
        }
        refresh_data();
    }

    function refresh_data() {
        rpc("","get_data").then(
            (ds) => {
                RED.diagram.setData(ds);
                if (RED.runtime.runstate!='STOPPED')
                    setTimeout(refresh_data, 500);
            }
        );
    }

    function open_diagrams_old() {

        RED.diagram.destroyDiagrams();
        let nodes = RED.nodes.createCompleteNodeSet();
        for(let i=0; i<nodes.length; ++i) {
            if (nodes[i].type == 'LineDiagram') {
                RED.diagram.createDiagram(nodes[i]);
            }
        }


        $("#node-dialog-diagram").empty();
        diagrams = { };
        //let nodes = RED.nodes.createCompleteNodeSet();
        for(let i=0; i<nodes.length; ++i) {
            if (nodes[i].type == 'LineDiagram') {
                let n = nodes[i];
                let node =  RED.nodes.node(n.id);
                let name = make_name(n);

                console.log({ n:n, node:node, name:name});  

                let diagram = $("<canvas style='flex:1' id='diagram-" + name + "-canvas' class='diagram-canvas'></canvas>");

                $("#node-dialog-diagram").append(diagram);

                let datasets = [ ]
                let links = RED.nodes.links;
                console.log(links);
                let colors = [ '#800', '#00f', '#0ff', '#f00'];
                let bgcolors = [ '#f44', '#f00f', '#f0ff', '#ff00'];
                let ports = [ ];
                for (let l=0; l<links.length; ++l) {
                    if (links[l].target === node) {
                        let port = parseInt(links[l].targetPort);
                        console.log(links[l]);
                        datasets.push({
                            port: port,
                            label: (port+1) + ': ' + links[l].source.name + '.' + links[l].sourcePort,
                            id: name+'_buffer_' + port,
                            borderColor: colors[port],
                            backgroundColor: colors[port],
                            data: [ ]
                        })
                    }
                }
                console.log(datasets);

                let labels=[];
                for (n=0; n<node.buffersize; ++n)
                    labels.push(n);   

                let canvas = diagram.get(0);
                let chart = new Chart(canvas, {
                        type: "line",
                        data: {
                            labels: labels,
                            datasets: datasets
                        },
                        options: {
                            animation: false,

                            responsive: true,
                            plugins: {
                                legend: {
                                    position: 'top'
                                },
                                title: {
                                    display: true,
                                    text: name
                                }
                            },
                            interactions: {
                                mode: 'index',
                                intersect: false
                            },
                            scales: {
                                x: {
                                    display: true,
                                },
                                y: {
                                    display: true,
                                    min:0,
                                    max: 256
                                }
                            }
                        }
                    }
                );
                diagrams[name] = {name: name, chart: chart, datasets: datasets};
            }
        }

        console.log(diagrams);
        load_data();
    }

    function load_data() {
        rpc("","get_data").then(
            datasets => {
                for( let name in datasets) {
                    let dataset = datasets[name];
                    let diagram = diagrams[name];
                    console.log(diagram, dataset);

                    for (let ds in diagram.datasets) {
                        let port = diagram.datasets[ds].port;
                        diagram.datasets[ds].data = dataset[port];
                    }
                    diagram.chart.update();
                }
                if (RED.runtime.runstate!='STOPPED')
                    data_timeout = setTimeout(() => {
                        load_data() 
                        }, 500);
            }   
        )

    }

    function close_diagrams() {

    }

    function set_runstate(is_running) {
        if (is_running && RED.runtime.runstate!="RUNNING") {
            RED.runtime.runstate = "RUNNING"
            RED.palette.hide();
            RED.enable_menu("file-new", false);
            RED.enable_menu("file-load", false);
            RED.enable_menu("edit-cut", false);
            RED.enable_menu("edit-paste", false);
            RED.enable_menu("edit-delete", false);
            RED.enable_menu("run-deploy", false);
            RED.enable_menu("run-stop", true);

            open_diagrams();

        } else if (RED.runtime.runstate!="STOPPED") {
            RED.runtime.runstate = "STOPPED"
            let active_tab = $('#workspace-tabs li.active a').attr('href').substr(1); 
            if ( active_tab == 'node-graph')
                RED.palette.show();
            RED.enable_menu("file-new", true);
            RED.enable_menu("file-load", true);
            RED.enable_menu("edit-cut", true);
            RED.enable_menu("edit-paste", true);
            RED.enable_menu("edit-delete", true);
            RED.enable_menu("run-deploy", true);
            RED.enable_menu("run-stop", false);

            close_diagrams();
        }
    }

    function deploy() {
        show_terminal();
        let nodes = RED.nodes.createCompleteNodeSet();
        let cpp = generate_code(nodes);

        terminal_socket.send(JSON.stringify({
            action: "deploy", 
            nodes: nodes, code: cpp
        }));
    }

    function x_run() {
        show_terminal();
        terminal_socket.send(JSON.stringify({
            action: "start", command: "ps axf"
        }));
    }

    function run() {
        RED.storage.save_file(RED.storage.filename, () => {
            let cpp = generate_code();
            let p = lib.path.parse(RED.storage.filename);
            let filename = p.dir + '/' + p.name + '.cpp';
            lib.vfs.writeFile(filename, cpp)
               .then(() => {
                    show_terminal();
                    terminal_socket.send(JSON.stringify({
                        action: "start", 
                        command: "cd $HOME/" + p.dir + ";" 
                               + "c++ -g $CXX_FLAGS -o " + p.name + " " + p.name + ".cpp $QRND_LIBS"
                               + "&& ./" + p.name
                    }));                    
               });
        });
    }

    function shutdown() {
        if (terminal_socket) {
            terminal_socket.send( JSON.stringify(
                { action: "shutdown" }
            ));    
        }
    }

    function deploy() {
        terminal.clear();
	}

    function show_code() {
        let nodes = RED.nodes.createCompleteNodeSet();
        let cpp = generate_code(nodes);

        RED.view.getForm('dialog-form', 'export-clipboard-dialog', function (d, f) {
            $("#node-input-export").val(cpp).focus(function() {
            var textarea = $(this);
            textarea.select();
            textarea.mouseup(function() {
                textarea.unbind("mouseup");
                return false;
            });
            }).focus();
        $( "#dialog" ).dialog("option","title","Export to Arduino").dialog( "open" );
        });

    }


    function make_name(n) {
		var name = (n.name ? n.name : n.id);
		name = name.replace(/[^a-zA-Z0-9_$]/g, "_");
		return name
	}

    function generate_code(namespace) {
        console.log(namespace)
        namespace = "composer_code"
        var nns = RED.nodes.createCompleteNodeSet();
        // sort by horizontal position, plus slight vertical position,
        // for well defined update order that follows signal flow
        nns.sort(function(a,b){ return (a.x + a.y/250) - (b.x + b.y/250); });

        var cpp = "// automatically generated code, do not edit\n\n"
                + "#include <qrnd-core.h>\n\n"
                + "#include <cassert>\n\n"
                + "#include <qrnd-ui.h>\n\n"
                + "#include <qrnd-ui-nodes.h>\n\n"
                + "namespace " + namespace + " {\n"
                + "using namespace QRND::UI;\n\n"
                ;

        cpp += "class Module : public AppServer {\n"
             + "public:\n"
             + "\tModule(int argc, char * const argv[]) \n"
             + "\t  : AppServer(argc, argv) \n\t{\n"
             + "\t}\n"

        cpp += "\n";

        cpp += "\t// declarations\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                if (n.type=='LineDiagram') {
                    var name = make_name(n)
                    cpp += "\tFrameBuffer *" + name + "_buffer_0 = 0;\n";    
                    cpp += "\tFrameBuffer *" + name + "_buffer_1 = 0;\n";    
                    cpp += "\tFrameBuffer *" + name + "_buffer_2 = 0;\n";    
                    cpp += "\tFrameBuffer *" + name + "_buffer_3 = 0;\n";    
                } else {
                    cpp += "\t" + n.type + " *";
                    var name = make_name(n)
                    cpp += name + " = 0;\n";    
                }
            }
        }
        cpp += "\n\n";

        cpp += "\t// get module name\n"
        cpp += "\tvirtual const char *get_name() {\n"
             + "\t\treturn \"" + namespace + "\";\n"
             + "\t}\n";

        cpp += "\t// get module source\n"
        cpp += "\tvirtual const char *get_json() {\n";
        let json = JSON.stringify(nns)
                    .replace( /["]/g, "\\\"")
                    .replace( /[\n]/g, "\\\n")
                    .replace( /[\t]/g, "\\\t");
        cpp += "\t\treturn \"" + json + "\";\n"
             + "\t}\n";
     
        cpp += "\n\n";

        cpp += "\t// initialize module\n"
        cpp += "\tvirtual void init() {\n";
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n);
                // console.log({node:node})
                cpp += "\t\t" + name + "_buffer_0 = new FrameBuffer(\"" + name + "_buffer_0\");\n";
                cpp += "\t\t" + name + "_buffer_1 = new FrameBuffer(\"" + name + "_buffer_1\");\n";
                cpp += "\t\t" + name + "_buffer_2 = new FrameBuffer(\"" + name + "_buffer_2\");\n";
                cpp += "\t\t" + name + "_buffer_3 = new FrameBuffer(\"" + name + "_buffer_3\");\n";

            } else  if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                var name = make_name(n);
                cpp += "\t\t" + name + " = new " + n.type + '(';
                for (let c in node._def.constructor) {
                    if (c!=0) cpp += ", ";
                    let arg = node._def.constructor[c]
                    let def = node._def.defaults[arg]
                    switch(def.datatype) {
                        case 'name':
                            cpp += '"' + node[arg] + '"';
                            break;
                        default:
                            cpp += node[arg];
                            break;
                    }
                }
                cpp += ');\n';

                for (let p in node._def.defaults) {
                    let prop = node._def.defaults[p];
                    // console.log({prop:prop, node:node})
                    if (!prop.attributes.ctr) {
                        if (node[prop.name]) {
                            cpp += "\t\t" + name + "->set_" + prop.name + "(";
                            let def = node._def.defaults[prop.name]
                            switch(def.datatype) {
                                case 'name':
                                    cpp += '"' + node[prop.name] + '"';
                                    break;
                                default:
                                    cpp += node[prop.name];
                                    break;
                            }
                            cpp += ");\n"
                        }
                    }
                }
            }
        }
        cpp += "\n";

        cpp += "\n";
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            if (n.wires) {
                var src_name = make_name(n);
                for (var j=0; j<n.wires.length; j++) {
                    var wire = n.wires[j][0];
                    // console.log(wire)
                    if (wire) {
                        var parts = wire.split(":")
                        var dst = RED.nodes.node(parts[0]);

                        if (dst.type=='LineDiagram') {
                            cpp += "\t\t" + src_name + "->connect_output(" + j +", *"
                            + make_name(dst) + "_buffer_" + parts[1] + ");\n";

                        } else {
                            cpp += "\t\t" + src_name + "->connect_output(" + j +", *"
                            + make_name(dst) + ");\n";

                       // cpp += "\t\t" + src_name + "->connect_output(" + j +", "
                       //     + make_name(dst) + ", " + parts[1] + ");\n";

                        }
                    }
                }
            }
        }

        cpp += "\t}\n\n";

        cpp += "\t// get module values\n";
        cpp += "\tvirtual cJSON *get_values() {\n"
             + "\t\tcJSON *values = js_new();\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type != 'tab' && n.type != 'console'  && n.type != 'diagram' && n.type!='LineDiagram') {
                var name = make_name(n)
                cpp += '\t\tjs_set(values, "' + name + '", ' + name + '->get_values());\n'
            }       
        }
        cpp += "\t\treturn values;\n\t}\n\n"

        cpp += "\t// get module diagram data\n";
        cpp += "\tvirtual cJSON *get_data() {\n"
             + "\t\tcJSON *dataset = js_new();\n"
             + "\t\tcJSON *data;\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n)

                cpp += '\t\tdata = js_array();\n'

                    + '\t\tif (' + name + '_buffer_0) {\n'
                    + '\t\t\tjs_push(data, ' + name + '_buffer_0->get_data());\n'
                    + '\t\t} else {\n'
                    + '\t\t\tjs_push(data, js_array());\n'
                    + '\t\t}\n'

                    + '\t\tif (' + name + '_buffer_1) {\n'
                    + '\t\t\tjs_push(data, ' + name + '_buffer_1->get_data());\n'
                    + '\t\t} else {\n'
                    + '\t\t\tjs_push(data, js_array());\n'
                    + '\t\t}\n'

                    + '\t\tif (' + name + '_buffer_2) {\n'
                    + '\t\t\tjs_push(data, ' + name + '_buffer_2->get_data());\n'
                    + '\t\t} else {\n'
                    + '\t\t\tjs_push(data, js_array());\n'
                    + '\t\t}\n'

                    + '\t\tif (' + name + '_buffer_3) {\n'
                    + '\t\t\tjs_push(data, ' + name + '_buffer_3->get_data());\n'
                    + '\t\t} else {\n'
                    + '\t\t\tjs_push(data, js_array());\n'
                    + '\t\t}\n';

                cpp += '\t\tjs_set(dataset, "' + name + '", data);\n';
            }       
        }
        cpp += "\t\treturn dataset;\n\t}\n\n"

        cpp += "\t// start module\n";
        cpp += "\tvirtual void start() {\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "_buffer_0->start();\n";
                cpp += "\t\t" + name + "_buffer_1->start();\n";
                cpp += "\t\t" + name + "_buffer_2->start();\n";
                cpp += "\t\t" + name + "_buffer_3->start();\n";
            } else if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "->start();\n";
            }
        }
        cpp += "\t}\n\n"

        cpp += "\t// pause module\n";
        cpp += "\tvirtual void pause() {\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "_buffer_0->pause();\n";
                cpp += "\t\t" + name + "_buffer_1->pause();\n";
                cpp += "\t\t" + name + "_buffer_2->pause();\n";
                cpp += "\t\t" + name + "_buffer_3->pause();\n";
            } else if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "->pause();\n";
            }
        }
        cpp += "\t}\n\n"

        cpp += "\t// resume module\n";
        cpp += "\tvirtual void resume() {\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "_buffer_0->resume();\n";
                cpp += "\t\t" + name + "_buffer_1->resume();\n";
                cpp += "\t\t" + name + "_buffer_2->resume();\n";
                cpp += "\t\t" + name + "_buffer_3->resume();\n";
            } else if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "->resume();\n";
            }
        }
        cpp += "\t}\n\n"

        cpp += "\t// stop module and cleanup\n";
        cpp += "\tvoid end() {\n"
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "_buffer_0->stop();\n";
                cpp += "\t\t" + name + "_buffer_1->stop();\n";
                cpp += "\t\t" + name + "_buffer_2->stop();\n";
                cpp += "\t\t" + name + "_buffer_3->stop();\n";
            } else if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                var name = make_name(n)
                cpp += "\t\t" + name + "->stop();\n";
            }
        }
        cpp += "\n";
        for (var i=0; i<nns.length; i++) {
            var n = nns[i];
            var node = RED.nodes.node(n.id);
            if (n.type=='LineDiagram') {
                var name = make_name(n)
                cpp += "\t\tdelete " + name + "_buffer_0;\n";
                cpp += "\t\tdelete " + name + "_buffer_1;\n";
                cpp += "\t\tdelete " + name + "_buffer_2;\n";
                cpp += "\t\tdelete " + name + "_buffer_3;\n";
            } else if (n.type != 'tab' && n.type != 'console' && n.type != 'diagram') {
                var name = make_name(n)
                cpp += "\t\tdelete " + name + ";\n";
            }
        }
        cpp += "\t}\n\n"

        cpp += "}; // end class Module\n\n"

        cpp += "} // end namespace\n"
             + "\n\n"
             + "int main(int argc, char * const argv[]) {\n"
             + "\t" + namespace + "::Module module(argc, argv);\n"
             + "\tmodule.run();\n"
             + "}\n\n"
        
        cpp += "// end automatically generated code\n";
        //console.log(cpp);    
        return cpp;   
    }

    return {
        terminal: terminal,
        callbacks: callbacks,

        runstate: "STOPPED",

        deploy: deploy,
        run: run,
        shutdown: shutdown,
        rpc: rpc,
        show_code: show_code,
        set_runstate: set_runstate,

        show_terminal: show_terminal,
        hide_terminal: hide_terminal,
        fit_terminal: fit_terminal
	}
})();
