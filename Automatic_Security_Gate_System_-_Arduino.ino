// Project  - Automatic Security Gate System
// Module   - Group Project in Hardware
// Group    - 10

// Code for Arduino Mega 

// Libraries
#include <Servo.h>              // Servo motor
#include <LiquidCrystal_I2C.h>  // LCD display
#include <MFRC522.h>            // RFID module
#include <SoftwareSerial.h>     // Serial communication
#include <uRTCLib.h>            // Real-time clock module

// Buttons
const int buttonPin = 11;       // Pin of the button to off the alarm  
const int vipbuttonPin = 8;     // Pin of the VIP button

// Button state (will change to 1 when push)
int buttonState = 0;  
int vipbuttonState = 0;


// Buzzer
int buzzPin= 12;                // Pin of buzzer

// SERVO Motor
Servo servo_gate;               // Servo motor of the gate
Servo servo_barrier;            // Servo motor of the barrier

// Motor position (angles in degrees as integer values)
int pos = 90;                   // Position of gate
int pos1 = 0;                   // Position of barrier

// IR Sensor
int irBefore = 2;               // Pin of IR sesnor before RFID module
int irAfter = 3;                // Pin of IR sesnor after RFID module

// IR sensor state (will change to 1 when vehicle detects)
int frontIR = 0;                // IR sesnor before RFID module
int rearIR = 0;                 // IR sesnor after RFID module

// Real-time clock module
uRTCLib rtc(0x68);

// LCD (16X2 with I2C)
LiquidCrystal_I2C lcd (0x27,16,2);


SoftwareSerial s(5,6);           // Rx,Tx use to connect with NodeMCU (Pin 5 - RX, Pin 6 - TX)

//Initialized variable to store recieved data from NodeMCU
String data;                    // data recieve from Serial
String user,id;                 // split data into 2 seperate data, which recieve from NodeMCU 

// Variables to store date and time
char current_date[11];
char current_time[8];
int y=0,m=0,d=0,h=0,n=0,c=0;

char *strings[2];               // An array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;

//************************** SETUP *************************
void setup() {
  
  // starting arduino serial 
  Serial.begin(115200);
  // use to store serial data into a string data variable
  Serial.setTimeout(10);
 
  // starting NodeMCU serial communication
  s.begin(115200);

 // real-time clock module
 URTCLIB_WIRE.begin();
 // Below code use to initialize the current date and time to clock module (upload once and comment, or use to update) 
  rtc.set(0, 39, 8, 3, 31, 1, 23);
 // rtc.set(0, 0, 0, 0, 0, 0, 0);
 // FORMAT --> rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)

 
  while (!Serial) {
    Serial.print(".");            // wait for serial port to connect.
  }
  Serial.println("Serial Communication started");
 
  // LCD Boot up Message
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Security");
  lcd.setCursor(2,1);
  lcd.print("Gate System");
  delay(2000);
  lcd.clear();

  // IR Sensor
  pinMode(irBefore,INPUT);
  pinMode(irAfter,INPUT);

  // Button for stop the alarm
  pinMode(buttonPin,INPUT_PULLUP);
  // Button to access VIP pass to gate
  pinMode(vipbuttonPin, INPUT_PULLUP);

  // Buzzer
  pinMode(buzzPin,OUTPUT); 
 

  // SERVO motor connect
  servo_gate.attach(10);          // Gate
  servo_barrier.attach(9);        // Barrier
  servo_gate.write(pos);          // Position values (in degrees)
  servo_barrier.write(pos1);
}



