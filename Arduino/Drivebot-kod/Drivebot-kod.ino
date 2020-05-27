//Nödvändiga bibliotek inkluderas.
#include "EspMQTTClient.h"
#include <Servo.h>

Servo servo;

void onConnectionEstablished();

//Klienten sätts upp. Wifi-ssid och lösenord för internetanslutning.
//Info för att kunna ansluta till rätt MQTT-broker.
EspMQTTClient client(
  "...",           // Wifi ssid
  "...",           // Wifi password
  "maqiatto.com",  // MQTT broker ip
  1883,             // MQTT broker port
  "linus.olofsson@abbindustrigymnasium.se",            // MQTT username
  "robotbil123",       // MQTT password
  "Robotbilen",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages
);

//Pins på baskortet definieras. Används till motorn.
#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1
#define motorPinLeftDir 2
#define motorPinLeftSpeed 4

//Pin för servo definieras. Pinmode sätts för alla pins.
void setup() {
  servo.attach(14); //D5
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);
  pinMode(motorPinLeftDir, OUTPUT);
  pinMode(motorPinLeftSpeed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
}

//Integers för hastighet, körriktning och styrningsvinkel definieras. 
bool off = false;
int speed = 0;
int degrees = 0;
int dir = 0;


/*
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

*/

void onConnectionEstablished(){

  //När meddelande skickas till MQTT-brokern under topicet "speed" utförs det inne i funktionen.
  client.subscribe("linus.olofsson@abbindustrigymnasium.se/speed", [] (const String & payload){

    //Om payloaden börjar på "h-" ska allt från och med det tredje till sista tecknet plockas ut.
    //De utplockade tecknen blir till en string "value". Integern speed sätts till value gånger 20. 
    //Integern dir sätts till 0, som är bakåt.
    if (payload.startsWith("h-")){
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(2, length);
      speed = value.toInt()*20;
      dir = 0;
      Serial.println(speed);
    } 
    //Om payloaden börjar på "h" görs samma sak som innan.
    //Integern dir sätts till 1, som är framåt.
    else if (payload.startsWith("h")){   
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(1, length);
      speed = value.toInt()*20;
      dir = 1;
      Serial.println(speed);
    }
    //Om payloaden börjar på "st" sätts integern speed till 0 (bilen stannar).
    else if (payload.startsWith("st")){   
      speed = 0;
    };

    //Om payloaden börjar på "s-" ska allt från och med det andra till sista tecknet plockas ut.
    //De utplockade tecknen blir till en string "value". Integern degrees sätts till value plus 90 grader. 
    if (payload.startsWith("s-")){   
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(1, length);
      degrees = value.toInt()+90;
      Serial.println(degrees);
    }
    //Om payloaden börjar på "s" görs samma sak som innan.
    else if (payload.startsWith("s")){   
      char info = payload.charAt(0);
      int length = payload.length();
      String value = payload.substring(1, length);
      degrees = value.toInt()+90;
      Serial.println(degrees);
    }
    //Om payloaden börjar på "st" sätts integern degrees till 90 (hjulen pekar framåt).
    else if (payload.startsWith("st")){   
      degrees = 90;
      Serial.println(degrees);
    }


  });

}

//Allt inom denna funktion körs om och om igen.
//Motorn kör i riktningen "dir" med hastigheten "speed"
//Servot styr med "degrees" antal grader.
void loop() {

  client.loop();

  digitalWrite(motorPinRightDir, dir);
  analogWrite(motorPinRightSpeed, speed);
  servo.write(degrees);
  
}
