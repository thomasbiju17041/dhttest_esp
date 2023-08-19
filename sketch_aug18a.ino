#include <Firebase_ESP_Client.h>
#include<DHT.h>
//#include<WiFi.h>
#include <ESP8266WiFi.h>
#define DHTPIN 13 
#define DHTTYPE DHT11
FirebaseAuth auth;
FirebaseConfig config; 
bool signupOK=false;
DHT dht(DHTPIN,DHTTYPE);
FirebaseData fbdo;
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
char ssid[] = "tomtom";  // type your wifi name
char pass[] = "tomtom17041";  // type your wifi password
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid,pass);
  Serial.print("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.println(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
  config.database_url= "https://esp32test-ccee2-default-rtdb.firebaseio.com/";
  config.api_key= "AIzaSyBXWJ2vCbjFN2C6S45kPHRhIeKCFEqhEqg";
  if(Firebase.signUp(&config,&auth,"","")){
    Serial.println("ok");
    signupOK=true;
  }
  else
  {
    Serial.printf("%s\n",config.signer.signupError.message.c_str());
  }
  config.token_status_callback=tokenStatusCallback;
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  if(Firebase.ready()&&signupOK){
    if(Firebase.RTDB.setFloat(&fbdo,"DHT_hum/humidity",h)){
      Serial.print("humidity");
      Serial.println(h);
    }
    else{
      Serial.println("FAILED");
      Serial.println("REASON: "+ fbdo.errorReason());
    }
    if(Firebase.RTDB.setFloat(&fbdo,"DHT_temp/temperature",t)){
      Serial.print("temperature");
      Serial.println(t);
    }
    else{
      Serial.println("FAILED");
      Serial.println("REASON: "+ fbdo.errorReason());
    }
    
    
  }
}
