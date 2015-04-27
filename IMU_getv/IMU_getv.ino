// Deriving velocity, distance traveled from acceleration values
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h> //Magnometer, Accelerometer

#include <Time.h>  
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


// Assign unique ID to accelerometer
Adafruit_LSM303_Accel_Unified accel  =   Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails(void) {
  
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("Velocity, distance derivation test program");
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  
  delay(1000);
}


void clock(){
  // digital clock display of the time
  Serial.print("Time Lapsed: ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void setup() {
  Serial.begin(9600);
  Serial.println("Testing accelerometer ");
  
  if(!accel.begin())
  {
    Serial.println("LSM303 not detected. Cannot read acceleration values.");
    while(1);
  }
  else{
    displaySensorDetails();
  }
}

void loop() {
  // get a new sensor event
  sensors_event_t event;
  
  //int accelmax = event.max_value; DOESNT WORK
  //int accelmin = sensor.min_value;

  
  accel.getEvent(&event);
  Serial.print("Acceleration: ");
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  Serial.print("Velocity:     ");
  Serial.print("X: "); //Serial.print(event.velocity.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print("  ");
  Serial.print("Z: "); Serial.print("  "); Serial.println("m/s ");
  
  clock();
  //Serial.print(accelmax);
  //Serial.print(accelmin);
  delay(500);  
  Serial.println("");
  
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
