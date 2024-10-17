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
#include "esp_wifi_types.h"

typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];  // Destination MAC
    uint8_t addr2[6];  // Source MAC
    uint8_t addr3[6];  // BSSID or Third Address
    uint16_t sequence_control;
} __attribute__((packed)) wifi_mac_header_t;

typedef struct {
    uint8_t dest_mac[6];     // Destination MAC address
    uint8_t src_mac[6];      // Source MAC address
    uint16_t ethertype;      // Type of payload (0x0800 = IP, 0x0806 = ARP)
} __attribute__((packed)) ethernet_header_t;

typedef struct {
    uint8_t ver_ihl; //version + ihl
    uint8_t tos;
    uint16_t tlen;
    uint16_t identification;
    uint16_t flags_frag; //flags + fragment offset
    uint8_t ttl;
    uint8_t protocol;
    u_int16_t checksum;
    uint8_t src_addr[4];
    uint8_t dst_addr[4];
} __attribute__((packed)) ipv4_header_t;

wifi_ap_record_t showNearbyNetworks();
char* get_auth_mode(wifi_auth_mode_t mode);

void settingupPromiscuousMode();
void handlePromPackets(void *buf, wifi_promiscuous_pkt_type_t type);
void printMac(uint8_t* srcmac, uint8_t* dstmac);
void channel_hopper(void *param);