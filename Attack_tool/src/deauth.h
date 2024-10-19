#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "packetSniffer.h"

typedef struct{
    uint8_t mac[6];
} __attribute__((packed)) mac_addr;


typedef struct {



} __attribute__((packed)) dauthPacket;

void sendDeauthPacket();