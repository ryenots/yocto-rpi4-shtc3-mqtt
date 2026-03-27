#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "mqtt.h"

// #define DEBUG

#define WRITE_SIZE 2
#define READ_SIZE  6
#define TOPIC_TEMP "Temperature"
#define TOPIC_HUM  "Humidty"

#define SHTC3_I2C_ADDR 0x70

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    __uint8_t shtc3_wake[2] = {0x35, 0x17};
    __uint8_t shtc3_sleep[2] = {0xb0, 0x98};
    __uint8_t shtc3_read_CSTF[2] = {0x7c, 0xa2};
    __uint8_t shtc3_read_NORMTF[2] = {0x78, 0x66};

    int file;
    int adapter_nr = 1; // i2c1 - gpio pins 3(SDA), 5(SCL)
    char filename[20];
    __uint8_t buf[6];
    __uint16_t t_raw, h_raw;
    float temperature, humidity;
    char pub_str[64];

    // register mqtt client
    mqtt_register();

    // set i2c adapter device file
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    file = open(filename, O_RDWR);
    
    #ifdef DEBUG
    if (file < 0) {
        printf("i2c file error");
        exit(1);
    }
    #endif

    // set slave addr
    signed int res = ioctl(file, I2C_SLAVE, SHTC3_I2C_ADDR);
    #ifdef DEBUG
    if(res < 0){
        printf("ioctl error");
        exit(1);
    }
    #endif

    while(1){
        // wake
        res = write(file, shtc3_wake, WRITE_SIZE);
        #ifdef DEBUG
        if(res != WRITE_SIZE){
            printf("wakeup failure");
            exit(1);
        }else{
            printf("waking...");
        }
        #endif
        usleep(300);

        // measure
        res = write(file, shtc3_read_NORMTF, WRITE_SIZE);
        #ifdef DEBUG
        if(res != WRITE_SIZE){
            printf("measure cmd failure");
            exit(1);
        }else{
            printf("measuring...");
            sleep(1);
        }
        #else
        usleep(13000);
        #endif

        // read temp and humidity
        res = read(file, buf, READ_SIZE);
        #ifdef DEBUG
        if(res != READ_SIZE){
            printf("read cmd failure");
            exit(1);
        }else{
            printf("reading...");
            sleep(1);
        }
        #endif

        // sleep
        res = write(file, shtc3_sleep, WRITE_SIZE);
        #ifdef DEBUG
        if(res != WRITE_SIZE){
            printf("sleep failure");
            exit(1);
        }else{
            printf("sleeping...");
            sleep(1);
        }
        #endif
        
        t_raw = (buf[0] << 8) | buf[1];
        h_raw = (buf[3] << 8) | buf[4];
        
        temperature = -45.0f + 175.0f * ((float)t_raw / 65535.0f);
        humidity = 100.0f * ((float)h_raw / 65535.0f);

        // publish data in JSON format
        snprintf(pub_str, sizeof(pub_str), "{\"Temperature\": %.2f}", temperature); 
        mqtt_publish(pub_str, TOPIC_TEMP);
        snprintf(pub_str, sizeof(pub_str), "{\"Humidity\": %.2f}", humidity); 
        mqtt_publish(pub_str, TOPIC_HUM);

        // routed to journalctl
        printf("Temp read: %.1f C\n", temperature);
        printf("Humidity read: %.1f \n", humidity);

        sleep(1);
    }
}
