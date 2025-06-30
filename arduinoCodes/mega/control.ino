#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial esp(10, 11); // RX, TX
#include <Servo.h>
Servo myservo;

#include <SPI.h>
#include <RFID.h>

#define SDA_DIO 53
#define RESET_DIO 49
RFID RC522(SDA_DIO, RESET_DIO);

int szelep = 3;
int kukoricaajto = 4;
int vezetekajto = 5;
int zseleajto = 6;
int aramtalanito = 7;

int habnyomas = 10;               // HAB nyomási idő
int value;
int hang = 18;

int kukoricaajtonyitas = 22;
int vezetekajtonyitas = 23;
int zseleajtonyitas = 24;
int konfettiajtonyitas = 25;
int kulcsoskapcsolo = 26;
int vezetek = 27;
int inditorele = 28;

int pos = 180;

bool kukoricaajtoallapot = 0;
bool vezetekajtoallapot = 0;
bool zseleajtoallapot = 0;
bool konfettiajtoallapot = 0;
bool kulcsoskapcsoloallapot = 0;
bool vezetekallapot = 1;

bool kukajtoall =0;
bool vezajtoall =0;
bool zselajtoall =0;
bool konfajtoall =0;
bool kulcskapcsall =0;
bool vezetekall =0;
bool inditoreleall =0;

bool kukajtoallregi =0;
bool vezajtoallregi =0;
bool zselajtoallregi =0;
bool konfajtoallregi =0;
bool kulcskapcsallregi =0; 
bool vezetekallregi =0;
bool inditoreleallregi =0;

bool ajtoallapot1 =0;
bool ajtoallapot2 =0;
bool ajtoallapot3 =0;
bool ajtoallapot4 =0;

bool kutya = 1;
bool kutya2 = 1;
bool kutya3 = 0;
bool szelepmehet=0;
bool tagkeres=0;
bool aramtalanitas =0;

bool smsf = 0;
bool habf = 0;
bool kukoricaf = 0;
bool vezetekf = 0;
bool zselef = 0;
bool konfettiajtof = 0;

void setup() {
  Serial.begin(9600);
  esp.begin(115200);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(18, INPUT);
  pinMode(22, INPUT);
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(28, INPUT);
  
  digitalWrite (szelep,0);
  digitalWrite (kukoricaajto,1);
  digitalWrite (vezetekajto,1);
  digitalWrite (zseleajto,1);
  digitalWrite (aramtalanito,0);
  
  Wire.begin(2);
  Wire.onReceive(receiveEvent);

  myservo.attach(19);
  myservo.write(pos);
  
  SPI.begin();
  RC522.init();

  Serial.println ("reboot");
}

