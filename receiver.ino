#include <SPI.h>
// lib for important functions for nrf sensor
#include <RF24.h>
// lib for important functions for servo
#include <Servo.h>

// Conncet the nrf's CE & CSE pins to D8 & D9 on arduino
RF24 radio(8, 9);

// define-Obviously-the triger and echo pins of the ultraSonic
#define TRIG_PIN A3
#define ECHO_PIN A4

// is car isn't moving
int isStop = 0;

// Create a Servo object
Servo myServo;

// declear the address for nrf to sending on it
const uint64_t senderAddress = 0xE8E8F0F0E1LL;

// Motor pins S.T A,IN1,IN2 is the right motors
const int enbA = 2;
const int enbB = 7;
const int IN1 = 3;
const int IN2 = 4;
const int IN3 = 6;
const int IN4 = 5;


// this function only called-work- once the arduion powerUp
void setup()
{
    // open serial on 9600 port
    /*
    serial is the screen which we open it to see the output othe device
    */
    Serial.begin(9600);

    // make ardino listen for specific pins as Output of motors
    pinMode(enbA, OUTPUT);
    pinMode(enbB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    myServo.attach(10);

    // give the nrf power
    radio.begin();
    // make nrf sending the data on receiverAddress address
    radio.openReadingPipe(1, senderAddress);
    // make the nrf start listeing on the proivieded addrss
    radio.startListening();
    
}

// this function called continuosly when it's finished
void loop()
{
       if (radio.available()) {
            int data[5];
            radio.read(data, sizeof(data));
            
            /* Serial.print("X:");
            Serial.print(data[0]);
            Serial.print(", Y:");
            Serial.println(data[1]); */
            Serial.println(getDistance());
  
            if(getDistance() > 40){
                
                //Forward
                if(data[1] >= 175){
                    analogWrite(enbA, 250);        
                    digitalWrite(IN1, HIGH);
                    digitalWrite(IN2, LOW);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, HIGH);
                    digitalWrite(IN4, LOW);
                }
                //Backward
                else if(data[1] <= 125){
                    analogWrite(enbA, 250);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);
                }
                //Right
                else if(data[0] >= 390){
                    analogWrite(enbA, 250);
                    digitalWrite(IN1, HIGH);
                    digitalWrite(IN2, LOW);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);
                }
                //Left
                else if(data[0] <= 320){
                    analogWrite(enbA, 250);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, HIGH);
                    digitalWrite(IN4, LOW); 
                }
                //Stop
                else {
                    analogWrite(enbA, 0);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    
                    analogWrite(enbB, 0);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                }
            }
            else{
                //Stop
                analogWrite(enbA, 0);
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                analogWrite(enbB, 0);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                
                
                
                //Backward
                if(data[1] <= 125){
                    analogWrite(enbA, 250);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);
                }
                //Right
                else if(data[0] >= 390){
                    analogWrite(enbA, 250);
                    digitalWrite(IN1, HIGH);
                    digitalWrite(IN2, LOW);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);
                }
                //Left
                else if(data[0] <= 320){
                    analogWrite(enbA, 250);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);

                    analogWrite(enbB, 250);
                    digitalWrite(IN3, HIGH);
                    digitalWrite(IN4, LOW); 
                }
                //Stop
                else{
                    analogWrite(enbA, 0);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    
                    analogWrite(enbB, 0);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                }
                    
            }
          
       }
              
       
}


float getDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2; // Speed of sound is 34 cm/ms

    return distance;
}
