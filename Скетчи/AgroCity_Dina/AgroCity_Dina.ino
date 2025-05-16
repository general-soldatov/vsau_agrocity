/*
******************************************************************************
  Описание:
  Скетч для управления Динамикой М1 при помощи ИК-пульта, 
  движение происходит только при удержании кнопки. 
  При помощи кнопок 2, 8, 4, 6 можно осуществить движение вперед, назад, поворот влево и вправо соответственно. 
  На "+" и "-" изменяется максимальная мощность моторов
  Ядро mgbot: 1.0.1
  Created by Vlasov Timofey
  //**************************************************************************
*/

//Подключение библиотек
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <IRremote.h>  // библиотека для пультов ДУа

// Библиотеки для датчика MGS-THP80
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Бибилиотеки для датчика MGS-L75
#include <BH1750.h>

// Библиотеки для датчика MGS-THI50
#include <MCP3221.h>
#include <Adafruit_MCP9808.h>

// Библиотеки для работы телеграм-бота
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


// Настройка портов, каналов, адресов
#define InputCamera_1 5   // Порт камеры сброс
#define InputCamera_2 23  // Порт камеры счетчик

#define Channel_MGS_THP80 0x06  // Датчик температуры, влажности воздуха и атм. давления
#define Channel_MGS_L75 0x07    // датчик освещенности
#define Channel_MGS_THI50 0x05  // датчик температуры, влажности почвы

// Адрес датчика можно узнат при помощи I2C сканера - https://gitverse.ru/MGBot/Codes/content/master/I2C%20scanner/I2C_scanner.ino
#define Addres_MGS_THI50 0x4E  // адрес датчик температуры, влажности почвы 


// Калибровка датчика температуры, влажности почвы
#define HumSoilMax 2248  // Максимальное значение
#define HumSoilMin 1165  // Минимальное значение

// Настройка параметров для Телеграм-бота
#define WIFI_SSID ""                                            // Логин WiFi-сети
#define WIFI_PASSWORD ""                                    // Пароль от WiFi-сети
#define BOT_TOKEN ""  // Токен Telegram-бота


Adafruit_BME280 bme280;                                       // Объект для работы с датчиком MGS-THP80
BH1750 lightMeter;                                            // Объект для работы с датчиком MGS-L75
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x70);  // адрес зависит от перемычек на плате (также попробуйте просканировать адрес: https://github.com/MAKblC/Codes/tree/master/I2C%20scanner)
IRrecv irrecv(27);                                            // пин 27 для ИК-приемника
decode_results results;                                       // переменная для принимаемого сигнал
MCP3221 mcp3221_5(Addres_MGS_THI50);                          // Объект для работы с датчиком MGS-THI50
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();             // Сенсор температуры на датчике MGS-THI50
WiFiClientSecure secured_client;                              // Натсройка шифрования для телеграм-бота
UniversalTelegramBot bot(BOT_TOKEN, secured_client);          // Объект для работы с телеграм-ботом
TaskHandle_t Task1;                                           // Объект для работы с ядром номер 1
TaskHandle_t Task2;                                           // Объект для работы с ядром номер 2


// Системные переменные
const unsigned long BOT_MTBS = 1000;    // частота опроса телеграм-бота (НЕ СТАВИТЬ МЕНЕЕ 100 мс!!!)
int IndexMGBCAM;                        // номер объекта, определяемого камерой MGB-CAM
int BufferMGBCAM;                       //Буфер для счетчика номера объекта с камеры MGB-CAM
byte direction;                         // направление движения Динамики
int MAXPOWERMOTOR = 70;                 // Максималдьная мощность по умолчанию
float lux;                              // Датчик освещенности
float Temperature, Humidity, Pressure;  // Датчик температуры, влажности воздуха и атм. давления
float TempSoil, HumSoil;                // Датчик температуры, влажности почвы


// Функция счетчика для модуля камеры MGB-CAM
void IncrementIndex() {
  BufferMGBCAM++;
}

// Сброс счетчика для модуля камеры MGB-CAM
void ResetIndex() {
  IndexMGBCAM = BufferMGBCAM;
  BufferMGBCAM = 0;
}

/* 
Настройка передвижения динамики
закоментируйте строчку, чтобы отключить не использовать инвертацию
*/

// #define INVERTFORWARD  // Инвертировать движение вперед (если по нажатию 2 динамика едет назад)
// #define INVERTTURN     // Инвертировать повороты (если при повороте динамика поворачивает в другую сторону)


