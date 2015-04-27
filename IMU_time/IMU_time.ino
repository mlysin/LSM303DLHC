// Master IMU template
// As of 04/27/2015, no DR math (see IMU_getv)

#include <Time.h>  /*http://forum.arduino.cc/index.php?topic=100084.0*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h> //Gyroscope
#include <Adafruit_LSM303_U.h> //Magnometer, Accelerometer

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// Assigning unique IDs to each sensor at the same time
Adafruit_L3GD20_Unified       gyro   = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Mag_Unified   mag    = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel  = Adafruit_LSM303_Accel_Unified(54321);

// T1262347200  //noon Jan 1 2010

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

void setup()  {
  Serial.begin(9600);
  Serial.println("Testing Gyroscope ...");
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  else {
    Serial.println("Gyroscope (L3GD20) signal detected ");
    Serial.println("");
  }
  /* Display some basic information on this sensor */
  
  Serial.println("Testing Magnetometer ...");
  //mag.enableAutoRange(true);
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  else {
    Serial.println("Magnetometer (LSM303) signal detected ");
    Serial.println("");
  }
  
  Serial.println("Testing Accelerometer ...");
  //mag.enableAutoRange(true);
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  else {
    Serial.println("Accelerometer (LSM303) signal detected ");
    Serial.println("");
  }
  displaySensorDetails();
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print("Time Lapsed: ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  /*Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); */
  Serial.println(); 
}

void loop(){ 
   /* Get a new sensor event */ 
  sensors_event_t event;  
 
  digitalClockDisplay();
  /* Display the results (speed is measured in rad/s) */
  gyro.getEvent(&event);
  Serial.print("Angular Velocity:        ");
  Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("    ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("    ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("    ");
  Serial.println("rad/s ");
  
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  mag.getEvent(&event);
  Serial.print("Magnetic Orientation:    ");
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("    ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("    ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("    ");Serial.println("uT");
  
  /* Display the results (acceleration is measured in m/s^2) */
  accel.getEvent(&event);
  Serial.print("Acceleration:            ");
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("    ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("    ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("    ");Serial.println("m/s^2 ");
  Serial.println("");
  delay(500);

  
  if(Serial.available() ) 
  {
    processSyncMessage();
  }
  if(timeStatus()!= timeNotSet) 
    Serial.println("waiting for sync message");
  else     
      //digitalClockDisplay();
  delay(500);
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
}
