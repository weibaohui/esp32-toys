#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
int x = 0;
int y = 0;
int z = 0;
int value = 0;

void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // 将加速度计范围设置为 +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // 将陀螺仪范围设置为 +- 500 度/秒
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // 将滤波器带宽设置为 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  delay(100);
}

void loop()
{

  if (mpu.getMotionInterruptStatus())
  {
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

    //
    //      Serial.print("Gyroscope ");
    //      Serial.print("X: ");
    //      Serial.print(g.gyro.x, 1);
    //      Serial.print(" rps, ");
    //      Serial.print("Y: ");
    //      Serial.print(g.gyro.y, 1);
    //      Serial.print(" rps, ");
    //      Serial.print("Z: ");
    //      Serial.print(g.gyro.z, 1);
    //      Serial.println(" rps");

    // LED 向上，引脚指向自己

    // Serial.printf("x:%9.6f rps\r\n", g.gyro.x);

    //
    //      Vector rawAccel = mpu..readRawAccel();
    //      Vector normAccel = mpu.readNormalizeAccel();
    //
    //      Serial.print(" Xraw = ");
    //      Serial.print(rawAccel.XAxis);
    //      Serial.print(" Yraw = ");
    //      Serial.print(rawAccel.YAxis);
    //      Serial.print(" Zraw = ");
    //
    //      Serial.println(rawAccel.ZAxis);
    //      Serial.print(" Xnorm = ");
    //      Serial.print(normAccel.XAxis);
    //      Serial.print(" Ynorm = ");
    //      Serial.print(normAccel.YAxis);
    //      Serial.print(" Znorm = ");
    //      Serial.println(normAccel.ZAxis);

    // Serial.print(x);Serial.print(" ");Serial.println(y);

    ////acceleration就是加速度的意思///
    x = a.acceleration.x;
    y = a.acceleration.y;
    z = a.acceleration.z;
    if (x < 10 && x > 0 && y < 4 && y > -4)
    {
      Serial.println("up");
    }
    else if (x > -10 && x < 0 && y < 4 && y > -4)
    {
      Serial.println("down");
    }

    if (y < 10 && y > 0 && x < 4 && x > -4)
    {
      Serial.println("Right");
       digitalWrite(15, HIGH);
      digitalWrite(16, LOW);
      
    }
    else if (y > -10 && y < 0 && x < 4 && x > -4)
    {
      Serial.println("left");
     digitalWrite(15, LOW);
      digitalWrite(16, HIGH);
    }
  }

  delay(10);
}

void led_right()
{
}