#include <Wire.h>

int Agomb = 2;
int Bgomb = 3;
int Cgomb = 4;
int btStatus = 5;
int audioStatus = 6;
int Agombstat = LOW;
int Bgombstat = LOW;
int Cgombstat = LOW;
char jogomb = 'D';
int kvizfeladatmegoldas[3];
int kvizsorszama = 0;
int Agombstatregi = LOW;
int Bgombstatregi = LOW;
int Cgombstatregi = LOW;
char quizMessage = 'X';
bool btReconnected = false;
bool isPopUpOn = false;
bool introPlayed = false;
bool smsSent = false;
bool isSoundOn = false;
volatile bool espReady = false;

void setup() {
  Serial.begin(9600);
  pinMode(Agomb, INPUT);
  pinMode(Bgomb, INPUT);
  pinMode(Cgomb, INPUT);
  pinMode(btStatus, INPUT);
  pinMode(audioStatus, INPUT);
  Wire.begin(1);
  Wire.onReceive(receiveEvent);

  setQuizToNull();
}

void loop() {

  if(!digitalRead(btStatus)) {
    btReconnected = false;
    if(!introPlayed && espReady) {
      //TODO need this delay here? it is here cuz waiting other arduinos to be ready but no it works with wire
      delay(1000);
      executeTask(1, 3);
      introPlayed = true;
    }
    //TODO need this delay here?
    delay(5000);
    //Serial.println(introPlayed && !smsSent && digitalRead(audioStatus));
    if(introPlayed && !smsSent && digitalRead(audioStatus)) {
      executeTask('S', 2);
      smsSent = true;
    }
  } else {
    if(!btReconnected) {
      if(isPopUpOn) {
        //Serial.println("quiz_number:" + String(kvizsorszama + 1));
      } else {
        //Serial.println("quiz_number:" + String(kvizsorszama));
      }
      btReconnected = true;
    }
    if(Serial.available()>0){

      byte receivedByte = Serial.read();

      if(receivedByte == 'R') {
        Serial.println("quiz_number:" + String(kvizsorszama));
      } else if (receivedByte == 'V' || receivedByte == 'H' || receivedByte == 'A' || receivedByte == 'Z' || receivedByte == 'K' || receivedByte == 'F') {
        if(!isPopUpOn) {
          isPopUpOn = true;
          kvizsorszama = kvizsorszama + 1;
          executeTask(receivedByte, 2);
        }
        setQuizToNull();
      } else {
        int value = (int)receivedByte - 48;
  
        //TODO mi ez?
        delay(80);
      
        if(value >= 0) {
          if(kvizfeladatmegoldas[0] == 254){
            kvizfeladatmegoldas[0] = value;
            kvizsorszama = kvizfeladatmegoldas[0];
          }
          else if(kvizfeladatmegoldas[1] == 254){
            kvizfeladatmegoldas[1]= value;
            if(kvizsorszama == 0) {
              kvizsorszama = kvizfeladatmegoldas[1];
            } else {
              kvizsorszama = kvizsorszama * 10 + kvizfeladatmegoldas[1];
            }
          }
          else if(kvizfeladatmegoldas[2] == 254){
            //TODO do false in another place? I guess yeah....
            setgood(value);
          }
        }
      }
    }

    if(!isPopUpOn && digitalRead(audioStatus)) {
      Agombstat = digitalRead(Agomb);
      Bgombstat = digitalRead(Bgomb);
      Cgombstat = digitalRead(Cgomb);
    }
  
    if(Agombstat == HIGH){
      buttonHandle(Agombstat, Agombstatregi, 'A');
    }
  
    if(Bgombstat == HIGH){
      buttonHandle(Bgombstat, Bgombstatregi, 'B');
    }
  
    if(Cgombstat == HIGH){
      buttonHandle(Cgombstat, Cgombstatregi, 'C');
    }
    
    Agombstatregi = Agombstat;
    Bgombstatregi = Bgombstat;
    Cgombstatregi = Cgombstat;
  }
}
    
void setgood(int button){
  if (button==0){
    jogomb='A';
  }
  else if(button==1){
    jogomb='B';
  }
  else if(button==2){
    jogomb='C';
  }
}

void buttonHandle(bool gombstat, bool gombstatregi, char gomb){
  if(gombstat != gombstatregi){
    //TODO added to later if necessary remove this delay
    delay(500);
    if( gombstat != gombstatregi ) {
      if(jogomb == gomb){
          //JÓ
          executeTask(rand()%(41-21)+21, 3);
          sendXAndReset();
      } else {
          //ROSSZ
          executeTask(rand()%(61-41)+41, 3);
          sendXAndReset();
      }
    }
  }
}

void sendXAndReset(){
  Serial.println(quizMessage);
  setQuizToNull();
  isPopUpOn = false;
}

void setQuizToNull() {
  kvizfeladatmegoldas[0]=254;
  kvizfeladatmegoldas[1]=254;
  kvizfeladatmegoldas[2]=254;
}

void executeTask(byte message, int id) {
  Wire.beginTransmission(id);
  Wire.write(message);
  Wire.endTransmission();
}

void receiveEvent(byte lalletata) {
  byte message = Wire.read();
  if(message == 'W') {
    espReady = true;
  }
  else if(message == 'F') {
    Serial.println("end_it");
  }
   else if (message == 'V' || message == 'H' || message == 'A' || message == 'Z' || message == 'K') {
    finishTask();
  }
}

void finishTask() {
  kvizsorszama = kvizsorszama + 1;
  sendXAndReset();
}
