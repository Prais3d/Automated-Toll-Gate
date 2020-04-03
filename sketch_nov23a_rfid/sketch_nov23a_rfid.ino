#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal lcd(23, 25, 27, 29, 31, 33);
char state=1;
Servo gate;

String vehicle;

void setup() {
  Serial.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  gate.attach(8); 
  SPI.begin();      
  mfrc522.PCD_Init();   
  lcd.begin(16, 2);
  initialize();
}

void loop() {
  if(Serial.available()){
    vehicle = Serial.readString();
  }
  checkUser();
}

void newUser(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  String tagId = content.substring(1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vehicle Added");
  lcd.setCursor(0,1);
  lcd.print("ID: ");
  lcd.setCursor(3,1);
  lcd.print(tagId);
}

void checkUser(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Serial.print("ID: ");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ID: ");
  lcd.setCursor(3,0);
  //Serial.print("Message : ");
  content.toUpperCase();
  String tagId = content.substring(1);
  lcd.print(tagId);
  if (vehicle.indexOf(tagId) > 0)
  {
    Serial.println(tagId); 
    //Serial.println("Authorized access");
    lcd.setCursor(0,1);
    lcd.print("Tk 100 paid, Go");
    digitalWrite(6,HIGH);
    gate.write(120);
    delay(3000);
    initialize();
  }
  else{
    //Serial.println(" Access denied");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(tagId);
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("Access denied");
    lcd.setCursor(0,1);
    lcd.print("Go Right");
    digitalWrite(7,HIGH);
    delay(3000);
    initialize();
  }  
} 

void initialize(){
  //Serial.println("Automated Toll Collection");
  lcd.setCursor(0, 0);
  lcd.print(" AUTOMATED TOLL ");
  lcd.setCursor(0, 1);
  lcd.print("    SCAN NOW    ");
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  gate.write(10);
}


