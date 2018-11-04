

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <SoftwareSerial.h>

#define WIFINAME  "JURA"

#define GPIORX    12
#define GPIOTX    13


byte z0, z1, z2, z3;
byte x0, x1, x2, x3, x4;
byte d0, d1, d2, d3;
byte intra = 1, inter = 7;
String hexval;


SoftwareSerial softserial(GPIORX, GPIOTX);
ESP8266WebServer webserver(80);

//write to nura
String cmd2jura(String outbytes) {
  String inbytes;
  int w = 0;

  while (softserial.available()) {
    softserial.read();
  }

  outbytes += "\r\n";
  for (int i = 0; i < outbytes.length(); i++) {
    for (int s = 0; s < 8; s += 2) {
      char rawbyte = 255;
      bitWrite(rawbyte, 2, bitRead(outbytes.charAt(i), s + 0));
      bitWrite(rawbyte, 5, bitRead(outbytes.charAt(i), s + 1));
      softserial.write(rawbyte);
    }
    delay(8);
  }

  int s = 0;
  char inbyte;
  while (!inbytes.endsWith("\r\n")) {
    if (softserial.available()) {
      byte rawbyte = softserial.read();
      bitWrite(inbyte, s + 0, bitRead(rawbyte, 2));
      bitWrite(inbyte, s + 1, bitRead(rawbyte, 5));
      if ((s += 2) >= 8) {
        s = 0;
        inbytes += inbyte;
      }
    } else {
      delay(10);
    }
    if (w++ > 500) {
      return "";
    }
  }

  return inbytes.substring(0, inbytes.length() - 2);
}