/*
============================КНОПКИ УПРАВЛЕНИЯ============================
  вперед (2) - FF18E7
  назад (8) -  FF4AB5
  влево (4) -  FF10EF
  вправо (6)-  FF5AA5
  удержание -  FFFFFFFF
  добавить мощность (+) -  FFA857
  убавить мощность (-)  -  FFE01F
*/



void setup() {
  // Инициализация последовательного порта
  Serial.begin(115200);
  pinMode(InputCamera_1, INPUT);
  pinMode(InputCamera_2, INPUT);
  irrecv.enableIRIn();  // запускаем прием
  // Инициализация драйвера
  Wire.begin();

  pwm.begin();
  // Частота (Гц)
  pwm.setPWMFreq(100);
  // Все порты выключены
  pwm.setPWM(8, 0, 4096);
  pwm.setPWM(9, 0, 4096);
  pwm.setPWM(10, 0, 4096);
  pwm.setPWM(11, 0, 4096);


  //Инициализация датчика MGS-THI50
  setBusChannel(Channel_MGS_THI50);
  mcp3221_5.setAlpha(DEFAULT_ALPHA);
  mcp3221_5.setNumSamples(DEFAULT_NUM_SAMPLES);
  mcp3221_5.setSmoothing(ROLLING_AVG);

  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808!");
    Serial.print("Возможные ошибки: ");
    Serial.println("505");
    Serial.println("504");
  }
  tempsensor.setResolution(3);

  //Инициализация датчика MGS-THP80
  setBusChannel(Channel_MGS_THP80);
  bool bme_status = bme280.begin();
  if (!bme_status) {
    Serial.println("Не найден по адресу 0х77, пробую другой...");
    bme_status = bme280.begin(0x76);
    if (!bme_status)
      Serial.println("Датчик не найден, проверьте соединение");
    Serial.print("Возможные ошибки: ");
    Serial.println("505");
  }

  //Инициализация датчика MGS-L75
  setBusChannel(Channel_MGS_L75);
  if (lightMeter.begin())
    Serial.println("Успешное подключение датчика освещенности (MGS-L75)!");
  else {
    Serial.println("Ошибка подключения датчика освещенности (MGS-L75) :(");
    Serial.print("Возможные ошибки: ");
    Serial.println("505");
  }

  //Настройка прерываний для модуля камеры MGB-CAM
  attachInterrupt(InputCamera_1, ResetIndex, RISING);
  attachInterrupt(InputCamera_2, IncrementIndex, RISING);


  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                 // Подключение к WiFi-сети
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Добавление корневого сертификата для api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println("Здесь должен быть ваша функция для вывода IP адреса");

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);


  xTaskCreatePinnedToCore(
    Task1code,
    "Task1",
    30000,
    NULL,
    1,
    &Task1,
    0);
  delay(500);

  //Создаем задачу, которая будет выполняться на ядре 1 с наивысшим приоритетом (1)
  xTaskCreatePinnedToCore(
    Task2code,
    "Task2",
    30000,
    NULL,
    1,
    &Task2,
    1);
  delay(500);
}

/*
  1 - вперед
  2 - назад
  3 - влево
  4 - вправо
  0 - стоп
*/


/*
Рекомендации по отображению значений температуры, влажности и освещения
Сообщение формируется и выводится пользователю лишь в тех случаях, когда значение конкретного параметра отличается от установленного эталонного значения более чем на 5% для температуры, на 16% для влажности, на 25% для освещенности.  

Например, если заданная температура составляет 25°C, а фактическое измеренное значение достигает отметки в 26°C 
(отклонение превышает допустимые пределы), пользователю будет показано соответствующее уведомление. В противном случае, 
при соблюдении нормальных условий, пользователю будет демонстрироваться стандартная фраза:  
«Показатель в норме.»
*/

