#include <Arduino.h>
#include <ResponsiveAnalogRead.h>

#define CLK 4
#define DATA 3

#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку для работы с nRF24L01+
const uint64_t pipe = 0xF0F1F2F3F4LL; // идентификатор передачи
RF24 radio(9,10); // Для MEGA2560 замените на RF24 radio(9,53);
int8_t data=0;
int potPin = 5; 
ResponsiveAnalogRead analog(potPin, true, 0.03);
int potValue,ledValue;  // value from the analog pin
int val = 0;

#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 8

CRGB leds[NUM_LEDS];
#define BRIGHTNESS  64



void setup() {
  pinMode(CLK, INPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DATA, INPUT);
  pinMode(DATA, INPUT_PULLUP); 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin (9600);
  Serial.println("RC Start:");
  leds[0] = CRGB::RoyalBlue;
  FastLED.show();

  radio.begin();       // включаем радио модуль
  radio.setChannel(0); // выбираем канал (от 0 до 127)

    // скорость: RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
  radio.setDataRate(RF24_1MBPS);
    // мощность: RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM
  radio.setPALevel(RF24_PA_HIGH);

  radio.openWritingPipe(pipe);      // открываем трубу на передачу
  radio.stopListening();  
  Serial.print("CurrentSpeed: ");
  Serial.println(0); 
  radio.write(&data, sizeof(data));
  delay(2000);
}


void setColour(int c){
  switch (c) {
     case 0:
      leds[0] = CRGB::Navy;
      FastLED.show();
      break;
     case 1:
      leds[0] = CRGB::SpringGreen;
      FastLED.show();
      break;
     case 2:
      leds[0] = CRGB::Green;
      FastLED.show();
      break; 
     case 3:
      leds[0] = CRGB::LightYellow;
      FastLED.show();
      break;     
    case 4:
      leds[0] = CRGB::Yellow;
      FastLED.show();
      break;  
    case 5:
      leds[0] = CRGB::DarkOrange;
      FastLED.show();
      break; 
    case 6:
      leds[0] = CRGB::OrangeRed;
      FastLED.show();
      break;
    case 7:
      leds[0] = CRGB::Crimson;
      FastLED.show();
      break;
    case 8:
      leds[0] = CRGB::Red;
      FastLED.show();
      break;    
  }
}

void writeToRadio(int c) {
  int data=c;   
  radio.write(&data, sizeof(data)); // отправляем данные и указываем байты
  Serial.print("data: ");
  Serial.println(data);             // выводим данные на монитор порта
}

void loop() { 
  
  potValue = analog.getValue();   // reads the value of the potentiometer (value between 0 and 1023)
  analog.update();             
  ledValue = map(potValue, 0, 1023, 0, 8);   // scale it to use it with the servo library (value between 0 and 180)    
  Serial.print("CurrentSpeed: ");
  Serial.println(potValue); 
  setColour(ledValue);
  writeToRadio(potValue); 
  delay(200);
  }      
  

