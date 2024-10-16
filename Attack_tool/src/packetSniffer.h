#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

wifi_ap_record_t showNearbyNetworks();
char* get_auth_mode(wifi_auth_mode_t mode);

void settingupPromiscuousMode();
void handlePromPackets(void *buf, wifi_promiscuous_pkt_type_t type);