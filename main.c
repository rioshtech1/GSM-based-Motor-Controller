/**
 * @author Manu Devappa
 *
 * @copyright Copyright (c) 2021
 * dmanu.techie@gmail.com
 */

#include <SoftwareSerial.h>

SoftwareSerial GPRS(3, 2);
String textMessage;
String lampState;
const int relay = 13; //If you're using a relay to switch, if not reverse all HIGH and LOW on the code
int ringCount;
void setup() {
  ringCount = 0;  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); // The current state of the light is ON

  Serial.begin(9600); 
  GPRS.begin(9600);
  delay(5000);
  Serial.print("GPRS ready...\r\n");
  GPRS.print("AT+CMGF=1\r\n"); 
  delay(1000);
  GPRS.print("AT+CNMI=2,2,0,0,0\r\n");
  delay(1000);
}

void loop(){
  if(GPRS.available()>0){
    textMessage = GPRS.readString();
    Serial.print(textMessage);    
    delay(10);

    if(textMessage.indexOf("RING")>=0){ 
      ringCount ++;
      Serial.println(ringCount);
    }
    
    if(textMessage.indexOf("NO CARRIER")>=0){ 
    if(ringCount == 2){
        on();
        ringCount = 0;   
        }
        else if(ringCount == 4){
        off();     
        ringCount = 0;
       }
       else{
        ringCount = 0;
        }
    } 
  }

   
  if(textMessage.indexOf("ON")>=0){ //If you sent "ON" the lights will turn on
    on();
  }
  
  if(textMessage.indexOf("OFF")>=0){
  off();
  }

  
  if(textMessage.indexOf("STATUS")>=0){
    status_reg();
  }
}

void status_reg(){
    if(lampState = "ON"){
    GPRS.println("AT+CMGS=\"+91xxxxxxxxxx\""); // RECEIVER: change the phone number here with international code
    delay(500);
    GPRS.print("Lamp is still ON.\r");
    GPRS.write( 0x1a );
    delay(1000);
    }
    else if(lampState = "OFF"){  
    GPRS.println("AT+CMGS=\"+91xxxxxxxxxx\""); // RECEIVER: change the phone number here with international code
    delay(500);
    GPRS.print("Lamp is still OFF.\r");
    GPRS.write( 0x1a );
    delay(1000);
    }
  }

void on(){
  // Turn on relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "ON";
    Serial.println("Lamp set to ON\r\n");  
    textMessage = "";
    GPRS.println("AT+CMGS=\"+91xxxxxxxxxx\""); // RECEIVER: change the phone number here with international code
    delay(500);
    GPRS.print("Lamp was finally switched ON.\r");
    GPRS.write( 0x1a );
    delay(1000);
  }

  void off(){
      // Turn off relay and save current state
    digitalWrite(relay, LOW);
    lampState = "OFF"; 
    Serial.println("Lamp set to OFF\r\n");
    textMessage = "";
    GPRS.println("AT+CMGS=\"+91xxxxxxxxxx\""); /// RECEIVER: change the phone number here with international code
    delay(500);
    GPRS.print("Lamp was finally switched OFF.\r");
    GPRS.write( 0x1a );
    delay(1000);
    }
