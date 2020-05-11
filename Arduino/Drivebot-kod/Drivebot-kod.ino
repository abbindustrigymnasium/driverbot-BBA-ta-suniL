#include "EspMQTTClient.h"
#include <Servo.h>

Servo servo;

void onConnectionEstablished();

EspMQTTClient client(
  "Olofsson_Guest",           // Wifi ssid
  "79Bj4a76",           // Wifi password
  "maqiatto.com",  // MQTT broker ip
  1883,             // MQTT broker port
  "linus.olofsson@abbindustrigymnasium.se",            // MQTT username
  "robotbil123",       // MQTT password
  "Robotbilen",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages
);


#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1
#define motorPinLeftDir 2
#define motorPinLeftSpeed 4

void setup() {
  servo.attach(14); //D5
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);
  pinMode(motorPinLeftDir, OUTPUT);
  pinMode(motorPinLeftSpeed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  servo.write(0);

  delay(2000);
}

bool off = false;
int speed = 0;
int degrees = 0;
int dir = 0;

void turn(bool left, int degrees) {

  if (left == true)
  {
    degrees -= 90;

    if (degrees < 0)
    {
      degrees = 0;
      digitalWrite(motorPinLeftDir, 1);
      analogWrite(motorPinLeftSpeed, 1000);
    }
    servo.write(degrees);
    Serial.println("Åk Vänster!");
  }
  else
  {

    degrees += 90;
    if (degrees > 180)
    {
      degrees = 180;
      digitalWrite(motorPinLeftDir, 1);
      analogWrite(motorPinLeftSpeed, 1000);
    }
    servo.write(degrees);
    Serial.println("Åk Höger!");

  }
  Serial.println(degrees);


}

void drive(bool dir, int speed) {

  //Om du vill åka rakt fram eller bak
  servo.write(90);

  Serial.println("Åk!");

  Serial.println(speed);
  digitalWrite(motorPinLeftDir, dir);
  analogWrite(motorPinLeftSpeed, speed);

  digitalWrite(motorPinRightDir, dir);
  analogWrite(motorPinRightSpeed, speed);

  digitalWrite(LED_BUILTIN, dir);

}



void onConnectionEstablished(){

  client.subscribe("linus.olofsson@abbindustrigymnasium.se/speed", [] (const String & payload)
  {

    if (payload.startsWith("h-")){
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(2, length);
      speed = value.toInt()*20;
      dir = 0;
      Serial.println(speed);
    }
      else if (payload.startsWith("h")){   
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(1, length);
      speed = value.toInt()*20;
      dir = 1;
      Serial.println(speed);
    }
    else if (payload.startsWith("h")){   
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(1, length);
      degrees = value.toInt();
      Serial.println(degrees);
    }
    else {   
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(1, length);
      degrees = value.toInt();
      Serial.println(degrees);
    }


  });

}

void loop() {

  client.loop();

  digitalWrite(motorPinRightDir, dir);
  analogWrite(motorPinRightSpeed, speed);
  
}