void handle_command() {
  String cmd;
  String result;

  if (webserver.method() != HTTP_POST) {
    webserver.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  cmd = webserver.arg("plain");
  if (cmd.length() < 3) {
    webserver.send(400, "text/plain", "Bad Request");
    return;
  }


  result = cmd2jura(cmd);

  if (result.length() < 3) {
    webserver.send(503, "text/plain", "Service Unavailable");
    return;
  }

  webserver.send(200, "text/plain", result);
}

//commands used to get from jura
void handle_status() {
  String cmd;

  if (webserver.method() != HTTP_POST) {
    webserver.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String input = "";
  String onOff = "";
  String status = "";
  cmd2jura("RR:03\r\n");
  delay(100);
  
  input = fromCoffeemaker();
  
  onOff = input.substring(4, 5);
  status = input.substring(6, 7);
  status += input.substring(25, 28);

  webserver.send(200, "text/json", "{onOFF:"+onOff+",status:"+status+"}");
}

String fromCoffeemaker() {
  delay(10);
  String inputString = "";
  byte d0, d1, d2, d3;
  char d4 = 255;
  while (softserial.available() > 0) {
    d0 = softserial.read();
    delay (1);
    d1 = softserial.read();
    delay (1);
    d2 = softserial.read();
    delay (1);
    d3 = softserial.read();
    delay (7);
    bitWrite(d4, 0, bitRead(d0, 2));
    bitWrite(d4, 1, bitRead(d0, 5));
    bitWrite(d4, 2, bitRead(d1, 2));
    bitWrite(d4, 3, bitRead(d1, 5));
    bitWrite(d4, 4, bitRead(d2, 2));
    bitWrite(d4, 5, bitRead(d2, 5));
    bitWrite(d4, 6, bitRead(d3, 2));
    bitWrite(d4, 7, bitRead(d3, 5));
    inputString += d4;
  }
  inputString.trim();
  if (inputString != "") {
    return (inputString);
  }
}

void handle_stats(){
   if (webserver.method() != HTTP_POST) {
    webserver.send(405, "text/plain", "Method Not Allowed");
    return;
  }
  
  int espresso = getCounter(0x280);
  int ristretto = getCounter(0x281);
  int koffie = getCounter(0x282);
  int cappuccino = getCounter(0x284);
  int latte_macchiato = getCounter(0x285);
  int macchiato = getCounter(0x286);
  webserver.send(200, "text/text", "espresso:"+espresso);
}

int getCounter(int offset)
{
  if(offset > 0x800) return -1;

  hexval = String(offset, HEX);
  while(hexval.length() < 3) hexval = "0" + hexval;

  toCoffeemaker('R'); delay(inter);
  toCoffeemaker('E'); delay(inter);
  toCoffeemaker(':'); delay(inter);
  toCoffeemaker('0'); delay(inter);
  toCoffeemaker(hexval[0]); delay(inter);
  toCoffeemaker(hexval[1]); delay(inter);
  toCoffeemaker(hexval[2]); delay(inter);
  toCoffeemaker(0x0D); delay(inter);
  toCoffeemaker(0x0A); delay(100);

  String r = "";

  while(Serial1.available()) {
    delay (intra); d0 = softserial.read();
    delay (intra); d1 = softserial.read();
    delay (intra); d2 = softserial.read();
    delay (intra); d3 = softserial.read();
    delay (inter);
    r += char(fromCoffeemaker(d0,d1,d2,d3));
  }

  if (r.length() == 9) {
    String hex = r.substring(3,7);
    int number = (int)strtol(hex.c_str(), NULL, 16);
    return number;
  } else {
    return -1;
  }
}

byte fromCoffeemaker(byte x0, byte x1, byte x2, byte x3) {
  bitWrite(x4, 0, bitRead(x0,2));
  bitWrite(x4, 1, bitRead(x0,5));
  bitWrite(x4, 2, bitRead(x1,2));
  bitWrite(x4, 3, bitRead(x1,5));
  bitWrite(x4, 4, bitRead(x2,2));
  bitWrite(x4, 5, bitRead(x2,5));
  bitWrite(x4, 6, bitRead(x3,2));
  bitWrite(x4, 7, bitRead(x3,5));
  return x4;
}

byte toCoffeemaker(byte z) {
  z0 = 255;
  z1 = 255;
  z2 = 255;
  z3 = 255;

  bitWrite(z0, 2, bitRead(z,0));
  bitWrite(z0, 5, bitRead(z,1));
  bitWrite(z1, 2, bitRead(z,2));
  bitWrite(z1, 5, bitRead(z,3));
  bitWrite(z2, 2, bitRead(z,4));
  bitWrite(z2, 5, bitRead(z,5));
  bitWrite(z3, 2, bitRead(z,6));
  bitWrite(z3, 5, bitRead(z,7));

  delay(intra); softserial.write(z0);
  delay(intra); softserial.write(z1);
  delay(intra); softserial.write(z2);
  delay(intra); softserial.write(z3);
  delay(inter);
}
void handle_web() {
  String html;

  html  = "<!DOCTYPE html><html><body><h1>&#9749; Jura Coffee Machine Gateway</h1>";
  html += "<form onsubmit=\"return s(this)\"><input type=\"text\" placeholder=\"enter command\" id=\"c\" list=\"l\" />";
  html += "<input type=\"submit\" /><datalist id=\"l\"><option value=\"TY:\"><option value=\"AN:01\">";
  html += "<option value=\"AN:02\"><option value=\"RT:00\"><option value=\"RR:00\"></datalist></form>";
  html += "<ul style=\"font-family: monospace\" id=\"r\"></ul><script>function s(f) { var x = new XMLHttpRequest();";
  html += "x.open('POST', '/api', true); x.onreadystatechange = function() { if(x.readyState === XMLHttpRequest.DONE && ";
  html += "x.status === 200) { var r = document.getElementById('r'); r.innerHTML = '<li>' + Date().toLocaleString() + ";
  html += "':&emsp;' + f.c.value + '&emsp;&#8594;&emsp;' + x.responseText + '</li>' + r.innerHTML; }}; x.send(f.c.value);";
  html += "return false;}</script></body></html>";

  webserver.send(200, "text/html", html);
}

void setup() {
  // put your setup code here, to run once:

  wifi_station_set_hostname(WIFINAME);

  WiFiManager wifimanager;
  wifimanager.autoConnect(WIFINAME);

  webserver.on("/", handle_web);
  webserver.on("/api/command", handle_command);
  webserver.on("/api/status", handle_status);
  //webserver.on("/api/stats", handle_stats);
  webserver.begin();

  softserial.begin(9600);


}

int i = 0;
void loop() {
  // put your main code here, to run repeatedly:

  webserver.handleClient();

}
