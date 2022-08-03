/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Plataforma ESP32
 * Proyecto Admin Panel Tool para el ESP32
 * -------------------------------------------------------------------
*/

// -------------------------------------------------------------------
// Librerias
// -------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// -------------------------------------------------------------------
// Archivos *.hpp - Fragmentar el CÃ³digo
// -------------------------------------------------------------------
#include "settings.hpp"
#include "functions.hpp"
#include "settingsReset.hpp"
#include "settingsRead.hpp"
#include "settingsSave.hpp"
#include "esp32_wifi.hpp"
#include "esp32_mqtt.hpp"

// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void setup() {
    // Baudrate
    Serial.begin(115200);
    // CPU Freq
    setCpuFrequencyMhz(240);
    // Inicio del Log por serial
    log("\nInfo: Iniciando Setup");
    // Configurar los Pines
    settingPines();
    // Inicio del SPIFFS                 
    if (!SPIFFS.begin(true)){               //IMPORTANTE PONER EL true 
        log(F("Error: Falló la inicializaciÓn del SPIFFS"));
        while (true);
    }
    //lee los estados de los relays
    settingsReadRelays();
    //paso estado a los pines Relays
    setOnOffSingle(RELAY1, Relay01_status);
    setOnOffSingle(RELAY2, Relay02_status);
    // Lee la Configuración WiFi
    settingsReadWiFi();
    // Configuracion WIFI
    WiFi.disconnect(true);
    delay(1000);
    // Setup del WiFI
    wifi_setup(); 
    // Lee la Configuración mqtt
    settingsReadMQTT();
}


// -------------------------------------------------------------------
// Loop Pincipal Nucleo 0
// -------------------------------------------------------------------
void loop() {

    yield();                                //cuando el loop tarda mucho se recomienda poner yield
                                            //para que el watch dog no se vuelva loco
    // -------------------------------------------------------------------
    // WIFI
    // -------------------------------------------------------------------
    if (wifi_mode == WIFI_STA){             //if para el modo de parpadeo en STA o AP
        wifiLoop();
    }else if (wifi_mode == WIFI_AP){
        wifiAPLoop();
    } 
// -------------------------------------------------------------------
    // MQTT
    // -------------------------------------------------------------------
    if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA)){
        if(mqtt_server != 0){
            mqttLoop();
            if (mqttclient.connected()){
                if (millis() - lastMsg > mqtt_time){
                    lastMsg = millis();
                    mqtt_publish();
                }
            }      
        }
    }


}