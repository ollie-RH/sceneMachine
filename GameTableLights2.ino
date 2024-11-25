#include <WiFiS3.h>           // Use the WiFi library specific to Arduino Uno R4 WiFi
#include <Adafruit_NeoPixel.h> // Library to control the LED strip

// WiFi credentials
const char* ssid = "Studio 5";
const char* password = "jubensha";

// Set up web server on port 80
WiFiServer server(80);

// LED Strip configuration
#define LED_PIN 6            // Data pin for the LED strip
#define NUM_LEDS 30          // Number of LEDs in the strip

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Initialize LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    client.flush();
    
    // Handle LED ON request
    if (request.indexOf("/ON") != -1) {
      Serial.println("Turning LED strip ON");
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // Set color to red
      }
      strip.show();
    }

    // Handle LED OFF request
    if (request.indexOf("/OFF") != -1) {
      Serial.println("Turning LED strip OFF");
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off LEDs
      }
      strip.show();
    }

    // Send a response back to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); // End of HTTP headers
    client.println("<html><body>");
    client.println("<h1>LED Control hello world</h1>");
    client.println("<a href=\"/ON\">Turn ON</a><br>");
    client.println("<a href=\"/OFF\">Turn OFF</a>");
    client.println("</body></html>");
    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
