//#include <ESP8266WiFi.h>
//#include "DHT.h"
//
//#define DHTPIN D1
//#define DHTTYPE DHT11 
//
//const char* ssid = "Rahul2122";
//const char* password = "gocorona321";
//
//// Create an instance of the server
//// specify the port to listen on as an argument
//WiFiServer server(80);
//DHT dht(DHTPIN, DHTTYPE);
//
//void setup() {
//  Serial.begin(115200);
//  delay(10);
//  dht.begin();
//  // Connect to WiFi network
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  
//  WiFi.begin(ssid, password);
//  
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  
//  // Start the server
//  server.begin();
//  Serial.println("Server started");
//
//  // Print the IP address
//  Serial.println(WiFi.localIP());
//}
//
//
//void loop() {
//  
//  // Check if a client has connected
//  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }
//  
//  // Wait until the client sends some data
//  Serial.println("new client");
//  while(!client.available()){
//    delay(1);
//  }
//  
//  // Read the first line of the request
//  float h = dht.readHumidity();
//  // Read temperature as Celsius (the default)
//  float t = dht.readTemperature();
//
//
//  // Check if any reads failed and exit early (to try again).
//  if (isnan(h) || isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }
//
//  String req = client.readStringUntil('\r');
//  Serial.println(req);
//  client.flush();
//  
//  if (req.indexOf("/data") != -1){
//    client.flush();
//    client.println("HTTP/1.1 200 OK");           // This tells the browser that the request to provide data was accepted
//    client.println("Access-Control-Allow-Origin: *");  //Tells the browser it has accepted its request for data from a different domain (origin).
//    client.println("Content-Type: application/json;charset=utf-8");  //Lets the browser know that the data will be in a JSON format
//    client.println("Server: Arduino");           // The data is coming from an Arduino Web Server (this line can be omitted)
//    client.println("Connection: close");         // Will close the connection at the end of data transmission.
//    client.println();                            // You need to include this blank line - it tells the browser that it has reached the end of the Server reponse header.
//                               // This is tha starting bracket of the JSON data
//    client.print("{\"temperature\": \"");
//    client.print(t);                           
//    client.print("\", \"Humidity\": \"");
//    client.print(h);               
//    client.print("\"}");                      
//                      
//  }
//  else {
//    client.flush();
//    client.print("HTTP/1.1 200 OK");           // This tells the browser that the request to provide data was accepted
//    client.print("Access-Control-Allow-Origin: *");  //Tells the browser it has accepted its request for data from a different domain (origin).
//    client.print("Content-Type: application/json;charset=utf-8");  //Lets the browser know that the data will be in a JSON format
//    client.print("Server: Arduino");           // The data is coming from an Arduino Web Server (this line can be omitted)
//    client.print("Connection: close");         // Will close the connection at the end of data transmission.
//    //client.print();                            // You need to include this blank line - it tells the browser that it has reached the end of the Server reponse header.
//                          // This is tha starting bracket of the JSON data
//    client.print("{\"Response\": ");
//    client.print("Invalid");                          
//    client.print("}");                     
//                       // This is the final bracket of the JSON data
//  }
//
//    delay(1);
//    Serial.println("Client disonnected");
//
//}

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
float ntu;

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

const char* ssid = "Rahul2122";  // Enter SSID here
const char* password = "gocorona321";  //Enter Password here

float Temperature;
float Humidity;
float sensorValue;

ESP8266WebServer server(80);

#define dht_dpin D1
DHT dht(dht_dpin, DHTTYPE); 
void setup(void)
{ 
  dht.begin();            
  s.begin(9600);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  Serial.begin(9600);
  Serial.println("Humidity and temperature \n\n");
  Serial.println("==========================================");
  delay(700);

}
void loop() {
      server.handleClient();
//    float h = dht.readHumidity();
//    float t = dht.readTemperature();         
//    Serial.print("Current humidity = ");
//    Serial.print(h);
//    Serial.print("%  ");
//    Serial.print("temperature = ");
//    Serial.print(t); 
//    Serial.println("C  ");
  delay(1000);
}
void handle_OnConnect() {
  s.write("s");
  if (s.available()>0)
  {
    ntu=s.read();
    Serial.println(ntu);
  }
  delay(1000);
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  sensorValue = analogRead(0);       // read analog input pin 0
  Serial.print("AirQua=");
  Serial.print(sensorValue, DEC);               // prints the value read
  Serial.println(" PPM");
  server.send(200, "text/html", SendHTML(Temperature,Humidity,sensorValue,ntu));
  
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat, float sensorValuestat, float ntustat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>blah</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +=" C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";

  ptr +="<p>Air Quality: ";
  ptr +=(int)sensorValuestat;
  ptr +=" PPM</p>";

  ptr +="<p>Water Quality: ";
  ptr +=(int)ntustat;
  ptr +=" NTU</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//#include "DHT.h"
//
//// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
////#define DHTTYPE DHT21   // DHT 21 (AM2301)
////#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//
///*Put your SSID & Password*/
//const char* ssid = "Rahul2122";  // Enter SSID here
//const char* password = "gocorona321";  //Enter Password here
//
//ESP8266WebServer server(80);
//
//// DHT Sensor
//uint8_t DHTPin = D1; 
//               
//// Initialize DHT sensor.
//DHT dht(DHTPin, DHTTYPE);                
//
//float Temperature;
//float Humidity;
// 
//void setup() {
//  Serial.begin(115200);
//  delay(100);
//  
//  pinMode(DHTPin, INPUT);
//
//  dht.begin();              
//
//  Serial.println("Connecting to ");
//  Serial.println(ssid);
//
//  //connect to your local wi-fi network
//  WiFi.begin(ssid, password);
//
//  //check wi-fi is connected to wi-fi network
//  while (WiFi.status() != WL_CONNECTED) {
//  delay(1000);
//  Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected..!");
//  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
//
//  server.on("/", handle_OnConnect);
//  server.onNotFound(handle_NotFound);
//
//  server.begin();
//  Serial.println("HTTP server started");
//
//}
//void loop() {
//  
//  server.handleClient();
//  
//}
//
//void handle_OnConnect() {
//
// Temperature = dht.readTemperature(); // Gets the values of the temperature
//  Humidity = dht.readHumidity(); // Gets the values of the humidity 
//  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
//}
//
//void handle_NotFound(){
//  server.send(404, "text/plain", "Not found");
//}
//
//String SendHTML(float Temperaturestat,float Humiditystat){
//  String ptr = "<!DOCTYPE html> <html>\n";
//  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
//  ptr +="<title>ESP8266 Weather Report</title>\n";
//  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
//  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
//  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
//  ptr +="</style>\n";
//  ptr +="</head>\n";
//  ptr +="<body>\n";
//  ptr +="<div id=\"webpage\">\n";
//  ptr +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";
//  
//  ptr +="<p>Temperature: ";
//  ptr +=(int)Temperaturestat;
//  ptr +="°C</p>";
//  ptr +="<p>Humidity: ";
//  ptr +=(int)Humiditystat;
//  ptr +="%</p>";
//  
//  ptr +="</div>\n";
//  ptr +="</body>\n";
//  ptr +="</html>\n";
//  return ptr;
//}
