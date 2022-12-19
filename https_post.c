//----------------------------------------Include the NodeMCU ESP8266 Library
//----------------------------------------NodeMCU ESP8266 library
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//----------------------------------------

#define Led_OnBoard 2 //4pin
#define sensor_a 13
#define sensor_a_led 12

#define sensor_b 14
#define sensor_b_led 2

//----------------------------------------SSID and Password of your WiFi router.
const char* ssid = "Extra"; //--> Your wifi name or SSID.
const char* password = "extra20000910"; //--> Your wifi password.
const char fingerprint[] PROGMEM = "51f067026201f4fb917533b27febcc5cbbe0550d";
const int httpsPort = 443;
//----------------------------------------

//----------------------------------------Web Server address / IPv4
// If using IPv4, press Windows key + R then type cmd, then type ipconfig (If using Windows OS).
const char *host = "sui-prevention.bdrip.org";
//----------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  Serial.println("");
    
  pinMode(Led_OnBoard,OUTPUT); //連線燈號為輸出
  digitalWrite(Led_OnBoard, LOW);
    
  pinMode(sensor_a, INPUT_PULLUP); //紅外線A為為輸入
  pinMode(sensor_a_led, OUTPUT); //紅外線A燈號為輸出
  digitalWrite(sensor_a_led, LOW); //紅外線A燈號預設為LOW
  
  pinMode(sensor_b, INPUT_PULLUP); //紅外線B為輸入
  pinMode(sensor_b_led, OUTPUT); //紅外線B燈號為輸出
  digitalWrite(sensor_b_led, LOW); //紅外線B燈號預設為LOW

  //------------------------------------------
  Serial.print("Connecting"); //監控視窗連線訊息

  /*還沒連到WIFI就讓Led_OnBoard燈號一直閃*/
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print(".");
    //----------------------------------------
    digitalWrite(Led_OnBoard, LOW); //連線燈號預設為LOW
    delay(250); //延遲0.25秒
    digitalWrite(Led_OnBoard, HIGH); //連線燈號預設為HIGH
    delay(250); //延遲0.25秒
    //----------------------------------------
  }
  
  //------------------------------------------
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //------------------------------------------
}

void loop() {
  // put your main code here, to run repeatedly:
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  client->setFingerprint(fingerprint);
  HTTPClient https;
  //----------------------------------------Getting senser value to MySQL Database
  String LdrValueSend_1, postData_1, LdrIdSend_1;
  int ldrvalue_1;
  int Check_1 = 10;
  String postData_check_1, Check_val_1; 

  if(digitalRead(sensor_a) == HIGH){
      ldrvalue_1 = 1;
      digitalWrite(sensor_a_led, HIGH);
  }else{
      ldrvalue_1 = 0;
      digitalWrite(sensor_a_led, LOW);
  }
 
  if(ldrvalue_1 == 0){
    Check_val_1 = String(Check_1);
    postData_check_1 = "stat=" + Check_val_1 + "&id=C002";
    https.begin(*client, "https://sui-prevention.bdrip.org/update.php");
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode_check_1 = https.POST(postData_check_1);
    String payload_check_1 = https.getString();
    Serial.println("senser_check_1=" + Check_val_1);
    Serial.println(payload_check_1);
  }

  
  
  LdrValueSend_1 = String(ldrvalue_1);
  postData_1 = "stat=" + LdrValueSend_1 + "&id=C002";

  https.begin(*client, "https://sui-prevention.bdrip.org/update.php");
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode_1 = https.POST(postData_1);
  String payload_1 = https.getString();
  
  Serial.println(payload_1);
  Serial.println("senser_A=" + LdrValueSend_1);
  https.end();
  delay(500);  
  String LdrValueSend_2, postData_2, LdrIdSend_2;
  int ldrvalue_2;
  int Check_2 = 10;
  String postData_check_2, Check_val_2; 

  if(digitalRead(sensor_b) == HIGH){
      ldrvalue_2 = 1;
      digitalWrite(sensor_b_led, HIGH);
  }else{
      ldrvalue_2 = 0;
      digitalWrite(sensor_b_led, LOW);
  }
  
  if(ldrvalue_2 == 0){
    Check_val_2 = String(Check_2);
    postData_check_2 = "stat=" + Check_val_2 + "&id=C003";
    https.begin(*client, "https://sui-prevention.bdrip.org/update.php");
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode_check_2 = https.POST(postData_check_2);
      String payload_check_2 = https.getString();---------------------------------------------------------
    Serial.println("senser_check_2=" + Check_val_2);
    Serial.println(payload_check_2);
  }

  LdrValueSend_2 = String(ldrvalue_2);
  postData_2 = "stat=" + LdrValueSend_2 + "&id=C003";

  https.begin(*client, "https://sui-prevention.bdrip.org/update.php");
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode_2 = https.POST(postData_2);
  String payload_2 = https.getString();
  
  Serial.println(payload_2);
  Serial.println("senser_B=" + LdrValueSend_2);
  
  https.end();
  delay(500);
}
