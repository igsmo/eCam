#include "wifi_connection.h"
#include "api_communication.h"

void app_main(void) {
    wifi_connect();
    api_get_nexttask(3);
    api_update_status(3, 2, 1, 5.6);
}