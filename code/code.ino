/**
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from more than one PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
 * MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
 *          and knowledge are required!
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define RFID_MOTOR      10         // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 2
#define RFID_MOBIL      8          // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 1

#define NR_OF_READERS   2

const int VLD_MOTOR = 4;
const int VLD_MOBIL =5;
const int RFID_RELAY_MOTOR = 2;
const int RFID_RELAY_MOBIL = 3;
byte ssPins[] = {RFID_MOTOR, RFID_MOBIL};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

/**
 * Initialize.
 */
void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus
  pinMode(VLD_MOTOR,INPUT_PULLUP);
  pinMode(VLD_MOBIL,INPUT_PULLUP);
  pinMode(RFID_RELAY_MOTOR,OUTPUT);
  pinMode(RFID_RELAY_MOBIL,OUTPUT);
  digitalWrite(RFID_RELAY_MOTOR,HIGH);
  digitalWrite(RFID_RELAY_MOBIL,HIGH);
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

/**
 * Main loop.
 */
void loop() {

    int readVLD_MOTOR = digitalRead(VLD_MOTOR);
    int readVLD_MOBIL = digitalRead(VLD_MOBIL);

    if(readVLD_MOTOR == 0){
      digitalWrite(RFID_RELAY_MOTOR,LOW);
    }
    if(readVLD_MOBIL == 0){
      digitalWrite(RFID_RELAY_MOBIL,LOW);
    }
  // for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards
    String content= "";
  byte letter;
  
    if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial()) {
      Serial.print(F("0 "));
      Serial.print(0);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[0].uid.uidByte, mfrc522[0].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[0].PICC_GetType(mfrc522[0].uid.sak);
      Serial.println(mfrc522[0].PICC_GetTypeName(piccType));
      for (byte i = 0; i < mfrc522[0].uid.size; i++) 
  {
     Serial.print(mfrc522[0].uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522[0].uid.uidByte[i], HEX);
     content.concat(String(mfrc522[0].uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522[0].uid.uidByte[i], HEX));
  }
      if (content.substring(1) == "C2 01 27 10") //change here the UID of the card/cards that you want to give access
      {
        digitalWrite(RFID_RELAY_MOTOR,LOW);
        Serial.println("Authorized access");
      }
      // Halt PICC
      mfrc522[0].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[0].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC

     if (mfrc522[1].PICC_IsNewCardPresent() && mfrc522[1].PICC_ReadCardSerial()) {
      Serial.print(F("1 "));
      Serial.print(1);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[1].uid.uidByte, mfrc522[1].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[1].PICC_GetType(mfrc522[1].uid.sak);
      Serial.println(mfrc522[1].PICC_GetTypeName(piccType));
        for (byte i = 0; i < mfrc522[1].uid.size; i++) 
  {
     Serial.print(mfrc522[1].uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522[1].uid.uidByte[i], HEX);
     content.concat(String(mfrc522[1].uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522[1].uid.uidByte[i], HEX));
  }
 if (content.substring(1) == "C2 01 27 10") //change here the UID of the card/cards that you want to give access
      {
        digitalWrite(RFID_RELAY_MOBIL,LOW);
        Serial.println("Authorized access");
      }
      // Halt PICC
      mfrc522[1].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[1].PCD_StopCrypto1();
    } //if (mfrc5
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
