
#include <SPI.h>
#include <IRremote.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9
#define KEY_ON     0x4FB4AB5    //вкл выкл ТВ

#define START A0
#define RFID_READY A1
#define IR_SEND A2
#define IR_OK 6
#define RFID_OK 7

RFID rfid(SS_PIN, RST_PIN);

// Данные о номере карты храняться в 5 переменных, будем запоминать их, чтобы проверять, считывали ли мы уже такую карту
int serNum0 = 249;
int serNum1 = 241;
int serNum2 = 74;
int serNum3 = 99;
int serNum4 = 33;

// Переменные о включении доступа
boolean readCard = false;

// переменные для ИК порта
boolean sendIR = false;
IRsend irsend;

void setup() {
  //инициализируем  порт для чтения карт
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  // Инициализируем ИК порт
  pinMode(START, INPUT);
  pinMode(RFID_READY, INPUT);
  pinMode(IR_SEND, INPUT);
  pinMode(IR_OK, OUTPUT);
  pinMode(RFID_OK, OUTPUT);

  digitalWrite(RFID_OK, LOW);
  digitalWrite(IR_OK, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (analogRead(START) > 100) {
    readCard = false;
    digitalWrite(RFID_OK, LOW);
    digitalWrite(IR_OK, LOW);
    sendIR = false;
    Serial.println("старт");
  }

  if ((analogRead(RFID_READY) > 100) && !readCard) {
    Serial.println(" читаем карту");
    if (rfid.isCard()) {
      if (rfid.readCardSerial()) { // Сравниваем номер карты с номером предыдущей карты
        if (rfid.serNum[0] == serNum0
            && rfid.serNum[1] == serNum1
            && rfid.serNum[2] == serNum2
            && rfid.serNum[3] == serNum3
            && rfid.serNum[4] == serNum4
           ) {
          /* Отправляем на пин распберри что карта та что нужна */
          Serial.println(" Карта верная и считана!");
          readCard = true;
          digitalWrite(RFID_OK, HIGH);
        }
      }
    }

    rfid.halt();
  }
  if ((analogRead(IR_SEND) > 100) && !sendIR) {
    Serial.println( " включаем ТВ" ); // печатаем данные
    sendIR = true;
    irsend.sendNEC(KEY_ON, 32);
    delay(50);
    irsend.sendNEC(KEY_ON, 32);
    delay(100);
    Serial.println("Отправлено");
    digitalWrite(IR_OK, HIGH);
  }

}