//************************** LOOP *************************
void loop() {
  
  // buzzer turn off at the begining
  digitalWrite(buzzPin,HIGH);
  
  // disply time on lcd
  rtc.refresh();
  
  // check if object detects from IR before RFID module
  if(digitalRead(irBefore) == LOW && frontIR == 0){
    frontIR = 1;                  // set the IR state to 1
    lcd.clear();                  // clear the lcd display

    // when vehicle detects to front IR, below loop run, till 'state' of IR after RFID module is equal to 1
    while(rearIR != 1){  
      // Place your card msg on LCD
      lcd.setCursor(0,0);
      lcd.print("Place your card");
      lcd.setCursor(0,1);
      lcd.print("here         ==>");
      delay(500);
      
      // check if vehicle pass the rear IR
      if(digitalRead(irAfter) == LOW){
        data = "null";             // set data to null (use to check later if vehicle tries to trespass)
        rearIR = 1;                // set the sate of rear IR to 1
      }

      // check if VIP button has push
      if(digitalRead(vipbuttonPin) == LOW){
         vipbuttonState = 1;       // set the button state
         
         // msg on arduino serial monitor
         Serial.println("Authorized access");
         Serial.println();
          
          // display welcome message in lcd
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Hi, Welcome");
         lcd.setCursor(0,1);
         lcd.print("Guest");      // user name set to Guest
      
          // sending data to NodeMCU
          s.print("VIP");
          
          // opening gate (loop added for smooth rotation of motor)
          for (pos = 90; pos >= 0; pos -= 1) {
            servo_gate.write(pos);  
            delay(5);    
          }
          
          rearIR = 1;             // state of rear IR set manually
          
          delay(6000);            // time (in miliseconds) holds gate open
          
          // closing gate 
          for (pos = 0; pos <= 90; pos += 1) {
            servo_gate.write(pos);              
            delay(5);  
          }
          delay(200);
          lcd.clear();
      }
      
      
      // check if getting serial data from NodeMCU
      if (Serial.available()>0) {
      
        // data received from nodemcu as "name,id"
        data = Serial.readString();

        //splitting string into user and id
        int str_len = data.length() + 1;
        char array[str_len];
        data.toCharArray(array, str_len);
        byte index = 0;
        ptr = strtok(array, ",");    // delimiter - comma
        
         while (ptr != NULL)
         {
            strings[index] = ptr;
            index++;
            ptr = strtok(NULL, ",");
         }
         
        // save splitted data from array
        user = strings[0];          
        id = strings[1];
      
        // printing data on arduino serial
        Serial.println(data);
      
        // checking if data get from database 
        if (data != "null")         // null - if id not exsit in database or can't connect to databse
        {
          // msg on arduino serial monitor
          Serial.println("Authorized access");
          Serial.println();
          
          // display welcome message in lcd
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Hi, Welcome");
          lcd.setCursor(0,1);
          lcd.print(user);        // printing user name which recive from database
      
          // sending user id for nodemcu
          s.print(user);
          
          // opening gate
          for (pos = 90; pos >= 0; pos -= 1) {
            servo_gate.write(pos);  
            delay(5);    
          }
          
          rearIR = 1;             // set rear IR state
          delay(6000);            // gate opening time
          
          // closing gate 
          for (pos = 0; pos <= 90; pos += 1) {
            servo_gate.write(pos);              
            delay(5);  
          }
          delay(200);
          lcd.clear();
        }
       
       else   {                   // deny acces false card
        
          Serial.println("Access denied");
          lcd.setCursor(0,0);
          lcd.setCursor(1,0);
          lcd.clear();
          lcd.print("Access Denied");
          
          // sending msg to NodeMCU
          s.print("False Card");
         
          Serial.println(digitalRead(buttonPin));

          // opening barrier
          for (pos1 = 0; pos1 <= 90; pos1 += 1) {
            servo_barrier.write(pos1);  
            delay(5);    
          }

          // Turning on alarm
          while(digitalRead(buttonPin) != LOW){
            // buzzer function
            dash();
            dot();
            dash();
            dot();
            dash();
            dash();
            dot();
            dash();
  
           }
         
          // closimg barrier
          for (pos1 = 90; pos1 >= 0; pos1 -= 1) {
            servo_barrier.write(pos1);  
            delay(5);    
          }
          delay(200);
          lcd.clear();
        }
      }
    }
    
    if (data=="null"){          // deny access force enter
          Serial.println("Access denied");
          lcd.setCursor(0,0);
          lcd.setCursor(1,0);
          lcd.clear();
          lcd.print("Access Denied");
          
          // sending msg for nodemcu
          s.print("Trespass");

         // opening barrier
          for (pos1 = 0; pos1 <= 90; pos1 += 1) {
            servo_barrier.write(pos1);  
            delay(5);    
          }
          
          // Buzzer function
          while(digitalRead(buttonPin) != LOW){
            dash();
            dot();
            dash();
            dot();
            
            dash();
            dash();
            dot();
            dash();
          }
          // closimg barrier
          for (pos1 = 90; pos1 >= 0; pos1 -= 1) {
            servo_barrier.write(pos1);  
            delay(5);    
          }
          delay(200);
          lcd.clear();
    }
    // resetting state value   
    frontIR = 0;
    
  }else{
    // display time    
    lcd.setCursor(3,0);
    y = rtc.year();
    m = rtc.month();
    d = rtc.day();

    // date    
    sprintf(current_date,("20%d/%02d/%02d   "), y,m,d);
    
    lcd.print(current_date);
    lcd.setCursor(4,1);
    
    h = rtc.hour();
    n = rtc.minute();
    c = rtc.second();

    // d=time
    sprintf(current_time,("%02d:%02d:%02d"), h,n,c);
    lcd.print(current_time);

    // hold time for 1 second and refresh
    delay(1000);
  }
  
  
  // resetting values
  data = "";
  user = "";
  id = "";
  rearIR = 0;
  vipbuttonState = 0;

}

// ****End of void loop****

// buzzer functions 
void dot(){
  digitalWrite(buzzPin, LOW);     // Tone ON
  delay(100);                     // Tone length
  digitalWrite(buzzPin, HIGH);    // Tone OFF
  delay(100);                     // Symbol pause
  return;
}

void dash(){
  digitalWrite(buzzPin, LOW);     // Tone ON
  delay(100);                     // Tone length
  digitalWrite(buzzPin, HIGH);    // Tone OFF
  delay(100);                     // Symbol pause
  return;
}
