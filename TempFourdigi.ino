#include <TM74HC595Display.h>
#include <TimerOne.h>
#include <DHT.h>;

int SCLK = 6;
int RCLK = 5;
int DIO = 4;
int led = 7;
#define DHTPIN 2
#define DHTTYPE DHT11

TM74HC595Display visor(SCLK, RCLK, DIO);
unsigned char LED_0F[17];
int Temp;
int Humid;
int dig1Temp = 0;
int dig2Temp = 0;
int dig1Humi = 0;
int dig2Humi = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  LED_0F[0] = 0xC0; //0
  LED_0F[1] = 0xF9; //1
  LED_0F[2] = 0xA4; //2
  LED_0F[3] = 0xB0; //3
  LED_0F[4] = 0x99; //4
  LED_0F[5] = 0x92; //5
  LED_0F[6] = 0x82; //6
  LED_0F[7] = 0xF8; //7
  LED_0F[8] = 0x80; //8
  LED_0F[9] = 0x90; //9
  LED_0F[10] = 0xC6; //C
  LED_0F[12] = 0xBF; //-
  LED_0F[17] = 0x89; //H
  LED_0F[23] = 0x8C; //P
  Timer1.initialize(1500);
  Timer1.attachInterrupt(timerIsr);
  pinMode(led, OUTPUT);
}

void loop() {
  Temp = dht.readTemperature();
  dig1Temp = Temp / 10;
  dig2Temp = Temp - (dig1Temp * 10);
  Humid = dht.readHumidity();
  dig1Humi = Humid / 10;
  dig2Humi = Humid - (dig1Humi * 10);

  Serial.print("Humidity: ");
  Serial.print(Humid);
  Serial.print("%");
  Serial.print(" / Temperature: ");
  Serial.print(Temp);
  Serial.println("*C");


  visor.set(LED_0F[dig1Humi], 3);
  visor.set(LED_0F[dig2Humi], 2);
  visor.set(LED_0F[17], 1);
  visor.set(LED_0F[12], 0);
  delay(2000);
  visor.clear();

  visor.set(LED_0F[12], 3);
  visor.set(LED_0F[12], 2);
  visor.set(LED_0F[12], 1);
  visor.set(LED_0F[12], 0);
  delay(1000);
  visor.clear();

  visor.set(LED_0F[dig1Temp], 3);
  visor.set(LED_0F[dig2Temp], 2);
  visor.set(LED_0F[12], 1);
  visor.set(LED_0F[10], 0);
  delay(2000);
  visor.clear();

  if (Temp >= 29) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
void timerIsr()
{
  visor.timerIsr();
}
