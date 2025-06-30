#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Lochnermobil";
const char* password = "lochner01";
//const char* ssid = "wlan-RH";
//const char* password = "laliwifi";
int value=0;
bool voltmar =0;

void setup () {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("W");    //wifi ok
  } 
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
  //*************************************
    
    if(Serial.available()>0){
      value = Serial.read();
      delay(100);
      if (value == 'S'){
        //****************************************
        HTTPClient http;  //Declare an object of class HTTPClient
 
        http.begin("http://www.kapow.co.uk/scripts/sendsms.php?%20username=joska2277&password=Joska0177&sms=szotyi%20teszt%20arduino&mobile=0036304594142");  //Specify request destination
        int httpCode = http.GET();                                                                  //Send the request
 
        if (httpCode > 0) { //Check the returning code
 
          String payload = http.getString();   //Get the request response payload
          if (voltmar==0){
            if (payload == "NOCREDIT"){
              Serial.println("0");
              voltmar = 1;
            }
            else{Serial.println("NEM");}
            if (payload == "sms kuldeskori válasz"){
              Serial.println("1");
              voltmar =0;
            }
          }
        }
        if (httpCode < 0) {
          Serial.println("httpCode: -1");
        }
        http.end();   //Close connection
      }
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
  delay(2000);
  Serial.println("NO");
  }
}
