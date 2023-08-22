// Project  - Automatic Security Gate System
// Module   - Group Project in Hardware
// Group    - 10

// Code for NodeMCU (ESP8266)

// pins ofRFID module
#define SS_PIN 4                    // D2
#define RST_PIN 5                   // D1
#define led D0                      // built in LED
 
// Web host
#define HOST "gphw10-security.000webhostapp.com"          

#define WIFI_SSID "^_^"             // WIFI SSID                                   
#define WIFI_PASSWORD "88888888"    // WIFI password


#include <SPI.h>                    //RFID
#include <MFRC522.h>                //RFID
#include <ESP8266WiFi.h>            //NodeMCU
#include <ESP8266HTTPClient.h>      //NodeMCU

HTTPClient http;                    // http object of clas HTTPClient
WiFiClient wclient;                 // wclient object of clas HTTPClient 

String postData, postData1;
String data;

//RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522(RFID) instance 


//----------SETUP--------------

void setup() {
  pinMode(D0, OUTPUT);
  
  SPI.begin();                      // Initiate  SPI bus
  mfrc522.PCD_Init();               // Initiate MFRC522

  // Starting serial communication
  Serial.begin(115200);
  Serial.setTimeout(10);
  
  while (!Serial) {
  Serial.print(".");                // wait for serial port to connect. Needed for native USB port only
  } 

  // Communication establish with Arduino
  Serial1.begin(115200); 
  
  Serial.println("Communication Started \n\n");  
  delay(1000);
  
  // connecting to Wifi
  WiFi.mode(WIFI_STA);           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);       
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED){ 
     Serial.print(".");
     // LED pattern
     delay(50); 
     digitalWrite(led, LOW);
     delay(50);
     digitalWrite(led, HIGH);
  }
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());    // print local IP address

  // LED pattern
  delay(100); 
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100); 
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
}


//----------LOOP------------

void loop() 
{
  
  data = Serial.readString();
  
  // checking arduino data from serial
  if (data == "Trespass" || data == "False Card" || data == "VIP"){
    
    postData1 = "id=" + data;
    http.begin(wclient, "http://gphw10-security.000webhostapp.com/dbwrite.php");              // Connect to host where MySQL databse is hosted
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  

    int httpCode = http.POST(postData1);   // Send POST request to php file and store server response code in variable named httpCode

    if (httpCode == 200) {  
      //Serial.println(httpCode); 
      String output = http.getString();    // Get output of php script
      Serial.println(output + "\n");
    }else { 
      // if failed to connect then return and restart
      Serial.println("Failed to connect to database.\n");
      http.end();
      return;
    }
      }else{
          // Look for new cards
          if ( ! mfrc522.PICC_IsNewCardPresent()) 
          {
            return;
          }
          // Select one of the cards
          if ( ! mfrc522.PICC_ReadCardSerial()) 
          {
            return;
          }
          
          delay(100); 
          digitalWrite(led, LOW);
          delay(100);
          digitalWrite(led, HIGH);
          delay(100); 
          digitalWrite(led, LOW);
          delay(100);
          digitalWrite(led, HIGH);
          
          // Show UID on serial monitor
          Serial.println();
          Serial.print("UID tag :");
          String content= "";
          byte letter;
          
          for (byte i = 0; i < mfrc522.uid.size; i++) {
             content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
             content.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          content.toUpperCase();
          Serial.println(content);
          
          //SEND ID to server 
          postData = "id=" + content;
          http.begin(wclient, "http://gphw10-security.000webhostapp.com/dbread.php");              // Connect to host where MySQL databse is hosted
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
        
          int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
        
          delay(500);
          
          // if connection eatablished then do this
          if (httpCode == 200) { 
            Serial.println("Connected to database successfully."); 
           
            String output = http.getString();    // Get user name of the card
            Serial.println("Name : " + output + "\n");
            Serial1.print(output);  
            digitalWrite(led, LOW);
            delay(500);
            digitalWrite(led, HIGH);
            delay(500); 
            digitalWrite(led, LOW);
            delay(500);
            digitalWrite(led, HIGH);
            delay(1000);
           
            if (Serial.available()) {
              data = Serial.readString();
              Serial.println(data);
        
              postData1 = "id=" + data;
              http.begin(wclient, "http://gphw10-security.000webhostapp.com/dbwrite.php");              // Connect to host where MySQL databse is hosted
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
        
              int httpCode = http.POST(postData1);   // Send POST request to php file and store server response code in variable named httpCode
        
              if (httpCode == 200) {  
                String output = http.getString();    // Get output of php script
                Serial.println(output + "\n");
              }else { 
                // if failed to connect then return and restart
                Serial.println("Failed to connect to database.\n");
                http.end();
                return;
              }
              delay(6000);
            }
         }else { 
            // if failed to connect then return and restart
        
            Serial.println("Failed to connect to database.\n");
            Serial1.print("null");
            http.end(); 
            
            delay(250); 
            digitalWrite(led, LOW);
            delay(250);
            digitalWrite(led, HIGH);
            delay(250); 
            digitalWrite(led, LOW);
            delay(250);
            digitalWrite(led, HIGH);
            delay(250); 
            digitalWrite(led, LOW);
            delay(250);
            digitalWrite(led, HIGH);
            delay(250); 
            digitalWrite(led, LOW);
            delay(250);
            digitalWrite(led, HIGH);
            return;
          }
      }
  
} 
