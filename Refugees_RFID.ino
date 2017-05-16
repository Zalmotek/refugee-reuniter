
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h> //include LCD libraries


Adafruit_LiquidCrystal lcd(0); 


#include <SPI.h>
#include <MFRC522.h>    //include RFID libraries

#define RST_PIN         9          // reset pin for RFID reader
#define SS_PIN          10         // select pin for RFID reader



int code1[] = {43,76,204,121};  //first code to compare
int code2[] = {43,152,165,121}; //second code to compare
int code3[] = {32,152,129,122}; //third code to compare

String uidString;  //for serial display

MFRC522 rfid(SS_PIN, RST_PIN);  //initialize RFID reader

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setBacklight(HIGH);
  
  Serial.println("LCD was initialized");
  SPI.begin();
  rfid.PCD_Init();
  
  Serial.println("RFID was initialized");
  Serial.println("Present card");
}

void loop() {
  //returns from loop if no card is present
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check if the PICC is of MIFARE type
 if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  //save the bytes in a string from the card
  uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);
    
  

  Serial.print("Tap card key: ");
  Serial.println(uidString);

  if (check_uid()==1)
    {
      Serial.println("Hello, <user1>!");
      Serial.println("");
      lcd.clear();
      lcd.print("Welcome, <user1>!");
    }
  else if (check_uid()==2)
    {
      Serial.println("Hello, <user2>!");
      Serial.println("");
      lcd.clear();
      lcd.print("Welcome, <user2>!");
    }
    else if (check_uid()==3) 
      {
        Serial.println("Hello, <user3>");
        Serial.println("");
        lcd.clear();
        lcd.print("Prototype: ");
        lcd.setCursor(0,1);
        lcd.print("Welcome, <user3>");
      }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

int check_uid() {
  int i = 0;
  boolean match = true;
  //comparing read card with the first saved code
  while(i<rfid.uid.size)
  {      
    if(!(rfid.uid.uidByte[i] == code1[i]))  
    {           
      match = false; 
    }
    i++;
  }
  if (match==true) return 1;  //returns 1 if we read the first card

  
  match=true;
  i=0;
  //comparing read card with the second saved code
  while(i<rfid.uid.size)
    {      
      if(!(rfid.uid.uidByte[i] == code2[i]))
      {           
        match = false;
      }
      i++;
    }
  if (match==true) return 2;  //returns 2 if we read the second card

  
  match=true;
  i=0;
  //comparing read card with the third saved code
  while(i<rfid.uid.size)
  {      
    if(!(rfid.uid.uidByte[i] == code3[i]))
    {           
      match = false;
    }
    i++;
  }
  if (match==true) return 3;  //returns 3 if we read the third card
  return 0;
}

