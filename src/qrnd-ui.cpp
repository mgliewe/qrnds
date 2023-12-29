
#include <string.h>
#include <getopt.h>

#include <stdexcept>
#include <string>
#include <iostream>

#include "qrnd-ui.h"

#include "cJSON/cJSON.h"

namespace QRND::UI {


static AppServer *appserver_instance;

static std::map<std::string, UI::Node *> nodes;

static void register_node(const char *name, UI::Node *ptr);
static void unregister_node(const char *name);
static void unregister_node(UI::Node *ptr);

Node *lookup_node(const char *name);



void register_node(const char *name, UI::Node *ptr) {
    std::cerr << "registering node " << name << std::endl;
    nodes[name] = ptr;
}

void unregister_node(const char *name) {
    nodes.erase(name);
}

void unregister_node(UI::Node *ptr) {
    for (std::map<std::string, UI::Node *>::iterator i = nodes.begin(); i!=nodes.end(); ++i) {
        if (i->second == ptr) {
            unregister_node(i->first.c_str());
            break;
        }
    }
}

UI::Node *lookup_node(const char *name) {
    std::cerr << "lookup node " << name << std::endl;
    std::map<std::string, Node *>::iterator i = nodes.find(name);
    if (i == nodes.end())
        return NULL;
    return i->second;
}

Node::Node(const char *name) {
    register_node(name, this);
}

Node::~Node() {
    unregister_node(this);
}


cJSON *Node::call(const char *method, cJSON *param) {
    if (strcmp(method, "get_values")==0) {
        return get_values();
    } else if (strcmp(method, "get_data")==0) {
        return get_data();
    } else {
        throw std::runtime_error("no such method: "+std::string(method)+"()");
    }
}


class AppControlNode : public Node {
    AppServer *srv;
public:
    AppControlNode(AppServer *srv) 
        : Node("Module"), srv(srv) 
    { }

    cJSON *call(const char *method, cJSON *param);

};


cJSON *AppControlNode::call(const char *method, cJSON *param) {
    if (strcmp(method, "pause") == 0) {
        srv->pause();
        cJSON *json = js_new();
        js_set(json, "status", "ok");
        return json;
    }
    else if (strcmp(method, "resume") == 0) {
        srv->resume();
        cJSON *json = js_new();
        js_set(json, "status", "ok");
        return json;
    }
    else if (strcmp(method, "get_json") == 0) {
        cJSON *json = js_new();
        js_set(json, "name", srv->get_name());
        js_set(json, "nodes", srv->get_json());
        return json;
    }
    else if (strcmp(method, "shutdown") == 0) {
        srv->end();
        cJSON *json = js_new();
        js_set(json, "status", "shutdown");
        return json;
    }
    return Node::call(method, param);
}


static void onopen(ws_cli_conn_t *client);
static void onclose(ws_cli_conn_t *client);
static void onmessage(ws_cli_conn_t *client,
			const unsigned char *msg, uint64_t msg_size, int type);

AppServer::AppServer() { 
    ws_server = new ::ws_server;

    ws_server->host = "0.0.0.0",
    ws_server->port = 9000,
    ws_server->thread_loop   = 0;
    ws_server->timeout_ms    = 1000;

    ws_server->evs.onopen    = onopen;
    ws_server->evs.onclose   = onclose;
    ws_server->evs.onmessage = onmessage;
}

static const char *appserver_optstr = "p:";
static struct option appserver_options[] = {
    "port", required_argument, 0, 'p',
    0 , 0, 0, 0
};

AppServer::AppServer(int argc, char * const *argv) 
    : AppServer()
{ 
    optind = 0;
    for(;;) {
        int o = getopt_long(argc, argv, appserver_optstr, appserver_options, NULL);
        if (o<0) break;

        switch(o) {
            case 'p':
                break;
            default:
                throw std::runtime_error("bad argv option");
        }
    }
}

void AppServer::run() {
    appserver_instance = this;
    this->init();
    this->start();
    ws_socket(this->ws_server);
}

static void onopen(ws_cli_conn_t *client)
{
	char *cli, *port;
	cli  = ws_getaddress(client);
	port = ws_getport(client);
	printf("Connection opened from %s:%s\n", cli, port);
}

static void onclose(ws_cli_conn_t *client)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("Connection closed from %s\n", cli);
}

static const char *get_json() {
    return appserver_instance->get_json();
}

static cJSON *get_values() {
    return appserver_instance->get_values();
}

static cJSON *get_data() {
    return appserver_instance->get_data();    
}

static void onmessage(ws_cli_conn_t *client,
	const unsigned char *msg, uint64_t size, int type)
{
	char *cli;
	cli = ws_getaddress(client);
	//printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s\n",
	//	msg, size, type, cli);

	/**
	 * Mimicks the same frame type received and re-send it again
	 *
	 * Please note that we could just use a ws_sendframe_txt()
	 * or ws_sendframe_bin() here, but we're just being safe
	 * and re-sending the very same frame type and content
	 * again.
	 *
	 * Client equals to NULL: broadcast
	 */
    //cJSON *json = cJSON_ParseWithLength((const char *)msg, size);
    cJSON *json = js_parse((const char *)msg, size);
    
    bool is_error = false;;
    std::string id = js_get_str(json, "id");
    std::string errmsg;
    cJSON *result;

    if (!json) {
        is_error = true;
        errmsg = "protocol error: bad request";

    } else {
        std::string obj = js_get_str(json, "object");

        std::string method = js_get_str(json, "method");
        cJSON *args = js_get(json, "arguments");

        if (id.empty()) {
            is_error = true;
            errmsg = "protocol error: missing id";
        }
        else if (method.empty()) {
            is_error = true;
            errmsg = "protocol error: missing method";
        }
        else if (obj.empty()) {
            if (method=="get_json") {
                const char *json = get_json();
                result = js_parse(json);
            } else if (method=="get_values") {
                result = get_values();
            } else if (method=="get_data") {
                result = get_data();
            } else {
                is_error = true;
                errmsg = "runtime error: unknown method";
            }
        } else {
            UI::Node *node = lookup_node(obj.c_str());
            if (!node) {
                is_error = true;
                errmsg = "runtime error: unknown object '" + obj + "'";
            } else {
                try {
                    result = node->call(method.c_str(), args);
                } catch (std::exception &err) {
                    is_error = true;
                    errmsg = err.what();
                } catch(...) {
                    is_error = true;
                    errmsg = "unknown exception";
                }
            }
        }
        js_delete(json);

    }

    json = js_new();
    js_set(json, "id", id.c_str());
    js_set(json, "type", is_error? "ERROR" : "SUCCESS");
    if (is_error) {
        js_set(json, "message", errmsg.c_str());
    } else {
        js_set(json, "result", result);
    }

    char *json_str = cJSON_Print(json);
    js_delete(json);

	//printf("I reply: %s\n", json_str);

    ws_sendframe(client, json_str, strlen(json_str),WS_FR_OP_TXT);

    free(json_str);
}



cJSON *Node::get_values() {
    cJSON *json = js_new();
    js_set(json, "name", UI::Node::name);
    return json;
}

cJSON *Node::get_data() {
    return js_new();
}





}  // end namespace
