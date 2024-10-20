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
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_netif_deinit());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    
    return bestrec; // else return the open 1.
}

void printMac(uint8_t* srcmac, uint8_t* dstmac, uint8_t subtype)
{
    printf("src: %2x:%2x:%2x:%2x:%2x:%2x\n", srcmac[0],srcmac[1],
                                             srcmac[2],srcmac[3],
                                             srcmac[4],srcmac[5]);

    printf("dst: %2x:%2x:%2x:%2x:%2x:%2x\n", dstmac[0],dstmac[1],
                                             dstmac[2],dstmac[3],
                                             dstmac[4],dstmac[5]);
    printf("subtype: %x\n", subtype);
                                            
}

void handlePromPackets(void *buf, wifi_promiscuous_pkt_type_t type)
{
    if(type != WIFI_PKT_MGMT){ return;} // only data packet metter.

    wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
    wifi_mac_header_t* machdr = (wifi_mac_header_t*)pkt->payload; 
    wifi_frame_control_t ctrl;
    ctrl.frame_control_value = machdr->frame_control;
    
    printf("FOUND PACKET\n");
    printMac(machdr->addr2, machdr->addr1, machdr->frame_control);
    


    /*logRawPacket(pkt->payload, pkt->rx_ctrl.sig_len);

    wifi_mac_header_t* machdr = (wifi_mac_header_t*)pkt->payload; // get init mac header

    ethernet_header_t* eth_header;

    // get control frame in struct format to check for fourth address
    wifi_frame_control_t frame_control;
    frame_control.frame_control_value = machdr->frame_control;


    int sizeCurr = 0;

    if (frame_control.type == 0b00 && frame_control.subtype == 0b0001) { // Data frame
        sizeCurr += sizeof(qos_control_t); // Add QoS Control size if applicable
    }

    if(frame_control.to_from_ds == 0b11) // if this field is 11 that means there is fourth address. this field also defines which source and dst (or ap src dst).
    {
        sizeCurr += sizeof(wifi_mac_header_full_t); 
        eth_header = (ethernet_header_t*)(pkt->payload + sizeCurr);
    }
    else{
        sizeCurr += sizeof(wifi_mac_header_t); 
        eth_header = (ethernet_header_t*)(pkt->payload + sizeCurr);
    }

    if (pkt->rx_ctrl.sig_len < sizeCurr + sizeof(ethernet_header_t)) {
        printf("Invalid packet length\n");
        return;
    }

    
    printf("packet:\n");
    printMac(eth_header->src_mac, eth_header->dest_mac);
    printMac(machdr->addr2, machdr->addr1);
    printf("-------------\n");
    */

    /*printf("ethertype: %x\n", __ntohs(eth_header->ethertype));
    if(eth_header->ethertype == 0x0800)
    {
        printMac(machdr->addr2,machdr->addr1);
        printf("IP");
    }
    else if(eth_header->ethertype == 0x0806)
    {
        printMac(eth_header->src_mac, eth_header->dest_mac);
        printf("ARP");
    }else
    {
        puts("NOT");
    }*/
    
    //printMac(machdr->addr2,machdr->addr1);
    
}

void logRawPacket(const void *buf, size_t len) {
    const uint8_t *data = (const uint8_t *)buf;
    printf("Raw Packet: ");
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}


void settingupPromiscuousMode()
{
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL)); // wifi mode is null beacuse we are not connecting or ap.
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));


    ESP_ERROR_CHECK(esp_wifi_start());


    esp_wifi_set_promiscuous(false);
    //set up handler for promiscous

    wifi_promiscuous_cb_t cb = (wifi_promiscuous_cb_t)handlePromPackets;
    esp_wifi_set_promiscuous_rx_cb(cb);

    /*wifi_promiscuous_filter_t filter;
    filter.filter_mask = WIFI_PROMIS_FILTER_MASK_ALL;
    esp_wifi_set_promiscuous_filter(&filter);*/

    esp_wifi_set_promiscuous(true);

    //xTaskCreate(channel_hopper , "channel_hopp", 8192, NULL, 10, NULL);

}
void channel_hopper(void *param)
{
    uint8_t channel = 1;
    while (true) {
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
        channel = (channel % 13) + 1;  // Hop between channels 1-13
        vTaskDelay(500 / portTICK_PERIOD_MS);  // Switch every 500ms
    }
}
