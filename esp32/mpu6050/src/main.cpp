#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  delay(100);
}

void loop() {

  if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);



//
//
//      Serial.print("Accelerometer ");
//      Serial.print("X: ");
//      Serial.print(a.acceleration.x, 1);
//      Serial.print(" m/s^2, ");
//      Serial.print("Y: ");
//      Serial.print(a.acceleration.y, 1);
//      Serial.print(" m/s^2, ");
//      Serial.print("Z: ");
//      Serial.print(a.acceleration.z, 1);
//      Serial.println(" m/s^2");


      Serial.print("Gyroscope ");
      Serial.print("X: ");
      Serial.print(g.gyro.x, 1);
      Serial.print(" rps, ");
      Serial.print("Y: ");
      Serial.print(g.gyro.y, 1);
      Serial.print(" rps, ");
      Serial.print("Z: ");
      Serial.print(g.gyro.z, 1);
      Serial.println(" rps");
  }

  delay(10);
}