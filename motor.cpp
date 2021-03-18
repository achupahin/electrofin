#include <SPI.h>      // библиотека для протокола SPI
#include <nRF24L01.h> // библиотека для nRF24L01+
#include <RF24.h>     // библиотека для радио модуля
#include <Servo.h>

#include "ESC.h"

const uint64_t pipe = 0xF0F1F2F3F4LL; // идентификатор передачи
RF24 radio(6,10); // Для MEGA2560 замените на RF24 radio(9,53);


int speed=0;

// specify the pin here
ESC myESC (9, 1000, 2000, 500); 


void setup() {

    Serial.begin(9600);  // запускаем последовательный порт
    radio.begin();       // включаем радио модуль
    radio.setChannel(0); // выбираем канал (от 0 до 127)

      // скорость: RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
    radio.setDataRate(RF24_1MBPS);
      // мощность: RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM
    radio.setPALevel(RF24_PA_HIGH);

    radio.openReadingPipe(1, pipe);    // открываем первую трубу
    radio.startListening();            // начинаем слушать трубу
    myESC.calib();                  // Calibration of the Max and Min value the ESC is expecting
    myESC.stop();     
    radio.printDetails();  // Вот эта строка напечатает нам что-то, если все правильно соединили.
    // Attaching motor will set pin to OUTPUT and and started making PWM pulse.
    // Since default pulse is 0, no pulse will be generated.  
    

}

void loop() {

  int data,escSpeed;
  
  if (radio.available())             // проверяем буфер обмена
  {
    radio.read(&data, sizeof(data)); // читаем данные    
    speed=data;
    Serial.print("speed: ");
    Serial.println(speed);            // выводим данные на монитор порта    
    escSpeed = map(speed, 0, 1023, 1000, 1500);


    myESC.speed(escSpeed); 
    Serial.print("esc: ");
    Serial.println(escSpeed);
    delay(1000);
  }

 
  

}
