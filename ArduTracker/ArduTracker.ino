
// testSDnRF24.ino
// Combination of SimpleTxAckPayload - the master or the transmitter
//   abd ListFiles.ino

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SD.h>

#define CE_PIN  4// for nRF24
#define CSN_PIN 5 // for nRF24

#define csPin 22 // for SDCard

const byte broadcastAddress[5] = {'R','x','T','x','0'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char myID[10] = "Message 1"; // the two values to be sent to the master
char dataReceived[10]; // must match dataToSend in master

//===============

void setup() {

    Serial.begin(9600);
    
     pinMode(csPin, OUTPUT);    
     digitalWrite(csPin, HIGH);
     SPI.begin();
     setupSD();
     listFiles();
     SPI.transfer(0xFF);

    delay(100);

    // digitalWrite(csPin, LOW);
    
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    
    //this is a mesh so we don't want ACKs!
    // radio.setAutoAck(false); 

    radio.openWritingPipe(broadcastAddress);
    radio.openReadingPipe(1, broadcastAddress);

    radio.setRetries(3,5); // delay, count
    radio.startListening();
}

//=============

void loop() {

    listFiles();
    SPI.transfer(0xFF);

    // Random delay
    delay(1000);

    radio.stopListening();

    // Send
    sendData();

    radio.startListening();
    
    // Random delay
    delay(1000);

    for(int i = 0; i < 10; i++)
        // Receive
        getData();    
}

//================
//   These are the nRF24 functions
//================
void sendData() {

    radio.write(&myID, sizeof(myID));
    Serial.println("Sent my ID");   

 }

void getData(){

    if ( radio.available() ) {
        radio.read(&dataReceived, sizeof(dataReceived));
        Serial.print("Data received: ");
        Serial.println(dataReceived);
    }  
}

//============
//  And these are the SD Card functions
//============

void setupSD() {
    if (!SD.begin(csPin)) {
    Serial.println("initialization failed!");
    }
    else {
        Serial.println("initialization done.");
    }
}

//===================

void listFiles() {

    File root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("done! ----------");
    Serial.println();
}

//=================

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
