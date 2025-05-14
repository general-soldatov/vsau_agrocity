#include <TypePlants_inferencing.h>

#include <TypePlants_inferencing.h>

#include <Wire.h>
#include <MCP3221.h>
#include <Adafruit_MCP9808.h>

MCP3221 mcp3221_5(0x4E);  // Адрес может отличаться

// Калибровочные данные
int a = 2248;  // масштабирование значений
int b = 1165;
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setBusChannel(0x05);
  mcp3221_5.setAlpha(DEFAULT_ALPHA);
  mcp3221_5.setNumSamples(DEFAULT_NUM_SAMPLES);
  mcp3221_5.setSmoothing(ROLLING_AVG);

  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808!");
    while (1)
      ;
  }
  tempsensor.setResolution(3);
  for (int i = 0; i <= 50; i++) { // настройка датчика
    tempsensor.readTempC();
    mcp3221_5.getData();
    delay(50);
  }
}

void loop() {
  setBusChannel(0x05);
  tempsensor.wake();
  static int MaximalValue = 0;
  static int MinimalValue = 4096;
  float t1 = tempsensor.readTempC();
  float h1 = mcp3221_5.getData();
  if (MaximalValue < h1)
    MaximalValue = h1;
  if (MinimalValue > h1)
    MinimalValue = h1;
  Serial.println("Максимальное значение: " + String(MaximalValue));
  Serial.println("Минимальное значение: " + String(MinimalValue));
  
  h1 = map(h1, MinimalValue, MaximalValue, 0, 100);
  Serial.println("Влажность: " + String(h1));
  Serial.println("----------------------");
  // Serial.println("Температура: " + String(t1));
  delay(500);
}



// Константы для I2C шилда
#define I2C_HUB_ADDR 0x70
#define EN_MASK 0x08
#define DEF_CHANNEL 0x00
#define MAX_CHANNEL 0x08

/*
  I2C порт 0x07 - выводы GP16 (SDA), GP17 (SCL)
  I2C порт 0x06 - выводы GP4 (SDA), GP13 (SCL)
  I2C порт 0x05 - выводы GP14 (SDA), GP15 (SCL)
  I2C порт 0x04 - выводы GP5 (SDA), GP23 (SCL)
  I2C порт 0x03 - выводы GP18 (SDA), GP19 (SCL)
*/


// Функция установки нужного выхода I2C
bool setBusChannel(uint8_t i2c_channel) {
  if (i2c_channel >= MAX_CHANNEL) {
    return false;
  } else {
    Wire.beginTransmission(I2C_HUB_ADDR);
    Wire.write(i2c_channel | EN_MASK);  // для микросхемы PCA9547
    // Wire.write(0x01 << i2c_channel); // Для микросхемы PW548A
    Wire.endTransmission();
    return true;
  }
}
