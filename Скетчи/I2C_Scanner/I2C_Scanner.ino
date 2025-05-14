/*
Данный код самостоятельно определяет и выводит тип датчика, который подключен к какому каналу он подключен, его адрес.  
Обязательно подключите одно из устройств к аппаратному I2C (21 - SDA; 22 - SCL) 
Устройства с I2C можно подключать в программные шины (желтые разъемы), если у вас есть плата MGB-I2C63
*/
#include <Wire.h>
#include <MGB_I2C63.h>
MGB_I2C63 mgb_i2c63 = MGB_I2C63(true);
int vals[] = { 0, 7, 6, 5, 4, 3 };  // номера шин
String wires[] = { "21/22", "16/17", "4/13", "14/15", "5/23", "19/18" }; // названия шин

const byte AddrDevices[][16] = {
  { 0x60, 0x61 },                                                                                      // MGB-BUZ1
  { 0x70 },                                                                                            // MGB-I2C63EN
  { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 },                                                  // MGB-LCD12864
  { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F },  // MGB-MDYN1.1
  { 0x4D },                                                                                            // MGB-RGB2
  { 0x1C, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18 },                                                        // MGB-RGB3
  { 0x41 },                                                                                            // MGR-2I2C
  { 0x41 },                                                                                            // MGR-4
  { 0x69, 0x68 },                                                                                      // MGS-A6
  { 0x39 },                                                                                            // MGS-CLM60
  { 0x58 },                                                                                            // MGS-CO30
  { 0x29 },                                                                                            // MGS-D20
  { 0x39 },                                                                                            // MGS-FR403
  { 0x4E },                                                                                            // MGS-GUVA
  { 0x23 },                                                                                            // MGS-L75
  { 0x3F },                                                                                            // MGS-LN19A
  { 0x4D },                                                                                            // MGS-SND504
  { 0x4A },                                                                                            // MGS-THI50EN
  { 0x77, 0x76 },                                                                                      // MGS-THP80
  { 0x10 },                                                                                            // MGS-UV60
  { 0x48 },                                                                                            // MGS-WT1
  { 0x48 }                                                                                             // MGS-D1015
};

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\nI2C Сканер");
  Serial.println("Количество устройств записано: " + String(1 + sizeof(AddrDevices) / sizeof(AddrDevices[0])));
  Serial.println(String(0x07, DEC));
}

void loop() {
  byte error, address;
  int nDevices;

  static bool NewDevice[sizeof(AddrDevices) / sizeof(AddrDevices[0])][sizeof(AddrDevices[0]) / sizeof(AddrDevices[0][0])];
  Serial.println("Сканирую...");

  nDevices = 0;

  for (int i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) {
    for (address = 8; address < 127; address++) {
      mgb_i2c63.setBusChannel(vals[i]);
      Wire.beginTransmission(address);
      error = Wire.endTransmission();


      if (error == 0) {
        byte CountFind = 0;
        for (int i = 0; i < sizeof(AddrDevices) / sizeof(AddrDevices[0]); i++) {
          for (int j = 0; j < sizeof(AddrDevices[0]) / sizeof(AddrDevices[0][0]); j++) {
            if (AddrDevices[i][j] == address) {
              CountFind++;
            }
          }
        }
        if (i != 0) {
          if (scan(address)) {
            continue;
          }
        }
        Serial.print("На шине " + String(vals[i]) + "(" + wires[i] + ") ");
        if (CountFind > 1) {
          Serial.print("Обнаружен один из модулей под адресом: 0x");
          if (address < 16)
            Serial.print("0");

          Serial.print(address, HEX);

          Serial.print(" ==> ");
        } else
          Serial.print("Обнаружен модуль ");

        for (int i = 0; i < sizeof(AddrDevices) / sizeof(AddrDevices[0]); i++) {
          for (int j = 0; j < sizeof(AddrDevices[0]) / sizeof(AddrDevices[0][0]); j++) {
            if (AddrDevices[i][j] == address) {
              switch (i) {
                case 0: Serial.print("MGB-BUZ1"); break;
                case 1: Serial.print("MGB-I2C63EN"); break;
                case 2: Serial.print("MGB-LCD12864"); break;
                case 3: Serial.print("MGB-MDYN1.1"); break;
                case 4: Serial.print("MGB-RGB2"); break;
                case 5: Serial.print("MGB-RGB3"); break;
                case 6: Serial.print("MGR-2I2C"); break;
                case 7: Serial.print("MGR-4"); break;
                case 8: Serial.print("MGS-A6"); break;
                case 9: Serial.print("MGS-CLM60"); break;
                case 10: Serial.print("MGS-CO30"); break;
                case 11: Serial.print("MGS-D20"); break;
                case 12: Serial.print("MGS-FR403"); break;
                case 13: Serial.print("MGS-GUVA"); break;
                case 14: Serial.print("MGS-L75"); break;
                case 15: Serial.print("MGS-LN19A"); break;
                case 16: Serial.print("MGS-SND504"); break;
                case 17: Serial.print("MGS-THI50EN"); break;
                case 18: Serial.print("MGS-THP80"); break;
                case 19: Serial.print("MGS-UV60"); break;
                case 20: Serial.print("MGS-WT1"); break;
                case 21: Serial.print("MGS-D1015"); break;
              }
              if (CountFind > 1)
                Serial.print("; ");
            }
          }
        }


        if (CountFind == 0) {
          Serial.print(" ****** по адресу: 0x");
          if (address < 16)
            Serial.print("0");
          Serial.print(address, HEX);
          Serial.println(" !");

        } else if (CountFind == 1) {
          Serial.print(" по адресу: 0x");
          if (address < 16)
            Serial.print("0");
          Serial.print(address, HEX);
          Serial.println(" !");

        } else {

          Serial.println("");
        }

        nDevices++;
      } else if (error == 4) {
        Serial.print("Неизвестное устройство по адресу 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
      }
    }
  }
  if (nDevices == 0)
    Serial.println("Не найдено I2C устройств\n");
  else
    Serial.println("готово!\n");

  delay(3000);
}

bool scan(byte ADDR) {
  mgb_i2c63.setBusChannel(0);
  for (byte address1 = 8; address1 < 127; address1++) {
    Wire.beginTransmission(address1);
    byte error1 = Wire.endTransmission();
    if (error1 == 0) {
      if (address1 == ADDR) {

        return true;
      }
    }
  }
  return false;
}
