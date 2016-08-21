/****************************************************************************************************************
 * **************************************************************************************************************
 *  Fecha: 21/08/2016       Autor: ABarrosoG         
 *  
 *  Material: WeMos D1 Mini + Led emisor 940nm / 5mm
 *  
 *  Objetivo: 
 *            -Primero.- ON / OFF del Aire Acondicionado y establecer valores de temperatura sin estar presente.
 *            -Segundo.- La finalidad de la versión WebServer es tomar datos del control solar, para cuando 
 *                        existan excedentes fotovoltáicos regule automáticamente la maquina de aire acondicoinado.
 *                        
 *  Conexión:   -  La conexión del led IR se realiza al GPIO4
 *                 
 * LICENCIA DE USO APACHE, si mejoras el programa o añades funcionalidades, por favor, compártelo!
 * 
 * LICENSE APACHE http://www.apache.org/licenses/
 * ***************************************************************************************************************
 * *************************************************************************************************************** */
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>

/*
// WLAN data
const char* ssid = "xxxxxxxxx";
const char* password = "xxxxxxxxxxxx";*/

bool conectado = false;

// static IP
IPAddress ip(192,168,1,110); //establece ip fija
IPAddress gateway(192,168,1,1);//puerta de enlace
IPAddress subnet(255,255,255,0);
MDNSResponder mdns;
ESP8266WebServer server(80);

// establece el pin IR GPIO
IRsend irsend(0);

void handleRoot() {
 String mensaje = "<html><body style=\"background-color:#DFDFDF\"><head>";
  mensaje += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  mensaje += "<meta http-equiv=\"Cache-Control\" content=\"no-cache, no-store, must-revalidate\" />";
  mensaje += "<meta http-equiv=\"Pragma\" content=\"no-cache\" />";
  mensaje += "<meta http-equiv=\"Expires\" content=\"0\" />";
  mensaje += "<meta http-equiv=\"refresh\" content=\"10a; URL=/\" >";
  mensaje += "<title>WebServer LG</title>";
  mensaje += "</head>";
  mensaje += "<body>";
  mensaje += "<center><font color=blue><h2>LG</h2></font></center>";
  mensaje += "<p><a href=\"ir?code=142608169&bits=28&protocol=LG\">Send cool 22</a></p>";
  mensaje += "<p><a href=\"ir?code=142608426&bits=28&protocol=LG\">Send cool 23</a></p>";
  mensaje += "<p><a href=\"ir?code=142608683&bits=28&protocol=LG\">Send cool 24</a></p>";
  mensaje += "<p><a href=\"ir?code=142608940&bits=28&protocol=LG\">Send cool 25</a></p>";
  mensaje += "<p><a href=\"ir?code=142609197&bits=28&protocol=LG\">Send cool 26</a></p>";
  mensaje += "<p><a href=\"ir?code=142609454&bits=28&protocol=LG\">Send cool 27</a></p>";
  mensaje += "<p><a href=\"ir?code=143392849&bits=28&protocol=LG\">Send cool 28</a></p>";
  mensaje += "<p><a href=\"ir?code=142624557&bits=28&protocol=LG\">Send heat 22</a></p>";
  mensaje += "<p><a href=\"ir?code=142624814&bits=28&protocol=LG\">Send heat 23</a></p>";
  mensaje += "<p><a href=\"ir?code=142625071&bits=28&protocol=LG\">Send heat 24</a></p>";
  mensaje += "<p><a href=\"ir?code=142625312&bits=28&protocol=LG\">Send heat 25</a></p>";
  mensaje += "<p><a href=\"ir?code=142625569&bits=28&protocol=LG\">Send heat 26</a></p>";
  mensaje += "<p><a href=\"ir?code=142625826&bits=28&protocol=LG\">Send heat 27</a></p>";
  mensaje += "<p><a href=\"ir?code=142626083&bits=28&protocol=LG\">Send heat 28</a></p>";
  mensaje += "<p><a href=\"ir?code=143392849&bits=28&protocol=LG\">Send OFF</a></p>";
  mensaje += "</body></html>\n";
  server.send(200, "text/html", mensaje);
}
void handleIr(){
  String codestring=server.arg("code");
  String protocol=server.arg("protocol");
  String bitsstring=server.arg("bits");
  String webOutput = "Protocol: "+protocol+"; Code: "+codestring+"; Bits: "+bitsstring;
  if ((codestring != "")&&(bitsstring != "")){
    unsigned long code = codestring.toInt();
    int bits = bitsstring.toInt();
    if (protocol == "NEC"){
      server.send(200, "text/html", webOutput);
      irsend.sendNEC(code, bits);
    }
    else if (protocol == "Sony"){
      server.send(200, "text/html", webOutput);
      irsend.sendSony(code, bits);
    }
    else if (protocol == "Whynter"){
      server.send(200, "text/html", webOutput);
      irsend.sendWhynter(code, bits);
    }
    else if (protocol == "LG"){
      server.send(200, "text/html", webOutput);
      irsend.sendLG(code, bits);
    }
    else if (protocol == "RC5"){
      server.send(200, "text/html", webOutput);
      irsend.sendRC5(code, bits);
    }
    else if (protocol == "RC6"){
      server.send(200, "text/html", webOutput);
      irsend.sendRC6(code, bits);
    }
    else if (protocol == "DISH"){
      server.send(200, "text/html", webOutput);
      irsend.sendDISH(code, bits);
    }
    else if (protocol == "SharpRaw"){
      server.send(200, "text/html", webOutput);
      irsend.sendSharpRaw(code, bits);
    }
    else if (protocol == "Samsung"){
      server.send(200, "text/html", webOutput);
      irsend.sendSAMSUNG(code, bits);
    }
    else {
      server.send(200, "text/html", "Protocolo no implementado!");
    }
  }
  else {
    server.send(200, "text/html", "Cual es el codigo o numero de bits?");
  }
  handleRoot();
}
void handleNotFound(){
  server.send(404, "text/plain", "Error: 404. - Expresion no aplicable");
}
void setup(void){
  irsend.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  Serial.println("");
  delay(100);
   
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
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/ir", handleIr);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void loop(void){
  server.handleClient();
}
