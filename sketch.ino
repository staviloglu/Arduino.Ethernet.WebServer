/*
  Web Server

 A simple web server to control a relay over web

 Circuit:
 * Ethernet shield
 * Analog inputs attached to pins A0 through A5 (optional)

 created 17.05.2017
 by Sinan Taviloglu

 */

#include <SPI.h> 
#include <Ethernet.h>

//ouput pin for relay control
const int RelayPin = 2;

//constant values for relay state
const int OPEN = 1;
const int CLOSED = 0;

int _relayState = OPEN;

//set mac address
byte mac[] = {
  0xDE,
  0xAD,
  0xBE,
  0xEF,
  0xFE,
  0xED
};
//set static ip
byte ip[] = {
  192,
  168,
  0,
  20
};

byte gateway[] = {
  192,
  168,
  0,
  1
}; 

byte subnet[] = {
  255,
  255,
  255,
  0
};

//standard HTTP port
EthernetServer server(80); 

String readString;

void setup() {
  pinMode(RelayPin, OUTPUT);
    
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop() {
  
  EthernetClient client = server.available();

  if (client) 
  {
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        
        if (readString.length() < 100) 
        {        
          readString += c;
        }
        
        if (c == '\n') 
        {
          
          if (readString.indexOf("?on") > 0) 
          {
            if (_relayState == CLOSED)
            {
              OpenRelay();
            }
          }

          if (readString.indexOf("?off") > 0) 
          {
            if (_relayState == OPEN)
              CloseRelay();
          }
          
          readString = "";

          SendHttpResponse(client);

          delay(1);
          
          client.stop();
        }
      }
    }
  }
}

void SendHttpResponse(EthernetClient client) 
{
  client.println("HTTP/1.1 200 OK"); 
  client.println("Content-Type: text/html");
  client.println();
  client.println("<HTML>");
  client.println("<HEAD>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("<link rel='stylesheet' type='text/css' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' />");
  client.println("<TITLE>Online Relay Control - Sinan TAVILOGLU</TITLE>");
  client.println("</HEAD>");
  client.println("<BODY>");
  client.println("<H1>Online Relay Control</H1>");
  client.println("<hr />");
  client.println("<br />");

  if (_relayState == OPEN)
    client.println("<a href=\"/?off\"\">Close</a><br />");
  else
    client.println("<a href=\"/?on\"\">Open</a>");

  client.println("<br />");
  client.println("<br />");
  client.println("<p>Sinan TAVILOGLU - staviloglu@gmail.com</p>");
  client.println("<p>Visit <a href='http://about.me/sinantaviloglu'>http://about.me/sinantaviloglu</a></p>");
  client.println("</BODY>");
  client.println("</HTML>");
}

void OpenRelay() {
  //may change according to relay, if it is NC/NO
  digitalWrite(RelayPin, LOW);
  _relayState = OPEN;
}

void CloseRelay() {
  //may change according to relay, if it is NC/NO
  digitalWrite(RelayPin, HIGH);
  _relayState = CLOSED;
}
