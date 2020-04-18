#include <ESP8266WiFi.h>

//wifi ağ ismi
const char* ssid     = "WIFI_SSID";
//wifi şifresi
const char* password = "WIFI_SIFRESI";

//nodemcu default port
WiFiServer server(80);

//HTTP isteği tutar
String header;

//analog sıcaklık sensörü pini
int tempSensorPin = A0;

void printIpAndStart()
{
  //Lokal IP adresi yazdır ve serverı başlat
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void setup() 
{
  Serial.begin(9600);

  //WiFi ağına SSID ve şifreyi kullanarak bağlan
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  printIpAndStart();
}

String calculateTemp()
{
  //sıcaklık sensörünün analog değerini oku
  int analogValue = analogRead(tempSensorPin);
  //nodemcu tarafından sağlanan 3.3V gerilime göre analog değere göre selsiyus elde etme
  float celsius = (analogValue/ 1024.0) * 3300 / 10; 
  return String(celsius);
}

void printPage(WiFiClient client)
{
  //HTTP header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  //HTML Web sayfası
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println("</style></head>");
  client.println("<body><h1>Dijital Termostat</h1>");
  client.println("<h1><i class= \"fa fa-thermometer\"></i>" + calculateTemp() + " &#8451; </h1>");
  client.println("</body></html>");
  client.println();
}

void loop(){
  //Gelen istekleri dinlemek için
  WiFiClient client = server.available();   

  //Eğer yeni bir istemci olursa
  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";
    //İstemci bağlı olduğu sürece
    while (client.connected()) { 
      if (client.available()) {                          
            printPage(client);
            break;
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