String RecomendationsPlants[10][3] = {
  { "Рекомендация для 🌼 по температуре", "Рекомендация для 🌼 по влажности", "Рекомендация для 🌼 по освещенности" },                                                                                          // Японская роза
  { "Рекомендация для 🍀 по влажности", "Рекомендация для 🍀 по влажности", "Рекомендация для 🍀 по освещенности" },                                                                                            // Хризантема
  { "Рекомендация для 🪻 по влажности", "Рекомендация для 🪻 по влажности", "Рекомендация для 🪻 по освещенности" },  // Шафран
  { "Рекомендация для 🌹 по влажности", "Рекомендация для 🌹 по влажности", "Рекомендация для 🌹 по освещенности" },                                                                                            // Мак
  { "Рекомендация для 🌸 по влажности", "Рекомендация для 🌸 по влажности", "Рекомендация для 🌸 по освещенности" },                                                                                            // Пион
  { "Рекомендация для 🍊 по влажности", "Рекомендация для 🍊 по влажности", "Рекомендация для 🍊 по освещенности" },                                                                                            // Апельсин
  { "Рекомендация для 🌱 по влажности", "Рекомендация для 🌱 по влажности", "Рекомендация для 🌱 по освещенности" },                                                                                            // Недозревшее дерево
  { "Рекомендация для 🍂 по влажности", "Рекомендация для 🍂 по влажности", "Рекомендация для 🍂 по освещенности" },                                                                                            // Засохшее дерево
  { "Рекомендация для 🍋 по влажности", "Рекомендация для 🍋 по влажности", "Рекомендация для 🍋 по освещенности" },                                                                                            // Лимон
  { "Рекомендация для 🍎🍑 по влажности", "Рекомендация для 🍎🍑 по влажности", "Рекомендация для 🍎🍑 по освещенности" }                                                                                          // Гранат/персик
};
int RecomendationPlantsSensors[10][3] = {
  { 25, 80, 200 },  // Японская роза 🌼
  { 25, 80, 200 },  // Хризантема 🍀
  { 25, 80, 200 },  // Шафран 🪻
  { 25, 80, 200 },  // Мак 🌹
  { 25, 80, 200 },  // Пион 🌸
  { 25, 80, 200 },  // Апельсин 🍊
  { 25, 80, 200 },  // Недозревшее дерево 🌱
  { 25, 80, 200 },  // Засохшее дерево 🍂
  { 25, 80, 200 },  // Лимон 🍋
  { 25, 80, 200 }   // Гранат/персик 🍎🍑
};

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    // if(bot.messages[i].text=="/menu"){
    //   bot.deleteMessage(bot.messages[i].chat_id, bot.messages[i].message_id-1);
    // }
    if (bot.messages[i].text == "/start" or bot.messages[i].text == "/menu") {
      String msg = "Привет ";
      msg += bot.messages[i].from_name;
      msg += "!\n";
      msg += "Для взаимодействия с умным роботом-садовником🤖 используй команды ниже \n\n";
      msg += "/GetInfoSensors - Узнать показатели датчиков👀\n";
      msg += "/GetNamePlants - Определить растение перед камерой🌳📸\n";
      msg += "/GetRecomPlants - Определить рекомендации для растения💡\n";
      bot.sendMessage(bot.messages[i].chat_id, msg, "");
    } else if (bot.messages[i].text == "/GetInfoSensors") {
      String msg = "Показатели датчиков\n";
      msg += "☀️Освещенность: " + String(lux, 2) + " lx\n";
      msg += "🌡️Температура: " + String(Temperature, 2) + " °C\n";
      msg += "💧Влажность: " + String(Humidity, 2) + " %\n";
      msg += "🗜Атм. давление: " + String(Pressure, 2) + " hPa\n";
      msg += "🌰💧Влажность почвы: " + String(HumSoil, 2) + " %\n";
      msg += "🌰🌡️Температура почвы: " + String(TempSoil, 2) + " °С\n\n";
      msg += "☰Вернуться обратно - /menu";
      bot.sendMessage(bot.messages[i].chat_id, msg, "");
    } else if (bot.messages[i].text == "/GetNamePlants") {
      String msg = "Перед роботом расположено - ";
      switch (IndexMGBCAM) {
        case 1:
          msg += "Японская роза🌼";
          break;
        case 2:
          msg += "Хризантемам🍀";
          break;
        case 3:
          msg += "Шафран🪻";
          break;
        case 4:
          msg += "Мак🌹";
          break;
        case 5:
          msg += "Пион🌸";
          break;
        case 6:
          msg += "Апельсин🍊";
          break;
        case 7:
          msg += "Недозревшее дерево🌱";
          break;
        case 8:
          msg += "Засохшее дерево🍂";
          break;
        case 9:
          msg += "Лимон🍋";
          break;
        default:
          msg += "🚨Некорретное значение🚨";
          msg += "Ошибка 503\n";
      }
      msg += "\n☰Вернуться обратно - /menu";
      bot.sendMessage(bot.messages[i].chat_id, msg, "");
    } else if (bot.messages[i].text == "/GetRecomPlants") {
      String msg = "Рекомендация для растения - ";
      switch (IndexMGBCAM) {
        case 1:
          msg += "Японская роза🌼";
          break;
        case 2:
          msg += "Хризантемам🍀";
          break;
        case 3:
          msg += "Шафран🪻";
          break;
        case 4:
          msg += "Мак🌹";
          break;
        case 5:
          msg += "Пион🌸";
          break;
        case 6:
          msg += "Апельсин🍊";
          break;
        case 7:
          msg += "Недозревшее дерево🌱";
          break;
        case 8:
          msg += "Засохшее дерево🍂";
          break;
        case 9:
          msg += "Лимон🍋";
          break;
        default:
          msg += "🚨Некорретное значение🚨\n";
          msg += "Ошибка 503\n";
          IndexMGBCAM = 9;  // Ограничение (от багов и паники ядра)
      }
      msg += "\n";

      //Рекомендация по температуре
      msg += "Рекомендация по температуре: ";
      if (Temperature > RecomendationPlantsSensors[IndexMGBCAM][0] * 1.04 or Temperature < RecomendationPlantsSensors[IndexMGBCAM][0] * 0.96) {
        msg += RecomendationsPlants[IndexMGBCAM][0];
      } else {
        msg += "температура в пределах нормы";
      }
      msg += "\n\n";

      //Рекомендация по влажности
      msg += "Рекомендация по влажности: ";
      if (Humidity > RecomendationPlantsSensors[IndexMGBCAM][1] * 1.16 or Humidity < RecomendationPlantsSensors[IndexMGBCAM][1] * 0.84) {
        msg += RecomendationsPlants[IndexMGBCAM][1];
      } else {
        msg += "Влажность в пределах нормы";
      }
      msg += "\n\n";

      //Рекомендация по температуре
      msg += "Рекомендация по Освещенности: ";
      if (lux > RecomendationPlantsSensors[IndexMGBCAM][2] * 1.25 or lux < RecomendationPlantsSensors[IndexMGBCAM][2] * 0.75) {
        msg += RecomendationsPlants[IndexMGBCAM][2];
      } else {
        msg += "Освещенность в пределах нормы";
      }
      msg += "\n\n";

      msg += "☰Вернуться обратно - /menu";

      bot.sendMessage(bot.messages[i].chat_id, msg, "");
    } else {
      bot.sendMessage(bot.messages[i].chat_id, "🚨Неизвестная команда!🚨\n☰Вернуться обратно - /menu", "");
    }
  }
}

