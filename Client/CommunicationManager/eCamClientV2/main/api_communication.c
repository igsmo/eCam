/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_connection.h"
#include "api_communication.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

static const char *TAG = "API Communication";

#define REQUEST_LENGTH 400
#define WEB_PATH_LENGTH 80


static const char* GET_PREFIX = "GET ";
static const char* PUT_PREFIX = "PUT ";
static const char* HTTP_REQUEST_SUFFIX = " HTTP/1.0\r\n"
            "Host: "WEB_SERVER":"WEB_PORT"\r\n"
            "User-Agent: esp-idf/1.0 esp32\r\n";
static const char* JSON_CONTENT_SUFFIX = "Content-Type: application/json\r\n"
            "Content-Length: ";
                     

char request[REQUEST_LENGTH];
char web_path[WEB_PATH_LENGTH];

/*
 * @brief Task to get an HTTP response.
 * 
 * @param pvParameters Name of the task.
 */
static void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[64];

    // Keeps repeating until there is an answer from the HTTP server
    while(true) {
            
        // Get address info at specified port
        int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);
        
        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Print resolved IP address of the device.
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        // Create socket
        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        // Connect to the socket
        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        // Send request to the socket
        if (write(s, request, strlen(request)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        // Set the timeout threshold 
        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        // Read HTTP response from the socket
        do {
            bzero(recv_buf, sizeof(recv_buf));
            r = read(s, recv_buf, sizeof(recv_buf)-1);
            for(int i = 0; i < r; i++) {
                putchar(recv_buf[i]);
            }
        } while(r > 0);

        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
        close(s);

        // Delete current task
        vTaskDelete(NULL);
        break;
    }
}

void api_get_nexttask(int id) {
    // Resets the request and destination web_path
    memset(request, 0, REQUEST_LENGTH);
    memset(web_path, 0, WEB_PATH_LENGTH);
    
    // Convert int to string. Argument 10 specifies radix.
    char id_str[5];
    itoa(id, id_str, 10); 

    strcpy(request, GET_PREFIX);
    strcpy(web_path, MANAGER_PREFIX_PATH);
    strcat(request, web_path);
    strcat(request, id_str);
    strcat(request, GET_NEXT_TASK_SUFFIX_PATH);
    strcat(request, HTTP_REQUEST_SUFFIX);

    printf(request);

    xTaskCreate(&http_get_task, "http_get_task", 4096, NULL, 5, NULL);
}


void api_update_status(int id, int task_id, int qr_id, float qr_distance) {
    // Resets the request and destination web_path
    memset(request, 0, REQUEST_LENGTH);
    memset(web_path, 0, WEB_PATH_LENGTH);
    
    // Convert int to string. Argument 10 specifies radix.
    char id_str[5];
    itoa(id, id_str, 10); 
    char taskid_str[5];
    itoa(task_id, taskid_str, 10); 
    char qrid_str[5];
    itoa(qr_id, qrid_str, 10); 
    char qrdistance_str[14];
    sprintf(qrdistance_str, "%g", qr_distance);

    // Create a JSON body for status update
    char json_body[200];
    strcpy(json_body, "{");

    strcat(json_body, "\"RobotID\": ");
    strcat(json_body, id_str);
    strcat(json_body, ",\r\n");

    // TaskID, QRID and QR Distance can be null,
    // so it is sufficient to pass -1 to them to set it to null.
    if (task_id >= 0) {
    }
    
    if (qr_id >= 0) {
        strcat(json_body, (char*)"\"QR_ID_InFront\": ");
        strcat(json_body, qrid_str);
        strcat(json_body, ",\r\n");

        // Floats are represented as strings in the database,
        // so we need to add ""
        strcat(json_body, "\"QR_Distance_InFront\": \"");
        strcat(json_body, qrdistance_str);
        strcat(json_body, "\"}");
    }

    int json_size_bytes = strlen(json_body);
    char jsonsize_str[5];
    itoa(json_size_bytes, jsonsize_str, 10); 

    strcpy(request, PUT_PREFIX);
    strcpy(web_path, MANAGER_PREFIX_PATH);
    strcat(request, web_path);
    strcat(request, id_str);
    strcat(request, UPDATE_STATUS_SUFFIX_PATH);
    strcat(request, HTTP_REQUEST_SUFFIX);
    strcat(request, JSON_CONTENT_SUFFIX);
    strcat(request, jsonsize_str);
    strcat(request, "\r\n");
    strcat(request, json_body);
    
    printf(request);

    xTaskCreate(&http_get_task, "http_put_task", 4096, NULL, 5, NULL);
}



