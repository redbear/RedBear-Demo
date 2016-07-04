#ifndef CLOUD_JSON_H_
#define CLOUD_JSON_H_

#include "application.h"

extern uint8_t json_enable;

extern uint8_t json_UUID[16];

extern uint8_t json_namespaceID[10];

extern uint8_t json_instanceID[6];

extern uint8_t json_url[18];

extern char* json_key;

extern uint32_t json_interv;

extern char* parseJson(char *jsonString);

extern uint8_t url_len;


#endif


