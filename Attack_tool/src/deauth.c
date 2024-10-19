#include "deauth.h"

void sendDeauthPacket()
{
    wifi_ap_record_t rec = showNearbyNetworks();

    // testing only on my local network.
    if(strcmp(rec.ssid, "dima570") !=){
        return;
    }

    

    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_STA, ));
}