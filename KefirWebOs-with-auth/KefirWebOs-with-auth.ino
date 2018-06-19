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

String MainPage()
{
  
  byte Temp = dht.readTemperature();
  String web; 
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>KefirWebOs</title>";
  web += "<script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>";
  web += "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/js/bootstrap.min.js\"></script>";
  web += "<style>";
  web += "body{background-image: url(https://i.imgur.com/N68DFWn.jpg); background-repeat: no-repeat; background-position: center center;background-attachment: fixed;background-size: cover;}";
  web += ".container1{margin:1px; height:20px;background-color:#f8f9fa;border:1px solid #f8f9fa;border-radius:5px 5px 5px 5px;}";
  web += ".buttinteface{margin-left:50px; margin-top:50px;}";
  web += ".titlepage{font-size:15px;font-weight: bold}";
  web += ".InContainerIcon{float:right;}";
  web += "";
  web += "</style>";
  web += "<link href=\"https://use.fontawesome.com/releases/v5.0.2/css/all.css\" rel=\"stylesheet\">";
  web += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.1/css/bootstrap.min.css'>";
  web += "</head>";


  web += "<body>";
  web += "<div class=\"container-fluid container1\">";
  web += "<span class=\"titlepage\">KefirWebOS</span>";
  web += "<i class=\"fas fa-signal InContainerIcon\"></i>";
  web += "";
  web +="</div>";

    //++++++++++ Modal ++++++++++++++
  web +="<div class=\"modal fade\" id=\"myModal\" tabindex=\"-1\" role=\"dialog\" aria-labelledby=\"myModalLabel\"> <div class=\"modal-dialog\" role=\"document\">";
  web +="<div class=\"modal-content\"><div class=\"modal-header\"><h4 class=\"modal-title\" id=\"myModalLabel\">Wifi Settings</h4></div>";
  web +="<div class=\"modal-body\">";
  web +="<p>SSID:"+String(ssid)+"</p>";
  web +="";
  web +="";
  web +="</div>";
  web +="<div class=\"modal-footer\"><button type=\"button\" class=\"btn btn-default\" data-dismiss=\"modal\">Close</button><button type=\"button\" class=\"btn btn-primary\">Save changes</button>";
  web +="</div></div></div></div>";
    //++++++++++ Modal ++++++++++++++

    //++++++++++ LED-3  +++++++++++++
  if(digitalRead(D3) == LOW) {
  web += "<a href=\"socket4On\"><button style=\"padding:8px 20.5px 8px 20.5px\" type=\"button\" class=\"btn btn-danger btn-lg buttinteface\"><i class=\"far fa-lightbulb fa-5x\"></i></button></a>";
    }
   if(digitalRead(D3) == HIGH) {
  web += "<a href=\"socket4Off\"><button style=\"padding:8px 20.5px 8px 20.5px\" type=\"button\" class=\"btn btn-success btn-lg buttinteface\"><i class=\"fas fa-lightbulb fa-5x\"></i></button></a>";   
      }
  // ++++++++ LED-3 +++++++++++++

  // ++++++++ Refresh +++++++++++
  web += "<a href=\"/\"><button style=\"padding:18px\" type=\"button\"  class=\"btn btn-info buttinteface\" aria-label=\"Center Align\">";
  web += "<i class=\"fas fa-sync-alt fa-5x\"></i>";
  web += "</button></a>";
  // ++++++++ Refresh +++++++++++

  //+++++++++ Modal ++++++++++++++
  web += "<button type=\"button\" style=\"padding:18px 8px\" class=\"btn btn-primary buttinteface\" aria-label=\"Center Align\" data-toggle=\"modal\" data-target=\"#myModal\">";
  web += "<i class=\"fas fa-wifi fa-5x\"></i>";
  web += "</button>";
  //+++++++++ Modal ++++++++++++++  

  web += "</body>";
  return(web);

}
