#include "mqtt.h"

void mqtt_register(){
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
}

void mqtt_publish(char* payload, char* topic){
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    if(rc != MQTTCLIENT_SUCCESS){
        printf("Failed to publish, return code %d\n", rc);
        fflush(stdout);
        return;
    }
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}