void Task1code(void* pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    delay(500);
    // Вывод измеренных значений в терминал
    Serial.println("Температура почвы: " + String(TempSoil, 1) + " °С");
    Serial.println("Влажность почвы: " + String(HumSoil, 1) + " %");
    Serial.println("Освещенность: " + String(lux, 1) + " lx");
    Serial.println("Температура воздуха: " + String(Temperature, 1) + " °C");
    Serial.println("Влажность: " + String(Humidity, 1) + " %");
    Serial.println("Атм. давление: " + String(Pressure, 1) + " hPa");  // 1 mmHg = 1 hPa / 1.33

    static uint32_t bot_lasttime;
    if (millis() - bot_lasttime > BOT_MTBS) {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

      while (numNewMessages) {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }

      bot_lasttime = millis();
    }
  }
}


void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    delay(10);
    setBusChannel(Channel_MGS_THI50);
    tempsensor.wake();
    TempSoil = tempsensor.readTempC();
    HumSoil = mcp3221_5.getData();
    HumSoil = map(HumSoil, HumSoilMax, HumSoilMin, 0, 100);

    setBusChannel(Channel_MGS_THP80);
    Temperature = bme280.readTemperature();
    Humidity = bme280.readHumidity();
    Pressure = bme280.readPressure() / 100.0F;

    setBusChannel(Channel_MGS_L75);
    lux = lightMeter.readLightLevel();

    static uint32_t durationTimer;
    if (irrecv.decode(&results)) {         // если данные пришли
      Serial.println(results.value, HEX);  // выводим в порт полученное значение
      irrecv.resume();                     // принимаем следующую команду
      durationTimer = millis();            // обновляем таймер
      switch (results.value) {             // Проверяем какая кнопка нажата и задаем направление движения
        case 0xFF18E7:
          direction = 1;
          break;
        case 0xFF4AB5:
          direction = 2;
          break;
        case 0xFF10EF:
          direction = 3;
          break;
        case 0xFF5AA5:
          direction = 4;
          break;
        case 0xFFFFFFFF:
          direction = direction;
          break;
        case 0xFFA857:  // Если нажат +, то добавляем мощность на 10%
          MAXPOWERMOTOR = constrain(MAXPOWERMOTOR + 10, 0, 100);
          Serial.println("Максимальная мощность моторов изменена на: " + String(MAXPOWERMOTOR) + "%");
          break;
        case 0xFFE01F:  // Если нажат -, то добавляем снижаем на 10%
          MAXPOWERMOTOR = constrain(MAXPOWERMOTOR - 10, 0, 100);
          Serial.println("Максимальная мощность моторов изменена на: " + String(MAXPOWERMOTOR) + "%");
          break;
      }
    }
    if (millis() - durationTimer >= 170 && direction != 0) {  // проверяем если ИК порт ничего не принял за 170 мс, значит на пульте нет нажатых кнопок, останавливаем резко моторы
      switch (direction) {
        case 1:
          motorA_setpower(-20, false);
          motorB_setpower(-20, true);
          break;
        case 2:
          motorA_setpower(20, false);
          motorB_setpower(20, true);
          break;
        case 3:
#if defined(INVERTTURN)
          motorA_setpower(-20, false);
          motorB_setpower(20, true);
#else
          motorA_setpower(20, false);
          motorB_setpower(-20, true);
#endif
          break;
        case 4:
#if defined(INVERTTURN)
          motorA_setpower(20, false);
          motorB_setpower(-20, true);
#else
          motorA_setpower(-20, false);
          motorB_setpower(20, true);
#endif
          break;
      }
      delay(50);
      motorA_setpower(0, false);
      motorB_setpower(0, true);
      direction = 0;
    }

    switch (direction) {  // Передвигаемся по нарпавлению движения
      case 1:
        motorA_setpower(100, false);
        motorB_setpower(100, true);
        break;
      case 2:
        motorA_setpower(-100, false);
        motorB_setpower(-100, true);
        break;
      case 3:
#if defined(INVERTTURN)
        motorA_setpower(100, false);
        motorB_setpower(-100, true);
#else
        motorA_setpower(-100, false);
        motorB_setpower(100, true);
#endif
        break;
      case 4:
#if defined(INVERTTURN)
        motorA_setpower(-100, false);
        motorB_setpower(100, true);
#else
        motorA_setpower(100, false);
        motorB_setpower(-100, true);
#endif
        break;
      case 0:
        motorA_setpower(0, false);
        motorB_setpower(0, true);
        break;
    }
  }
}



