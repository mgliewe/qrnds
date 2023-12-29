
#include "qrnd-core.h"

#include "wsServer/ws.h"
#include "cJSON/cJSON.h"

#include <string.h>

#ifndef __QRND_UI_H__
#define __QRND_UI_H__

namespace QRND { namespace UI {

class Node {
protected:
    const char *name;
public:
    Node(const char *name);
    ~Node();

    virtual cJSON *call(const char *method, cJSON *param);
    virtual cJSON *get_values();
    virtual cJSON *get_data();
};

class AppServer {
    struct ws_server * ws_server;
public:
    AppServer();
    AppServer(int argc, char * const *argv);

    void run();

    virtual const char *get_name() = 0;
    virtual const char *get_json() = 0;

    virtual void init() = 0;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void end() = 0;

    virtual cJSON *get_values() = 0;
    virtual cJSON *get_data() = 0;
};

inline cJSON *js_parse(const char *json, unsigned sz) {
    return cJSON_ParseWithLength(json, sz);
}

inline cJSON *js_parse(const char *json) {
    return js_parse(json, strlen(json));
}

inline cJSON *js_parse(std::string &json) {
    return js_parse(json.c_str(), json.length());
}

inline cJSON *js_new() {
    return cJSON_CreateObject();
}

inline cJSON *js_array() {
    return cJSON_CreateArray();
}

inline void js_delete(cJSON *json) {
    cJSON_Delete(json);
}

inline char *js_cstr(cJSON *json) {
    return cJSON_Print(json);
}

inline void js_push(cJSON *array, cJSON *item) {
    cJSON_AddItemToArray(array, item);
}

inline void js_push(cJSON *array, int value) {
    cJSON *item = cJSON_CreateNumber(value); 
    cJSON_AddItemToArray(array, item);
}

inline void js_set(cJSON *json, const char *name, int value) {
    cJSON_AddNumberToObject(json, name, value);
}
inline void js_set(cJSON *json, const char *name, unsigned int value) {
    cJSON_AddNumberToObject(json, name, value);
}
inline void js_set(cJSON *json, const char *name, const char *value) {
    cJSON_AddStringToObject(json, name, value);
}
inline void js_set(cJSON *json, const char *name, bool value) {
    cJSON_AddBoolToObject(json, name, value);
}
inline void js_set(cJSON *json, const char *name, cJSON *value) {
    cJSON_AddItemToObject(json, name, value);
}

inline cJSON *js_get(cJSON *json, const char *name) {
    return cJSON_GetObjectItemCaseSensitive(json, name);
}

inline char *js_get_cstr(cJSON *json, const char *name) {
    cJSON *value = js_get(json, name);
    if (cJSON_IsString(value)) {
        //printf("%s=%s\n", name, json->valuestring);
        return value->valuestring;
    }
    return NULL;
}

inline std::string js_get_str(cJSON *json, const char *name) {
    return std::string(js_get_cstr(json, name));
}

} }

#endif // __QRND_UI_H__