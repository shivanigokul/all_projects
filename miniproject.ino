#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 16, 2); 

#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);

#include "Adafruit_Fingerprint.h"
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

int total_id = 25; 
int set_time=8; 

#define bt_enroll 4
#define bt_up     5
#define bt_down   6
#define bt_delete 7

#define door_C_pin 8
#define door_O_pin 9    

#define buzzer 13 

void setup(){ 

Serial.begin(9600);
finger.begin(57600);

pinMode(bt_enroll, INPUT_PULLUP);
pinMode(bt_up,     INPUT_PULLUP); 
pinMode(bt_down,   INPUT_PULLUP); 
pinMode(bt_delete, INPUT_PULLUP);

pinMode(door_O_pin,OUTPUT); 
pinMode(door_C_pin,OUTPUT); 

pinMode(buzzer,OUTPUT); 

lcd.init();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Security System");
lcd.setCursor(0,1);
lcd.print("by Finger Print");
delay(2000); 
lcd.clear(); 
lcd.print("Finding Module");
lcd.setCursor(0,1);
delay(1000);
if(finger.verifyPassword()){Serial.println("Found fingerprint sensor!");
lcd.clear();
lcd.print("Found Module ");
} 
else{Serial.println("Did not find fingerprint sensor :(");
lcd.clear();
lcd.print("module not Found");
lcd.setCursor(0,1);
lcd.print("Check Connections");
while (1);
}

digitalWrite(door_O_pin, LOW); 
digitalWrite(door_C_pin, HIGH);
delay(2000); 
digitalWrite(door_O_pin, LOW); 
digitalWrite(door_C_pin, LOW); 

lcd.clear();
}

void loop(){
lcd.setCursor(0,0);
lcd.print("Place Finger [ ]");
lcd.setCursor(0,1);
lcd.print("                ");

int result=getFingerprintIDez();
if(result>0){  
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);  
lcd.clear();
lcd.print("Please Wait... ");
lcd.setCursor(0,1);
lcd.print(" Door is Opening");
digitalWrite(door_O_pin, HIGH);
digitalWrite(door_C_pin, LOW);  
delay(2000); 
digitalWrite(door_O_pin, LOW);  
digitalWrite(door_C_pin, LOW); 

lcd.clear();
lcd.print("    Welcome   ");
lcd.setCursor(0,1);
lcd.print("  Door is Open ");
for(int i=0;i<set_time;i++){delay(1000);}

lcd.clear();
lcd.print("Please Wait... ");
lcd.setCursor(0,1);
lcd.print(" Door is Closing");
digitalWrite(door_O_pin, LOW); // Door Open 
digitalWrite(door_C_pin, HIGH);// Door Close 
delay(2000); // Waiting for a while
digitalWrite(door_O_pin, LOW); // Door Open 
digitalWrite(door_C_pin, LOW); // Door Close 
 
delay(100);  
lcd.clear();
return;
}

checkKeys();
delay(500);
}

void checkKeys(){

if(digitalRead(bt_enroll) == 0){
lcd.clear();
lcd.print("Please Wait");
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
delay(1000);
while(digitalRead(bt_enroll) == 0);
Enroll();
}

else if(digitalRead(bt_delete) == 0){
lcd.clear();
lcd.print("Please Wait");
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
delay(1000);
delet();
}  

}
void Enroll(){
int count=0;
lcd.clear();
lcd.print("Enroll Finger    ");
lcd.setCursor(0,1);
lcd.print("Location:");
while(1){
lcd.setCursor(9,1);
lcd.print(count);
lcd.print("   ");

if(digitalRead(bt_up) == 0){
count++;
if(count>25)count=0;
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW); 
}

else if(digitalRead(bt_down) == 0){
count--;
if(count<0)count=25;
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
}

else if(digitalRead(bt_enroll) == 0){
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
id=count;
getFingerprintEnroll();
return;
}
     
else if(digitalRead(bt_delete) == 0){
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
return;
  }
  
 }
}

void delet(){
int count=0;
lcd.clear();
lcd.print("Delete Finger    ");
lcd.setCursor(0,1);
lcd.print("Location:");

while(1){
lcd.setCursor(9,1);
lcd.print(count);
lcd.print("   ");

if(digitalRead(bt_up) == 0){
count++;
if(count>25)count=0;
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
}

else if(digitalRead(bt_down) == 0){
count--;
if(count<0)count=25;
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
}

else if(digitalRead(bt_delete) == 0){
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
id=count;
deleteFingerprint(id);
return;
}

else if(digitalRead(bt_enroll) == 0){        
digitalWrite(buzzer, HIGH);
delay(300); 
digitalWrite(buzzer, LOW);
return;
   }
 }
}
uint8_t getFingerprintEnroll(){
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No Finger");
      lcd.clear();
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.print("Imaging Error");
      break;
    default:
      Serial.println("Unknown error");
       lcd.clear();
      lcd.print("Unknown Error");
      break;
    }
  }
  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       lcd.clear();
       lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
   lcd.clear();
      lcd.print("Place Finger");
      lcd.setCursor(0,1);
      lcd.print("   Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      return;
    }
  }
  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else{
    Serial.println("Unknown error");
    return p;
  }   
}
int getFingerprintIDez(){
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  
  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
   lcd.clear();
   lcd.print("Finger Not Found");
   lcd.setCursor(0,1);
   lcd.print("Try Later ");
   for(int x=0; x<10; x++){
   digitalWrite(buzzer, HIGH);
   delay(300);  
   digitalWrite(buzzer, LOW);
   delay(100);
   }
  return -1;
  }
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id){
  uint8_t p = -1;  
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if(p == FINGERPRINT_OK) {Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Figer Deleted");
    lcd.setCursor(0,1);
    lcd.print("Successfully");
    delay(1000);
  }else{Serial.print("Something Wrong"); 
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0,1);
    lcd.print("Try Again Later");
    delay(2000);
    lcd.clear();
    return p;
  }   
}
