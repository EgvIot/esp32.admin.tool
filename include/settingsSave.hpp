/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Plataforma ESP32
 * Proyecto Admin Panel Tool para el ESP32
 * -------------------------------------------------------------------
*/
// -------------------------------------------------------------------
// Guardar los estados de los relays
// ------------------------------------------------------------------- 

boolean settingsSaveRelays(){
    // Guarda configuración de los relays
    StaticJsonDocument<500> jsonConfig;
    File file = SPIFFS.open(F("/ConfigRelay.json"), "w+");
    if(file){
        jsonConfig["Relay01_status"] = Relay01_status;
        jsonConfig["Relay02_status"] = Relay02_status;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log(F("Info: Estado de los Relay almacenado"));
        ;
        serializeJsonPretty(jsonConfig, Serial);
        return true;
    }else{
        log(F("Error: fallo el almacenaminto de los Estado de los Relay"));
        return true;
    }
}