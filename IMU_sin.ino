#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h> //Gyroscope
#include <Adafruit_LSM303_U.h> //Magnometer

// Assigning unique IDs to each sensor at the same time
Adafruit_L3GD20_Unified       gyro   = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Mag_Unified   mag    = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel  = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails(void) {
  
  sensor_t sensor;
  
  gyro.getSensor(&sensor);
  Serial.println("-----------  GYROSCOPE  ------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  mag.getSensor(&sensor);
  Serial.println("----------  MAGNETOMETER  ----------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  accel.getSensor(&sensor);
  Serial.println("---------  ACCELEROMETER  ----------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  
  delay(500);
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Testing Gyroscope"); Serial.println("");
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  /* Display some basic information on this sensor */
  
  Serial.println("Testing Magnetometer"); Serial.println("");
  //mag.enableAutoRange(true);
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  Serial.println("Testing Accelerometer"); Serial.println("");
  //mag.enableAutoRange(true);
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  displaySensorDetails();
}

void loop() {
  /* Get a new sensor event */ 
  sensors_event_t event;  
 
  /* Display the results (speed is measured in rad/s) */
  gyro.getEvent(&event);
  Serial.print("Angular Velocity:        ");
  Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");
  Serial.println("rad/s ");
  
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  mag.getEvent(&event);
  Serial.print("Magnetic Orientation:    ");
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  
  /* Display the results (acceleration is measured in m/s^2) */
  accel.getEvent(&event);
  Serial.print("Acceleration:            ");
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  Serial.println("");
  delay(500);

}
