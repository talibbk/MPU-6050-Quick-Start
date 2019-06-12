#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
MPU6050 mpu;
uint8_t mpuIntStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
float totalYaw;
float totalPitch;
float totalRoll;
Quaternion q;
VectorFloat gravity;
float ypr[3];
volatile bool mpuInterrupt = false;
void dmpDataReady() {mpuInterrupt = true;}

void setup() {

    Wire.begin();
    TWBR = 24;
    mpu.initialize();
    mpu.dmpInitialize();
    mpu.setDMPEnabled(true);
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    packetSize = mpu.dmpGetFIFOPacketSize();

    Serial.begin(115200);

}

void loop() {

    while (!mpuInterrupt && fifoCount < packetSize) {






    }

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
 
        Serial.println(F("FIFO overflow!"));
    }
    else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        
        totalYaw = ypr[0]*180/M_PI-30; //minus error to get better value
        totalPitch =  -(ypr[1]*180/M_PI- 360);
        totalRoll = ypr[2]*180/M_PI-1;
        
        //COMENTAR_OFICIAL
        Serial.print("ypr\t");
        Serial.print(totalYaw);
        Serial.print("\t");
        Serial.print(totalPitch);
        Serial.print("\t");
        Serial.print(totalRoll);
        Serial.println();

    }

}
