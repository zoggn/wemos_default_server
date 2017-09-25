#define _ssid "EspD1mini_00"

// ESP-8266MOD D1 mini
#define _RfLed 2// On-LOW, Off-HIGH 

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

boolean RfLed;
boolean BfBlink;

String formHTML(){
  String vRf; if (RfLed) vRf="Off"; else vRf="On";
  String vBf; if (BfBlink) vBf="Off"; else vBf="On";
  String vTime=String(millis()/1000,DEC);

  String form =""
  "<html>\n"
  "<head>\n"
  "<style>\n"
  "body,table,button,input {font-size:30px} \n"
  "</style>\n"
  "<script>\n"
  "Run1=false; \n"
  "count1=0; \n"
  "function Loop1(){\n"     
  "ct1.value=++count1;\n"
  "if (Run1) setTimeout(Loop1,1000);}\n"
  "function setTimer1(){\n"
  "if (!Run1) {\n"
    "Run1=true;\n"
    "Loop1();\n"
    "bt1.value='Stop';\n"
  "}else{\n"
    "Run1=false;\n"
    "bt1.value='Start';}}\n"
  "</script>\n"
  "</head>\n" 
  "<body>\n"
  "<h4 align=center>ESP8266 D1 mini board</h4>\n"
  "Run time is " + vTime + " seconds\n" 
  "<form action='act' align=center>\n"
  "RF-Led: <button name='Rf' value='"+vRf+"'>"+vRf+"</button>\n"
  "BF-Blink: <button name='Bf' value='"+vBf+"'>"+vBf+"</button>\n"
  "<input type=text id=ct1>\n"
  "<input type=button id=bt1 value='Start' onClick=setTimer1()>\n"
  "</form>\n"
  "</body>\n"
  "</html>";
  return (form);
}//formHTML

void handle_root() {
  server.send(200,"text/html",formHTML());
  Serial.println("handle_root");
}//handle_root

void handle_act() {
  RfLed=(server.arg("Rf")=="On");
  setRfLed(RfLed);
  BfBlink=(server.arg("Bf")=="Off");
  setBfBlink(BfBlink); 
  server.send(200,"text/html",formHTML());
  Serial.println("handle_act");
}//handle_led

void setup(void) {
  Serial.begin(115200);
  Serial.println("");

  pinMode(_RfLed,OUTPUT); 
  setRfLed(0);
  setBfBlink(0);

  //This needs only at first time to disable Station mode
  //WiFi.mode(WIFI_STA); WiFi.disconnect(); //Disable Station Mode
  delay(100);
  WiFi.softAP(_ssid);
  Serial.println(WiFi.softAPIP());
   
  server.on("/", handle_root);
  server.on("/act", handle_act);
  //server.onNotFound(handle_root);
  
  server.begin();
  Serial.println("HTTP server started");
}//setup

void loop(void) {
  server.handleClient();
}//loop

void setRfLed(boolean LedOn){
  if (LedOn) digitalWrite(_RfLed,LOW); 
  else digitalWrite(_RfLed,HIGH); 
}//setRfLed

void setBfBlink(boolean BlinkOn){
  while(BlinkOn)
  {
  digitalWrite(_RfLed,LOW);
  delay(1000);
  digitalWrite(_RfLed,HIGH);
  delay(1000);
  } 
}
