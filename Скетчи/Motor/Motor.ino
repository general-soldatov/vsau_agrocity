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

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <IRremote.h>                                         // библиотека для пультов ДУа
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x70);  // адрес зависит от перемычек на плате (также попробуйте просканировать адрес: https://github.com/MAKblC/Codes/tree/master/I2C%20scanner)

IRrecv irrecv(27);       // пин 27 для ИК-приемника
decode_results results;  // переменная для принимаемого сигнал


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
}

/*
  1 - вперед
  2 - назад
  3 - влево
  4 - вправо
  0 - стоп
*/



byte direction;

int MAXPOWERMOTOR = 70;  // Максималдьная мощность по умолчанию

void loop() {
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
