

RED.diagram = (function() {

    var diagrams = {};

    function createDiagram(node) {
        let name = node.name || node.id;
        node = RED.nodes.node(name)
        let canvas = $("<canvas id='node-red-diagram-" + name + "' class='diagram'></canvas>");
        $("#node-red-diagrams").append(canvas);

		var tabId = "diagram";
		var ws = {
			type:"diagram",
			id: name, 
			label: name
		};
		RED.view.workspace_tabs.addTab(ws);


        let datasets = [ ]
        let links = RED.nodes.links;
        // console.log(links);
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
        // console.log(datasets);

        let labels=[];
        for (n=0; n<node.buffersize; ++n)
            labels.push(n);   

        rgb = (r,g,b) => r << 16|g<<8|b;

        let chart = new Chart(canvas.get(0), {
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
                        },
                        zoom: {
                            pan: {
                                enabled: true,
                                scaleMode: 'xy',
                            },
                            zoom: {
                                mode: 'xy',
                                wheel: {
                                    enabled: true
                                },
                                drag: {
                                    enabled: true,
                                    modifierKey: 'ctrl',
                                    borderColor: rgb(255,0,0),
                                    backgroundColor: rgb(127,0,0),

                                }

                            }

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


        let diagram = {            id: name,
            name: name,
            chart: chart,
            datasets: datasets
        };

        diagrams[name] = diagram;

    }

    function showDiagram(id) {
        $("#node-red-diagrams .diagram").hide();
        $("#node-red-diagram-"+id).show();
        $("#node-red-diagrams").show();
    }

    function hideDiagrams() {
        $("#node-red-diagrams").hide();

    }

    function destroyDiagrams() {
        for (let n in diagrams) {
            RED.view.workspace_tabs.removeTab(n);
        }
        $("#node-red-diagrams").empty();
        diagrams = {};
    }

    function setData(datasets) {
        for( let name in datasets) {
            let dataset = datasets[name];
            let diagram = diagrams[name];
            // console.log(diagram, dataset);

            for (let ds in diagram.datasets) {
                let port = diagram.datasets[ds].port;
                diagram.datasets[ds].data = dataset[port];
            }
            diagram.chart.update();
        }
    }

    return {
        createDiagram: createDiagram,
        showDiagram: showDiagram, 
        hideDiagrams: hideDiagrams, 
        destroyDiagrams: destroyDiagrams,
        setData: setData
    }
})();