#include <Time.h>  /*http://forum.arduino.cc/index.php?topic=100084.0*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h> //Gyroscope
#include <Adafruit_LSM303_U.h> //Magnometer

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// Assigning unique IDs to each sensor at the same time
Adafruit_LSM303_Accel_Unified accel  = Adafruit_LSM303_Accel_Unified(54321);

// T1262347200  //noon Jan 1 2010

void displaySensorDetails(void) {
  
  sensor_t sensor;
  
  accel.getSensor(&sensor);
  Serial.println("---------  ACCELEROMETER START  ----------");
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("");
  
  delay(500);
}

void setup()  {
  Serial.begin(9600);
  
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

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print("Time Lapsed :            ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());//milliseconds display at nonrepresentative rate
  Serial.println("  "); 
}

void loop(){ 
   /* Get a new sensor event */ 
  sensors_event_t event;  
 
  if(Serial.available() ) {
      processSyncMessage();
  }
  if(timeStatus()!= timeNotSet) {
    Serial.println("waiting for sync message");
  }
  else {     
      //digitalClockDisplay();
  }
  delay(500); 
  
  
  /* Display the results (acceleration is measured in m/s^2) */
  if (second() != false){// if the timer is working
    
    if (0 < second() <= 10) {// start calibration period
      
      int t = second();
      int x = event.acceleration.x;
      int y = event.acceleration.y;
      int z = event.acceleration.z;
      int speeds[] = {t, x, y, z}; 
      
      accel.getEvent(&event);
      Serial.print("Acceleration:            ");
      Serial.print("X: "); Serial.print(x); Serial.print("  ");

      
      /*assign new accel values to variables
      //new accel are combined with existing accel, for t = 0 accel = 0
      //existing values are stored in array
      //new values are included in the array though
      //averaging them with the existing values*/
      Serial.print("Y: "); Serial.print(y); Serial.print("  ");
      Serial.print("Z: "); Serial.print(z); Serial.print("  ");Serial.println("m/s^2 ");
      digitalClockDisplay();
      Serial.println("  ");
      delay(500);
    }
    if (second() == 10) {
      Serial.print("End calibration");
      Serial.print("Average X: "); Serial.print("");
      Serial.print("Average Y: ");
      Serial.print("Average Z: ");
      Serial.println("  ");
    }
    else {
      
    }
  }  
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
