set(embed_files "web/www/index_ov2640.html.gz"
                "web/www/index_ov3660.html.gz"
                "web/www/index_ov5640.html.gz"
                "web/www/monitor.html.gz")

set(src_dirs    
                ai
                camera
                led
                button
                web
                trace
                imu
                QR_Posing)

set(include_dirs    
                    ai
                    camera
                    led
                    button
                    web
                    trace
                    imu
                    QR_Posing)

set(requires    esp32-camera
                esp-dl
                bus
                screen
                esp_http_server
                nvs_flash
                mdns
                esp_adc_cal
                fb_gfx
                esp-code-scanner)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires} EMBED_FILES ${embed_files})

component_compile_options(-ffast-math -O3)
