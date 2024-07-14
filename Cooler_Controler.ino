#include <ESP8266WiFi.h>

const char* ssid = "galaxy";
const char* password = "        ";
// int lastFloatSwitchState = HIGH;

#define WIFI_INDICATOR 16 // D0
#define MOTOR_PIN 5 // D1 
#define PUMP_PIN 14 // D5 
// #define WATER_CHECK_PIN 12 // D6
#define SWING_PIN 12 // D6

int motor_value = HIGH;
int swing_value = HIGH;
int pump_value = HIGH;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(SWING_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(WIFI_INDICATOR, OUTPUT);

  digitalWrite(SWING_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);
  digitalWrite(MOTOR_PIN, HIGH);
  digitalWrite(WIFI_INDICATOR, LOW);

  // pinMode(WATER_CHECK_PIN, INPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    digitalWrite(WIFI_INDICATOR, LOW);
    delay(250);
    digitalWrite(WIFI_INDICATOR, HIGH);
    Serial.print(".");
  }

  digitalWrite(WIFI_INDICATOR, LOW);
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // int floatSwitchState = digitalRead(WATER_CHECK_PIN);
  // if (lastFloatSwitchState != floatSwitchState) {
  //   if (floatSwitchState == HIGH) {
  //     Serial.println("Water is present");
  //     // digitalWrite(PUMP_PIN, LOW);
  //   } else {
  //     Serial.println("Water is not present");
  //     // digitalWrite(PUMP_PIN, high);
  //   }
  //   lastFloatSwitchState = floatSwitchState;  // Update state here
  // }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) return;

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  // For Motor
  if (request.indexOf("/MOTOR_PIN=OFF") != -1) {
    Serial.println("MOTOR_PIN=OFF");
    digitalWrite(MOTOR_PIN, LOW);
    motor_value = LOW;
  }
  if (request.indexOf("/MOTOR_PIN=ON") != -1) {
    Serial.println("MOTOR_PIN=ON");
    digitalWrite(MOTOR_PIN, HIGH);
    motor_value = HIGH;
  }
  
  // For Pump
  if (request.indexOf("/PUMP_PIN=OFF") != -1) {
    Serial.println("PUMP_PIN=OFF");
    digitalWrite(PUMP_PIN, LOW);
    pump_value = LOW;
  }
  if (request.indexOf("/PUMP_PIN=ON") != -1) {
    Serial.println("PUMP_PIN=ON");
    digitalWrite(PUMP_PIN, HIGH);
    pump_value = HIGH;
  }

  // For Swings
  if (request.indexOf("/SWING_PIN=ON") != -1) {
    Serial.println("SWING_PIN=ON");
    digitalWrite(SWING_PIN, LOW);
    swing_value = LOW;
  }
  if (request.indexOf("/SWING_PIN=OFF") != -1) {
    Serial.println("SWING_PIN=OFF");
    digitalWrite(SWING_PIN, HIGH);
    swing_value = HIGH;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");  //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>COOLER REMOTE</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("<style>");
  client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; display: flex; justify-content: center; align-items: center; align-content: center; height: 100vh; flex-direction: column; }");
  client.println(".container { margin: 20px auto; padding: 20px; max-width: 350px; background: #fff; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }");
  client.println("h2 { color: #333; }");
  client.println(".button { display: inline-block; padding: 15px 25px; font-size: 14px; margin: 10px; cursor: pointer; color: #fff; background-color: #007BFF; border: none; border-radius: 5px; text-decoration: none; }");
  client.println(".button:active { background-color: #0056b3; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");

  // For Motor
  client.println("<div class=\"container\">");
  client.print("<h2>MOTOR : ");
  if (motor_value == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }
  client.print("</h2>");
  client.println("<a href=\"/MOTOR_PIN=OFF\" class=\"button\">Turn OFF</a>");
  client.println("<a href=\"/MOTOR_PIN=ON\" class=\"button\">Turn ON</a>");
  client.println("</div>");

  // For Pump
  client.println("<div class=\"container\">");
  client.print("<h2>PUMP : ");
  if (pump_value == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }
  client.print("</h2>");
  client.println("<a href=\"/PUMP_PIN=OFF\" class=\"button\">Turn OFF</a>");
  client.println("<a href=\"/PUMP_PIN=ON\" class=\"button\">Turn ON</a>");
  client.println("</div>");

  // For Swings
  client.println("<div class=\"container\">");
  client.print("<h2>SWINGS : ");
  if (swing_value == HIGH) {
    client.print("OFF");
  } else {
    client.print("ON");
  }
  client.print("</h2>");
  client.println("<a href=\"/SWING_PIN=OFF\" class=\"button\">Turn OFF</a>");
  client.println("<a href=\"/SWING_PIN=ON\" class=\"button\">Turn ON</a>");
  client.println("</div>");

  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
