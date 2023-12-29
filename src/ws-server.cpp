
#include <string>
#include <map>
#include <stdexcept>


#include <stdio.h>
#include <string.h>

#include "cJSON/cJSON.h"
#include "wsServer/ws.h"

#include "appsrv.h"

using QRND::UI::Node;


class Object : public UI::Node {
public:
    Object(const char *name) : UI::Node(name) {

    }
protected:

    cJSON *call(const char *method, cJSON *param) {
        if (strcmp(method, "test")==0) {
            return cJSON_CreateString("Ok");
        }
        throw std::runtime_error("no such method: "+std::string(method)+"()");
    }

};




static std::string get_obj_string(cJSON *obj, const char *name) {
    cJSON *json = cJSON_GetObjectItemCaseSensitive(obj, name);
    if (cJSON_IsString(json)) {
        return json->valuestring;
    }
    return NULL;
}

static cJSON *get_obj_value(cJSON *obj, const char *name) {
    cJSON *json = cJSON_GetObjectItemCaseSensitive(obj, name);
    return json;
}


void onopen(ws_cli_conn_t *client)
{
	char *cli, *port;
	cli  = ws_getaddress(client);
	port = ws_getport(client);
	printf("Connection opened, addr: %s, port: %s\n", cli, port);
}

void onclose(ws_cli_conn_t *client)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("Connection closed, addr: %s\n", cli);
}

void onmessage(ws_cli_conn_t *client,
	const unsigned char *msg, uint64_t size, int type)
{

    cJSON *json = cJSON_ParseWithLength((const char *)msg, size);
    
    bool is_error = false;;
    std::string id = get_obj_string(json, "id");
    std::string errmsg;
    cJSON *result;

    if (!json) {
        is_error = true;
        errmsg = "protocol error: bad request";

    } else {
        std::string obj = get_obj_string(json, "object");
        std::string method = get_obj_string(json, "method");
        cJSON *args = get_obj_value(json, "arguments");

        if (id.empty()) {
            is_error = true;
            errmsg = "protocol error: missing id";
        }
        else if (obj.empty()) {
            is_error = true;
            errmsg = "protocol error: missing object";
        }
        else if (method.empty()) {
            is_error = true;
            errmsg = "protocol error: missing method";
        }
        else {
            UI::Node *node = lookup_node(obj.c_str());
            if (!node) {
                is_error = true;
                errmsg = "runtime error: unknown object";
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
        cJSON_Delete(json);

    }

    json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "id", id.c_str());
    cJSON_AddStringToObject(json, "type", is_error? "ERROR" : "SUCCESS");
    if (is_error) {
        cJSON_AddStringToObject(json, "message", errmsg.c_str());
    } else {
        cJSON_AddItemToObject(json, "result", result);
    }

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

	//printf("I reply: %s\n", json_str);

    ws_sendframe(client, json_str, strlen(json_str),WS_FR_OP_TXT);

    free(json_str);
}

void start_appsrv() {

    Object *obj = new Object("test");

    struct ws_server server = {
		/*
		 * Bind host:
		 * localhost -> localhost/127.0.0.1
		 * 0.0.0.0   -> global IPv4
		 * ::        -> global IPv4+IPv6 (DualStack)
		 */
		.host = "0.0.0.0",
		.port = 0,
		.thread_loop   = 0,
		.timeout_ms    = 1000,
		.evs = {
            .onopen    = &onopen,
		    .onclose   = &onclose,
		    .onmessage = &onmessage
        }
	};
    ws_socket(&server);

	/*
	 * If you want to execute code past ws_socket(), set
	 * .thread_loop to '1'.
	 */
}
