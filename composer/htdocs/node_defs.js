RED.node_defs = 
[
    {
        "classname": "LineDiagram",
        "input": [
            "the first input",
            "the second input",
            "the third input",
            "the fourth input"
        ],
        "output": [],
        "defaults": {
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "imutable": true
                },
                "comment": [
                    "Number of datapooints in diagram                      "
                ],
                "value": "1024"
            },
            "display-tab": {
                "name": "display-tab",
                "datatype": "string",
                "attributes": {
                    "imutable": true
                },
                "comment": [
                    "The Tab, where to show the disgram in composer                      "
                ]
            },
            "flex-flow": {
                "name": "flex-flow",
                "datatype": "string",
                "attributes": {
                    "imutable": true
                },
                "comment": [
                    "A css style to render the diagram container                      "
                ]
            },
            "flex": {
                "name": "flex",
                "datatype": "string",
                "attributes": {
                    "imutable": true
                },
                "comment": [
                    "A css style to render the diagram                      "
                ]
            },
            "flex-order": {
                "name": "flex-order",
                "datatype": "string",
                "attributes": {
                    "imutable": true
                },
                "comment": [
                    "A css style to order the diagrams in its container                      "
                ]
            }
        },
        "description": [
            "A diagram display in composer. Each input connects to a FrameBuffer,",
            "holding the last recent Frame.  "
        ],
        "constructor": [],
        "summary": "A line diagram with 4 inputs.",
        "shortName": "LineDiagram",
        "inputs": "4",
        "outputs": "0",
        "category": "diagram",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "NullProducer",
        "input": [],
        "output": [
            "the output stream"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer                      "
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "32768"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames"
                ],
                "value": "16"
            }
        },
        "description": [
            "This producer is mainly intended for testing purposes."
        ],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A producer sending all Zeros.",
        "shortName": "NullProducer",
        "inputs": "0",
        "outputs": "1",
        "category": "producer",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "RandProducer",
        "input": [],
        "output": [
            "the output stream"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer                      "
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "32768"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "16"
            },
            "seed": {
                "name": "seed",
                "datatype": "number",
                "attributes": {},
                "comment": [
                    "The seed values used for srand(). Use 0 for calling time()                                   "
                ]
            }
        },
        "description": [
            "This producer is mainly intended for testing purposes."
        ],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A producer using libs rand() as its input.",
        "shortName": "RandProducer",
        "inputs": "0",
        "outputs": "1",
        "category": "producer",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "GaussianProducer",
        "input": [
            "the entropy source"
        ],
        "output": [
            "the output stream"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer                      "
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "32768"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "16"
            },
            "mean": {
                "name": "mean",
                "datatype": "number",
                "attributes": {},
                "comment": [
                    "The mean value                           "
                ],
                "value": "127"
            },
            "derivation": {
                "name": "derivation",
                "datatype": "number",
                "attributes": {},
                "comment": [
                    "The standard derivation of the mean value      "
                ],
                "value": "100"
            }
        },
        "description": [
            "using an entropy source. It uses a modified Box-Muller transformation.",
            "This producer is mainly intended for testing purposes."
        ],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A producer producing a normal distributed signal",
        "shortName": "GaussianProducer",
        "inputs": "1",
        "outputs": "1",
        "category": "producer",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "VoidSink",
        "input": [
            "the input stream"
        ],
        "output": [],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer                      "
                ]
            }
        },
        "description": [
            "This consumer is mainly intended for testing purposes."
        ],
        "constructor": [
            "name"
        ],
        "summary": "A consumer discarding al input.",
        "shortName": "VoidSink",
        "inputs": "1",
        "outputs": "0",
        "category": "consumer",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "Stream2Splitter",
        "input": [
            "the input stream"
        ],
        "output": [
            "the original input stream",
            "a copy of the input stream"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer                      "
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "32768"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "32"
            }
        },
        "description": [
            "The splitter distributes copies of its input to several outputs.",
            "The splitter creates in-memory copies of each input frame. "
        ],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A producer splitting its input to several outputs.",
        "shortName": "Stream2Splitter",
        "inputs": "1",
        "outputs": "2",
        "category": "dataflow",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "Stream4Splitter",
        "input": [
            "the input stream"
        ],
        "output": [
            "the original input stream",
            "first copy of the input stream",
            "second copy of the input stream",
            "third copy of the input stream"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer                      "
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "32768"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "64"
            }
        },
        "description": [
            "The splitter distributes copies of its input to several outputs.",
            "The splitter creates in-memory copies of each input frame. "
        ],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A producer splitting its input to several outputs.",
        "shortName": "Stream4Splitter",
        "inputs": "1",
        "outputs": "4",
        "category": "dataflow",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "AmplitudeMeter",
        "input": [
            "the input stream"
        ],
        "output": [
            "the original input stream",
            "Signal rms amplitude over time",
            "Signal avg amplitude over time"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer"
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "1024"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "8"
            },
            "windowsize": {
                "name": "windowsize",
                "datatype": "number",
                "attributes": {
                    "mutable": true
                },
                "comment": [
                    "Windows size for measurement"
                ],
                "value": "32768"
            },
            "avg": {
                "name": "avg",
                "datatype": "number",
                "attributes": {
                    "runtime": true
                },
                "comment": [
                    "Current average value of the amplitude                       "
                ]
            },
            "rms": {
                "name": "rms",
                "datatype": "number",
                "attributes": {
                    "runtime": true
                },
                "comment": [
                    "Current mean square root value of the amplitude                       "
                ]
            }
        },
        "description": [],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A filter, measuring the rms amplitude of its input.",
        "shortName": "AmplitudeMeter",
        "inputs": "1",
        "outputs": "3",
        "category": "measurement",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "BandwidthMeter",
        "input": [
            "the input stream"
        ],
        "output": [
            "the original input stream",
            "bandwidth over time"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer"
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "1024"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "8"
            },
            "windowsize": {
                "name": "windowsize",
                "datatype": "number",
                "attributes": {
                    "mutable": true
                },
                "comment": [
                    "Windows size for measurement"
                ],
                "value": "32768"
            },
            "bandwidth": {
                "name": "bandwidth",
                "datatype": "number",
                "attributes": {
                    "runtime": true
                },
                "comment": [
                    "Current bandwidth                       "
                ]
            }
        },
        "description": [],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A filter, measuring the rms amplitude of its input.",
        "shortName": "BandwidthMeter",
        "inputs": "1",
        "outputs": "2",
        "category": "measurement",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "Histogram",
        "input": [
            "the input stream"
        ],
        "output": [
            "the original input stream",
            "Signal distribution"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer"
                ]
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames                                         "
                ],
                "value": "4"
            },
            "windowsize": {
                "name": "windowsize",
                "datatype": "number",
                "attributes": {
                    "mutable": true
                },
                "comment": [
                    "Windows size for measurement"
                ],
                "value": "32768"
            }
        },
        "description": [],
        "constructor": [
            "name",
            "num_buffer"
        ],
        "summary": "A filter, collecting the signal distribution of the input.",
        "shortName": "Histogram",
        "inputs": "1",
        "outputs": "2",
        "category": "measurement",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    },
    {
        "classname": "Derivation",
        "input": [
            "the input stream"
        ],
        "output": [
            "the original input stream",
            "Signal mean amplitude",
            "Signal amplitude derivation"
        ],
        "defaults": {
            "name": {
                "name": "name",
                "datatype": "name",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "The class name shown in Composer"
                ]
            },
            "buffersize": {
                "name": "buffersize",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Framesize for the output stream"
                ],
                "value": "1024"
            },
            "num_buffer": {
                "name": "num_buffer",
                "datatype": "number",
                "attributes": {
                    "ctr": true,
                    "imutable": true
                },
                "comment": [
                    "Number of preallocated Frames for output"
                ],
                "value": "4"
            },
            "windowsize": {
                "name": "windowsize",
                "datatype": "number",
                "attributes": {
                    "mutable": true
                },
                "comment": [
                    "Windows size for measurement"
                ]
            },
            "mean_value": {
                "name": "mean_value",
                "datatype": "number",
                "attributes": {
                    "runtime": true
                },
                "comment": [
                    "calculated mean value"
                ]
            },
            "derivation": {
                "name": "derivation",
                "datatype": "number",
                "attributes": {
                    "runtime": true
                },
                "comment": [
                    "calculated derivation"
                ],
                "value": "32768"
            }
        },
        "description": [],
        "constructor": [
            "name",
            "buffersize",
            "num_buffer"
        ],
        "summary": "A filter, calculating mean amplitude and derivation of a signal.",
        "shortName": "Derivation",
        "inputs": "1",
        "outputs": "3",
        "category": "measurement",
        "color": "#E6E0F8",
        "icon": "arrow-in.png"
    }
]
