#include "packetSniffer.h"

char* get_auth_mode(wifi_auth_mode_t mode)
{
    char *types[] = {"OPEN", "WEP", "WPA PSK", "WPA2 PSK", "WPA WPA2 PSK", "MAX"};
    return types[mode];
}

wifi_ap_record_t showNearbyNetworks()
{
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    wifi_scan_config_t scncfg = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = true,
    };
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scncfg, true));

    wifi_ap_record_t wfrecords[15];
    
    uint16_t maxrec = 15;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&maxrec, wfrecords));

    wifi_ap_record_t bestrec;
    int found = 0;

    printf("Found %d networks :\n", maxrec);
    for (int i = 0; i < maxrec; i++)
    {
        // print network details
        printf("ssid: %s, authmode: %s\n", wfrecords[i].ssid, get_auth_mode(wfrecords[i].authmode));
        if(strcmp(get_auth_mode(wfrecords[i].authmode), "OPEN") == 0){
            bestrec = wfrecords[i];
            found = 1;
        }
    }
    
    // if didn't found and there are nearcy access points we will just return the first one.
    if(found == 0 && maxrec > 0){
        return wfrecords[0];
    }
    return bestrec; // else return the open 1.
}