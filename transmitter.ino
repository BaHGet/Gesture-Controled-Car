#include <SPI.h>
// lib for important functions for nrf sensor
#include <RF24.h>
// lib for commucate between mpu and arduino
#include "Wire.h"
// lib for intaliez mpu sensor (GyroScope)
#include "MPU6050.h"

// Conncet the nrf's CE & CSE pins to D8 & D9 on arduino
RF24 radio(8, 9);

// initialize mpu
MPU6050 mpu;
// varbles for x,y,z axies
int16_t ax, ay, az;
// varbles for x,y,z rotation axies
/*
    NOTE: we want use the gx, gy, gz data for anything in this project
    it's only decleard for perfectionism
*/
int16_t gx, gy, gz;

// set a set for puting data of gyroScope in it
int data[5];

// declear the address for nrf to sending on it
const uint64_t receiverAddress = 0xE8E8F0F0E1LL;

// this function only called-work- once the arduion powerUp
void setup()
{
    // open serial on 9600 port
    /*
        serial is the screen which we open it to see the output othe device
    */
    Serial.begin(9600);

    // give the nrf power
    radio.begin();
    // make nrf sending the data on receiverAddress address
    radio.openWritingPipe(receiverAddress);
    // make the arduino listen for mpu
    Wire.begin();
    // initialize mpu
    mpu.initialize();
}

// this function called continuosly when it's finished
void loop()
{
    // get the x,y,x axies and rotation axies from mpu
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // save the ax, ay, az, gx, gy and gz in data set
    data[0] = map(ax, -17000, 17000, 300, 400);
    data[1] = map(ay, -17000, 17000, 100, 200);
    data[2] = map(az, -17000, 17000, 300, 400);
    data[3] = map(gx, -17000, 17000, 100, 200);
    data[4] = map(gy, -17000, 17000, 100, 200);
    data[5] = map(gz, -17000, 17000, 300, 400);
    // print Out the x, y data
    Serial.print("X:");
    Serial.print(data[0]);
    Serial.print(", Y:");
    Serial.println(data[1]);
    // send the data set to the car
    radio.write(data, sizeof(data));
}
