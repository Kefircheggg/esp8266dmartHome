#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

DHT dht(D4, DHT11);

MDNSResponder mdns;

// Wi-Fi
const char* ssid = "TP-LINK_604C76";
const char* password = "Vnikifor238238";
const char* www_username = "Admin";
const char* www_password = "Admin";

byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,1,35);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int D0_pin = D0;
int D2_pin = D2;
int D1_pin = D1;


void setup(void){

  dht.begin();
  
  // preparing GPIOs
  pinMode(D0_pin, OUTPUT);
  digitalWrite(D0_pin, LOW);
  pinMode(D2_pin, OUTPUT);
  digitalWrite(D2_pin, LOW);
  pinMode(D1_pin, OUTPUT);
  digitalWrite(D1_pin, LOW);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, LOW);
 

  delay(100);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ArduinoOTA.begin();
  WiFi.config(ip, gateway, subnet);
  
 
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("Esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  


    server.on("/", [](){
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    server.send(200, "text/html", MainPage());
  });
  server.begin();

   //+++++++++++++++++++++++ WifiModal start ++++++++++++++++++++ 
  server.on("/WifiModal", [](){
    
    server.send(200, "text/html", Wifi_Modal());
    delay(100);    
  });
   //+++++++++++++++++++++++ WifiModal End ++++++++++++++++++++ 
  
  server.on("/socket4On", [](){
    digitalWrite(D3, HIGH);
    server.send(200, "text/html", MainPage());
    delay(100);    
  });
  server.on("/socket4Off", [](){
    digitalWrite(D3, LOW);
   server.send(200, "text/html", MainPage());
    delay(100);
  });  
   // +++++++++++++++++++++++ END  LED-1 ++++++++++++++++++++
    
    
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
  ArduinoOTA.handle();
} 

String Wifi_Modal() 
{
  String web;

    web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>KefirWebOs Wifi Settings</title>";
    web += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
    web += "";
    web += "<style>";
    web += "body{background-image: url(https://i.imgur.com/N68DFWn.jpg); background-repeat: no-repeat; background-position: center center;background-attachment: fixed;background-size: cover;}";
    web += ".container1{margin:1px; height:20px;background-color:#f8f9fa;border:1px solid #f8f9fa;border-radius:5px 5px 5px 5px;}";
    web += ".titlepage{font-size:15px;font-weight: bold}";
    web += "";
    web += "";
    web += "</style>";
    web += "</Head>";

    web +="<body>";
    web +="<div class=\"container-fluid container1\"><span class=\"titlepage\">KefirWebOS</span></div>";
    web +="";
    web +="";
    web +="";
    web +="";
    web +="";
    web +="";
    web +="</body>";
  return(web);
}




String MainPage()
{
  
  byte Temp = dht.readTemperature();
  String web; 
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>KefirWebOs</title>";
  
  web += "<style>";
  web += "body{background-image: url(https://i.imgur.com/N68DFWn.jpg); background-repeat: no-repeat; background-position: center center;background-attachment: fixed;background-size: cover;}";
  web += ".container1{margin:1px; height:20px;background-color:#f8f9fa;border:1px solid #f8f9fa;border-radius:5px 5px 5px 5px;}";
  web += ".buttinteface{margin-left:50px; margin-top:50px; }";
  web += ".glyphinterface{font-size:50pt;}";
  web += ".titlepage{font-size:15px;font-weight: bold}";
  web += ".InContainerIcon{float:right;}";
  web += "</style>";
  web += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
  web += "</head>";


  web += "<body>";
  web += "<div class=\"container-fluid container1\">";
  web += "<span class=\"titlepage\">KefirWebOS</span>";
  web += "<a href=\"WifiModal\"><button><span class=\"glyphicon glyphicon-signal InContainerIcon\" aria-hidden=\"true\"></span></button></a>";
  web += "";
  web +="</div>";

    //++++++++++ Modal ++++++++++++++
  web +="<div href=\"\" class=\"modal fade\"><div class=\"modal-dialog\"> <div class=\"modal-content\">";
  web +="<div class=\"modal-header\"><h4 class=\"modal-title\">Настройки Wi-fi</h4></div>";
  web +="<div class=\"modal-body\"> </div>";
  web +="<div class=\"modal-footer\">";
  web +="<button type=\"button\" class=\"btn btn-default\" data-dismiss=\"modal\">Закрыть</button><button type=\"button\" class=\"btn btn-primary\">Сохранить изменения</button></div></div></div></div>";
  web +="";
    //++++++++++ Modal ++++++++++++++

    //++++++++++ LED-3  +++++++++++++
  if(digitalRead(D3) == LOW) {
  web += "<a href=\"socket4On\"><button  type=\"button\" class=\"btn btn-danger btn-lg buttinteface\"><span class=\"glyphicon glyphicon glyphicon-lamp glyphinterface\"aria-hidden=\"true\"></span></button></a>";
    }
   if(digitalRead(D3) == HIGH) {
  web += "<a href=\"socket4Off\"><button  type=\"button\" class=\"btn btn-success btn-lg buttinteface\"><span class=\"glyphicon glyphicon glyphicon-lamp glyphinterface\"aria-hidden=\"true\"></span></button></a>";   
      }
  // ++++++++ LED-3 +++++++++++++

  // ++++++++ Refresh +++++++++++
  web += "<a href=\"/\"><button  type=\"button\"  class=\"btn btn-info buttinteface\" aria-label=\"Center Align\">";
  web += "<span class=\"glyphicon glyphicon-refresh glyphinterface\" aria-hidden=\"true\"></span>";
  web += "</button></a>";
  // ++++++++ Refresh +++++++++++
  //glyphicon glyphicon-signal
  web += "</body>";
  return(web);

}