void loop() {
}

// Мощность мотора "A" от -100% до +100% (от знака зависит направление вращения)
void motorA_setpower(float pwr, bool invert) {
  pwr = map(pwr, -100, 100, -MAXPOWERMOTOR, MAXPOWERMOTOR);  // Установка изменяемой мощности мотора
// Проверка, инвертирован ли мотор
#if defined(INVERTFORWARD)
  if (!invert) {
    pwr = -pwr;
  }
#else
  if (invert) {
    pwr = -pwr;
  }
#endif

  // Проверка диапазонов
  if (pwr < -100) {
    pwr = -100;
  }
  if (pwr > 100) {
    pwr = 100;
  }
  int pwmvalue = fabs(pwr) * 40.95;
  if (pwr < 0) {
    pwm.setPWM(10, 0, 4096);
    pwm.setPWM(11, 0, pwmvalue);
  } else {
    pwm.setPWM(11, 0, 4096);
    pwm.setPWM(10, 0, pwmvalue);
  }
}

// Мощность мотора "B" от -100% до +100% (от знака зависит направление вращения)
void motorB_setpower(float pwr, bool invert) {
  pwr = map(pwr, -100, 100, -MAXPOWERMOTOR, MAXPOWERMOTOR);  // Установка изменяемой мощности мотора
// Проверка, инвертирован ли мотор
#if defined(INVERTFORWARD)
  if (!invert) {
    pwr = -pwr;
  }
#else
  if (invert) {
    pwr = -pwr;
  }
#endif
  // Проверка диапазонов
  if (pwr < -100) {
    pwr = -100;
  }
  if (pwr > 100) {
    pwr = 100;
  }
  int pwmvalue = fabs(pwr) * 40.95;
  if (pwr < 0) {
    pwm.setPWM(8, 0, 4096);
    pwm.setPWM(9, 0, pwmvalue);
  } else {
    pwm.setPWM(9, 0, 4096);
    pwm.setPWM(8, 0, pwmvalue);
  }
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