void loop() {
  //******************teszt****************
  if(Serial.available()>0){
    value = Serial.read();
    delay(5);
    if (value == '1'){
      delay(2000);
      digitalWrite (szelep,1);
      delay(1500);
      digitalWrite(szelep,0);
      Serial.println("egy");
    }
    if (value == '2'){
      for (pos = 180; pos >= 90; pos -= 1) {
        myservo.write(pos);             
        delay(5);         
      }
      delay (1000);
      for (pos = 90; pos <= 180; pos += 1) {
        myservo.write(pos);             
        delay(5);
      }
    }
    if (value == '3'){
      kukoricaajtoallapot = 0;
    }
    if (value =='4'){
      kommunikacio('K',1);
      Serial.println("nagy K ment");
    }
    if (value =='5'){
      kommunikacio(3,3);
      Serial.println("nano kommunikál");
    }
    if (value =='6'){
      esp.write("S");
    }
    if (value == '8'){
      digitalWrite (kukoricaajto,1);
    }
    if (value == '9'){
      digitalWrite(kukoricaajto,0);
    }
  }
/*Serial.print(kukajtoall);
  Serial.print(vezajtoall);
  Serial.print(zselajtoall);
  Serial.print(konfajtoall);
  Serial.println(kulcskapcsall);*/
//****************************************************** 



// ajtok nyitásérzékelője
  kukajtoall = digitalRead(kukoricaajtonyitas);
  {
    if(kukajtoall !=  kukajtoallregi)
    {
      delay(500); 
        if( kukajtoall == HIGH &&  kukajtoallregi == LOW){
        kukoricaajtoallapot = 1; 
        }
        else{kukoricaajtoallapot = 0;}
    }    
  }
  //---------------------------------------------------
  vezajtoall = digitalRead(vezetekajtonyitas);
  {
    if(vezajtoall !=  vezajtoallregi)
    {
      delay(500); 
        if( vezajtoall == HIGH &&  vezajtoallregi == LOW){
        vezetekajtoallapot = 1; 
        }
        else{vezetekajtoallapot = 0;}
    }    
  }
  //-----------------------------------------------------
  zselajtoall = digitalRead(zseleajtonyitas);
  {
    if(zselajtoall !=  zselajtoallregi)
    {
      delay(500); 
        if( zselajtoall == HIGH &&  zselajtoallregi == LOW){
        zseleajtoallapot = 1; 
        }
        else{zseleajtoallapot = 0;}
    }    
  }
  //-----------------------------------------------------
  konfajtoall = digitalRead(konfettiajtonyitas);
  {
    if(konfajtoall !=  konfajtoallregi)
    {
      delay(500); 
        if( konfajtoall == HIGH &&  konfajtoallregi == LOW){
        konfettiajtoallapot = 1; 
        }
        else{konfettiajtoallapot = 0;}
    }    
  }
  //-----------------------------------------------------
  kulcskapcsall = digitalRead(kulcsoskapcsolo);
  {
    if(kulcskapcsall !=  kulcskapcsallregi)
    {
      delay(500); 
        if( kulcskapcsall == LOW &&  kulcskapcsallregi == HIGH){
        kulcsoskapcsoloallapot = 1; 
        }
        else{kulcsoskapcsoloallapot = 0;}
    }    
  }
  //-----------------------------------------------------
  vezetekall = digitalRead(vezetek);
  {
    if(vezetekall !=  vezetekallregi)
    {
      delay(500); 
        if( vezetekall == HIGH &&  vezetekallregi == LOW){
        vezetekallapot = 0; 
        }
    }    
  }
  //-------------------------indítás----------------------
  inditoreleall = digitalRead(inditorele);
  {
    if(inditoreleall != inditoreleallregi)
    {
      delay(1000);
        if(inditoreleall == HIGH && inditoreleallregi == LOW){
          kommunikacio('W',1);
        }
    }
  }
  //------------------------------------------------------
  kukajtoallregi = kukajtoall;
  vezajtoallregi = vezajtoall;
  zselajtoallregi = zselajtoall;
  konfajtoallregi = konfajtoall;
  kulcskapcsallregi = kulcskapcsall;
  vezetekallregi = vezetekall;
  inditoreleallregi = inditoreleall;
  
//****************ajtók vezérlés**************************************
  //---kukorica ajtó
  if (kukoricaajtoallapot && !ajtoallapot1){      // kukorica ajtaját kinyitották
     kommunikacio(11,3);                           // .mp3 szamár hang lejátszás
     ajtoallapot1 = 1;                            // ide már nem lép vissza
     tagkeres = 1;
     delay(500);                                  // késleltet hogy a hang elinduljon
  }
  else if (ajtoallapot1 && !kukoricaajtoallapot && digitalRead(hang)){      // kukorica ajtaját becsukták
    Serial.print("TAGot megkapta");
    kommunikacio(7,3);                                // .mp3 keresd meg a kulcs helyét
    delay(800);
    digitalWrite(kukoricaajto,1);                     // kukorica ajt visszazárása
    ajtoallapot1 = 0;                                  //ide már nem lép vissza
  }
  if (tagkeres){                                        // TAG olvasás
    if (RC522.isCard()){
      RC522.readCardSerial();
      String content= "";
      for(int i=0;i<5;i++){
        content.concat(String(RC522.serNum[i], HEX));
      }
      content.toUpperCase();
      if (content == "1C18E517F6"){
        kutya3=1;
        tagkeres=0;
      }
    }
  }
  if (kutya3){
    Serial.print("kukorica vége");
    kommunikacio(12,3);                                 // .mp3 Folytasd a kvízt duma
    kommunikacio('K',1);                              // UNOnak "K" kukorica feladat vége 
    kutya3 = 0;                                         //ide már nem lép vissza
    
  }

//--- vezeték ajtó
  if (vezetekajtoallapot && !ajtoallapot2){            // vezeték ajtaját kinyitották
    kommunikacio(4,3);                               // instrukció a vezetékelvágázhoz vagy valami zenei aláfestés
    ajtoallapot2 = 1;                                 //ide már nem lép vissza
  }
  else if (ajtoallapot2 && !vezetekajtoallapot && !vezetekallapot){       //vezeték ajtaját becsukták
    Serial.print ("vezeték vége");
    delay(800);
    digitalWrite (vezetekajto,1);                     // vezeték ajto bezárása
    kommunikacio ('V',1);                             //UNOnak "V" vezeték feladat vége                   
    ajtoallapot2 = 0;                                  //ide már nem lép vissza
  }
  else if (ajtoallapot2 && !vezetekallapot && kutya2){           // a vezeték ellett-e vágva
    Serial.print ("vezeték elvágva");
    kommunikacio (5,3);                               // .mp3  hülye vagy rossz vezetéket vágtál csukd be az ajtót
    kutya2 = 0;                                       // ide csak 1-szer lép be
    
  }

//--- zselé ajtó
  if (zseleajtoallapot && !ajtoallapot3){            // zsele ajtaját kinyitották
    kommunikacio(7,3);                             // instrukció a kulcskereséshez vagy valami zenei aláfestés
    ajtoallapot3 = 1;                                 //ide már nem lép vissza
  }
  else if (ajtoallapot3 && !zseleajtoallapot && kulcsoskapcsoloallapot){       //zsele ajtaját becsukták
    Serial.print ("zselé vége");
    delay(800);
    digitalWrite (zseleajto,1);                                                // zselé ajto bezárása
    kommunikacio (8,3);                                                       //NANOnak kacaj 
    for (pos = 180; pos >= 90; pos -= 1) {                                     // hab nyomása
        myservo.write(pos);             
        delay(5);         
      }
    delay (habnyomas*1000);                                                             // hab ennyi ideig jön (milisec)
    for (pos = 90; pos <= 180; pos += 1) {                                     //hab nyomás vége
        myservo.write(pos);             
        delay(5);
    }
    kommunikacio ('Z',1);                                                      //UNOnak "Z" Zselé feladat vége                 
    ajtoallapot3 = 0;                                                          //ide már nem lép vissza
  }
  else if (ajtoallapot3 && kulcsoskapcsoloallapot && kutya){
    Serial.print ("tetejet becsukni");              
    kommunikacio (16,3);                            // .mp3 tetejét csukd be duma
    kutya = 0;                                      // ide csak 1-szer lép be
  }

//--- konfetti ajto
/*if (konfettiajtoallapot && !ajtoallapot4){            // konfetti ajtaját kinyitották
    kommunikacio(20,3);                             // instrukció a konfettihez vagy valami zenei aláfestés
    ajtoallapot4 = 1;                                 //ide már nem lép vissza
  }
  else if (ajtoallapot4 && !konfettiajtoallapot){       //konfetti ajtaját becsukták
    digitalWrite (konfettiajto,1);                     // konfetti ajto bezárása
    kommunikacio ('F',1);                             //UNOnak "F" konfetti feladat vége
    Serial.print ("konfetti vége");                   
    ajtoallapot4 = 0;                                  //ide már nem lép vissza
  }*/

//************************* Ajtok nyitása ***************************************
//---vezeték ajtó
if (vezetekf || zselef || kukoricaf || konfettiajtof || szelepmehet || konfettiajtof){
delay (1000);
if (vezetekf && digitalRead(hang)){       // kaptam "V" UNOtol
  Serial.print("V");
  delay (2000);
  digitalWrite(vezetekajto, 0); // kizárja a vezeték ajtaját
  kommunikacio(3,3);            // .mp3 ajto nyitva szöveg
  vezetekf = 0;                 // ide már nem lép vissza
}

//--- zselé ajtó
if (zselef && digitalRead(hang)){       // kaptam "V" UNOtol
  Serial.print("ZS");
  delay (2000);
  digitalWrite(zseleajto, 0); // kizárja a vezeték ajtaját
  kommunikacio(6,3);         // .mp3 ajto nyitva szöveg
  zselef = 0;                 // ide már nem lép vissza
}

//--kukorica ajto
if (kukoricaf && digitalRead(hang)){        // Kaptam "K" UNOtol
  Serial.print("K");
  delay (2000);
  digitalWrite(kukoricaajto, 0); // kizárja a kukorica ajtaját
  kommunikacio(10,3);           // .mp3 ajto nyitva szöveg
  kukoricaf=0;                  // ide már nem lép vissza
}

//--- konfetti ajtó
if (konfettiajtof && digitalRead(hang)){
  Serial.print("outro");
  delay (2000);
  kommunikacio(13,3);           // .mp3 outro
  konfettiajtof = 0;            //ide már nem lép vissza
  szelepmehet = 1;              // ha vége a hangnak intézi az ajtó nytást és szelepkilövést
  delay (2000);
}

//******** konfetti ajtó + szelep finálé ****
if (szelepmehet && digitalRead(hang)){
  Serial.print ("PUKKK");
  //digitalWrite(konfettiajto, 0);    //kinyitja a konfetti ajtaját
  delay(800);
  digitalWrite (szelep,1);          // szelep kinyit
  delay(1000);
  digitalWrite(szelep,0);           // szelep bezár
  kommunikacio(14,3);               // .mp3 celebration
  kommunikacio('F',1);              // UNO-nak F finálé vége alkalmazás bezárásához
  aramtalanitas=1;                  // áramtalanításnak engedélyt ad ha vége a celebrationnak
  szelepmehet =0;                   // ide már nem lép vissza
  delay(1000);                      // késleltet legyen hang az áramtalanítás vizsgálat előtt
}
}
if (aramtalanitas && digitalRead(hang)){
  digitalWrite (aramtalanito,1);              // ITT A VÉGE MINDENNEK! Elveszi a tápot mindentől.
}
//************* ESP dolgok ************
  if (smsf){
    Serial.println("S kérés");
    sms();
    delay(2000);
  }
  
  if (esp.available()){
    int valasz = esp.read();
    Serial.write(valasz);
    if (valasz == '0'){
      Serial.println ("S OK");
      smsf = 0;
      kommunikacio(2,3);             // .mp3 elküldtem az üzenetet
    }
    /*if (valasz == 'W'){             //wifi ok UNOnak
      kommunikacio ('W',1);
    }*/
  }
}

//****************** Wire fogadás**********************************
void receiveEvent (byte lallee){
  byte message = Wire.read();
  if (message  == 'S'){
    smsf = 1;
  }
  if (message == 'K'){  //kukorica feladat
    kukoricaf = 1;   
  }
  if (message == 'H'){  //hab feladat
    habf = 1;   
  }
  if (message == 'V'){  //vezetek feladat
    vezetekf = 1;   
  }
  if (message == 'Z'){  // zsele feladat
    zselef = 1;   
  }
  if (message == 'F'){  // konfetti feladat
    konfettiajtof = 1;   
  }
}

//************** SMS küldés **********
void sms(){
  esp.write("S");
}

//**************  Wire küldés************
void kommunikacio(byte message, int id) {
  Serial.println(message);
  Wire.beginTransmission(id);
  Wire.write(message);
  Wire.endTransmission();
}
