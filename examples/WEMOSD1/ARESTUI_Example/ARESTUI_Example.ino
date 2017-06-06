//#define DEBUG_MODE 1
//#define LIGHTWEIGHT 1

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>


// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
const char* ssid = "ssid";
const char* password = "password";

int pos = 90;
Servo servo;

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

void setup(void) {
  // Start Serial
  Serial.begin(115200);
  servo.attach(D6);
  // Set the title
  rest.title("aREST UI Demo");

  rest.button(5,"LED");
  rest.slider(5);
  rest.function("servoPlus",servoPlus);
  rest.function("servoMinus",servoMinus);
  rest.variable("Servo",&pos);
  rest.label("Servo");
  rest.callFunction("servoPlus","push");
  rest.callFunction("servoMinus","push");

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);

}


int servoPlus(String command) {
  pos+=10;
  servo.write(pos);
  return 1;
}
int servoMinus(String command) {
  pos-=10;
  servo.write(pos);
  return 1;
}
