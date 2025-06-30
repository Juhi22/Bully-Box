#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(10, 11); //RX,, TX
#include <Wire.h>

DFRobotDFPlayerMini myDFPlayer;
volatile bool audioEnabled = false;
volatile int trackNumber = 0;

void setup() {
  delay(1000); // Wait for DFPlayer mini to be usable
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  Wire.begin(3);
  Wire.onReceive(receiveEvent);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  // Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  myDFPlayer.volume(40);
}

void loop() {

  if(audioEnabled && trackNumber > 0) {
    playAudio(trackNumber);
    audioEnabled = false;
    trackNumber = 0;
  }

}

void playAudio(int track) {
  myDFPlayer.play(track);
  delay(200);
  int file = myDFPlayer.readCurrentFileNumber();
  while (file != track) {
    Serial.println("nononono");
    Serial.println(file);
    myDFPlayer.play(track);
    delay(200);
    file = myDFPlayer.readCurrentFileNumber();
  }
  //while(file == track) {
    //Serial.println("yesyesyes");
    //Serial.println(file);
    //delay(200);
    //file = myDFPlayer.readCurrentFileNumber();
  //}
  Serial.println(file);
  //isSoundOn = true;
  //Serial.println("vége");
}

void receiveEvent(byte lalletata) {
  int message = Wire.read();
  trackNumber = message;
  audioEnabled = true;
  /*if(message == 1){
    trackNumber = 1;
    audioEnabled = true;
  }
  else if(message == '2'){
    trackNumber = 2;
    audioEnabled = true;
  }
  else if(message == '3'){
    trackNumber = 3;
    audioEnabled = true;
  }
  else if(message == '4'){
    trackNumber = 4;
    audioEnabled = true;
  }
  else if(message == '5'){
    trackNumber = 5;
    audioEnabled = true;
  }*/
}
