#define BLYNK_TEMPLATE_ID "TMPL3j_NUjYJN"
#define BLYNK_TEMPLATE_NAME "Portable Garbage bin with Segregation"
#define BLYNK_AUTH_TOKEN "yUDkE0BZpDBfODfkJR-Qd5vZQEIr-pmD"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ultrasonic.h>
#include <Servo.h>

char ssid[] = "ArunYadav"; // Your WiFi network SSID
char pass[] = "chinmay2004";  //Password
  

const int moistureSensorPin = A0;  //moisture

#define TRIGGER_PIN1 5 // GPIO5  U1  D1 DRY 
#define ECHO_PIN1 4    // GPIO4  U1  D2 DRY
#define TRIGGER_PIN2 0 // GPIO0  U2  D3 WET
#define ECHO_PIN2 2    // GPIO2  U2  D4 WET
#define irSensorPin 14 // Assuming IR sensor is connected to pin D5 (GPIO 14)
#define DISTANCE_THRESHOLD 10


BlynkTimer timer;
Servo servo;

Ultrasonic ultrasonic1(TRIGGER_PIN1, ECHO_PIN1);   //Dry
Ultrasonic ultrasonic2(TRIGGER_PIN2, ECHO_PIN2);   //Wet


void notifydry(int distance) {
  
  if (distance < DISTANCE_THRESHOLD) {
    Blynk.logEvent("dry_bin_is_about_to_full","Your Dry waste is about to Full!"); //Enter your Event Name
    
  }
}

void notifywet(int distance) {
  
  if (distance < DISTANCE_THRESHOLD) {
    Blynk.logEvent("wet_bin_is_about_to_full","Your Wet waste is about to Full!"); //Enter your Event Name
    
  }
}



void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(irSensorPin, INPUT);
  servo.attach(16); //D0
  delay(2000);
  
}



void loop() {
  Blynk.run();
  timer.run();
  int sensorValue = digitalRead(irSensorPin);
  unsigned int distance1;
  unsigned int distance2;
  
  delay(1000);
  if(sensorValue==0){
    int moistureValue = analogRead(moistureSensorPin);
    Serial.print("Moisture Value: ");
    Serial.println(moistureValue);

    if (moistureValue < 1000) {
      // Soil is dry, turn the servo by 90 degrees
      setServoAngle(0);
      delay(100);

      setServoAngle(-90);
      delay(1000);

      setServoAngle(0);
      delay(1000);
    
    } 
    else {
      // Soil is wet, turn the servo to the initial position (0 degrees)
      setServoAngle(0);
      delay(100);

      setServoAngle(90);
      delay(1000);

      setServoAngle(0);
      delay(1000);
    
    }
  }

  distance1 = ultrasonic1.read(); // Read distance in centimeters
  distance2 = ultrasonic2.read();
  Serial.print("DRY GARBAGE STATUS: ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("WET GARBAGE STATUS: ");
  Serial.print(distance2);
  Serial.println(" cm");
  delay(1000); // Wait for a second before taking another reading
  Blynk.virtualWrite(V0, distance1);
  Blynk.virtualWrite(V1, distance2);
  delay(500);

  if (distance1 == 0) {
    Serial.println("Error: out of range");
  } 
  else {
    Serial.print("Distance: ");
    Serial.print(distance1);
    Serial.println(" cm");
    notifydry(distance1);
    Blynk.virtualWrite(V0, distance1); // Write distance value to Blynk virtual pin V5
  }
  delay(500);

  if (distance2 == 0) {
    Serial.println("Error: out of range");
  } 
  else {
    Serial.print("Distance: ");
    Serial.print(distance2);
    Serial.println(" cm");
    notifywet(distance2);
    Blynk.virtualWrite(V1, distance2); // Write distance value to Blynk virtual pin V5
  }
  delay(500);


  delay(1000);


}  


void setServoAngle(int angle) {
  // Map the angle to the servo range (0 to 180)
  int mappedAngle = map(angle, -90, 90, 0, 180);

  // Set the servo position
  servo.write(mappedAngle);

  // Print the angle to the Serial Monitor
  Serial.print("Servo angle: ");
  Serial.println(angle);
}




