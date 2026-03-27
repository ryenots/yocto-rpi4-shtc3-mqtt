#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define ADDRESS     "127.0.0.1:1883"
#define CLIENTID    "SHTC3-Publisher"
#define QOS         1
#define TIMEOUT     10000L

static MQTTClient client;
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
static MQTTClient_deliveryToken token;
static int rc;

void mqtt_register();
void mqtt_publish(char* payload, char* topic);
