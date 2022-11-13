#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

String valor;

int freq = 0 ;
int channel = 0;
int resolution = 8;
const int buzz=15;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        valor = "";
        for (int i = 0; i < value.length(); i++){
          // Serial.print(value[i]);
          valor = valor + value[i];
        }

        Serial.println("*********");
        Serial.print("Read = ");
        Serial.println(valor);
        Serial.println(value.length());
        if (value.length() < 3){
         freq = 2000;
         ledcWriteTone(channel, freq);
         Serial.println("Far");
        }else{
         freq = 0;
         ledcWriteTone(channel, freq);
         Serial.println("Near");
        }
        }
      }
};

void setup() {
  Serial.begin(115200);
  
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(15, channel);
  
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  delay(2000);
}
