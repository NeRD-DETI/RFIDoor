/*
   nfc.ino

    This program was adapted from the available manufacturer example.
   This program will send the data (id from a nfcTag) through serial

   Made some changes in the library function: nfc.readPassiveTargetID

    almeida.rodrigo@ua.pt
    October 2016
*/


//This example reads all MIFARE memory block from 0x00 to 0x63.
//It is tested with a new MIFARE 1K cards. Uses default keys for authenication.
//Contributed by Seeed Technology Inc (www.seeedstudio.com)

#include <PN532.h>
#include <SPI.h>
#include <Print.h>
#include <Servo.h>

/*Chip select pin can be connected to D10 or D9 which is hareware optional*/
/*if you the version of NFC Shield from SeeedStudio is v2.0.*/
#define  NFC_DEMO_DEBUG 1
#define PN532_CS 10
#define DEG_CLOSED 35
#define DEG_OPEN 130
#define SERVO_PIN 9
#define buttonPin 5
#define DELAY_HOLD_OPEN 2500

PN532 nfc(PN532_CS);

char temp;
int servoDegree = 0;
Servo myservo;  // create servo object to control a servo

extern uint32_t acceptCards[];
extern size_t card_count;


void setup(void) {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    while (1); // halt
  }

  // configure board to read RFID tags and cards
  nfc.SAMConfig();

  //starting servo on closed position
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object

  myservo.write(DEG_CLOSED);
  delay(2000);

  myservo.detach();
}

void loop(void) {

  //servoCalib();
  uint32_t id = 0;
  if (digitalRead(buttonPin) == LOW) {
       Serial.print("OPEN " + buttonPin);
      moveServo();
  }
  // look for MiFare type cards
  id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
  if (id != 0) {
    for (int i = 0; i < card_count; i++) { 

      if (id == acceptCards[i]) {

        Serial.print("chupa!!, id validated");
        Serial.println();
        moveServo();
        break;
      }
    }
    Serial.print("new id found!: ");
    Serial.print(id);
    Serial.println();
  } else {
    Serial.print("anyid");
    Serial.println();
  }
  delay(500);
}


//servo calibration func
void servoCalib() {
  while (1) {
    temp = Serial.read();
    if (temp == '1') {
      servoDegree += 5;
      myservo.write(servoDegree);
      Serial.print(servoDegree);
      Serial.println();
    }
    if (temp == '2') {
      servoDegree += 10;
      myservo.write(servoDegree);
      Serial.print(servoDegree);
      Serial.println();
    }
    if (temp == '3') {
      servoDegree -= 5;
      myservo.write(servoDegree);
      Serial.print(servoDegree);
      Serial.println();
    }

    if (temp == '4') {
      servoDegree -= 10;
      myservo.write(servoDegree);
      Serial.print(servoDegree);
      Serial.println();
    }
  }
}
void moveServo(){
          myservo.attach(SERVO_PIN);
        myservo.write(DEG_OPEN);
        delay(DELAY_HOLD_OPEN);
        myservo.write(DEG_CLOSED);
        delay(1000);
        myservo.detach();
}


