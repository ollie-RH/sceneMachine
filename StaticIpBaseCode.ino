#include <WiFiS3.h>

// Replace with your network credentials
char ssid[] = "YOUR NETWORK";
char password[] = "NETWORK PASSWORD";

IPAddress local_IP(192, 168, 1, 55);  //Change static IP here
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Optional

void setup() {
  // Start the Serial communication to send messages to the computer
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Static IP Setup
  WiFi.config(local_IP, gateway, subnet, primaryDNS);

  while (WiFi.status() != WL_CONNECTED) {
    // Attempt to connect to WiFi network
    WiFi.begin(ssid, password);

    // Wait for connection
    int maxAttempts = 20; // Maximum number of attempts before retrying
    int attempt = 0;

    while (WiFi.status() != WL_CONNECTED && attempt < maxAttempts) {
      delay(500);
      Serial.print(".");
      attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      // If connection successful
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      break; // Exit the outer loop
    } else {
      // If connection failed
      Serial.println("");
      Serial.println("WiFi connection failed. Retrying...");
      Serial.print("Current WiFi Status: ");
      Serial.println(wifiStatusToString(WiFi.status()));
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

void loop() {
  // Put your main code here, to run repeatedly
}

const char* wifiStatusToString(uint8_t status) {
  switch (status) {
    case WL_NO_SHIELD:
      return "NO SHIELD";
    case WL_IDLE_STATUS:
      return "IDLE";
    case WL_NO_SSID_AVAIL:
      return "NO SSID AVAILABLE";
    case WL_SCAN_COMPLETED:
      return "SCAN COMPLETED";
    case WL_CONNECTED:
      return "CONNECTED";
    case WL_CONNECT_FAILED:
      return "CONNECT FAILED";
    case WL_CONNECTION_LOST:
      return "CONNECTION LOST";
    case WL_DISCONNECTED:
      return "DISCONNECTED";
    default:
      return "UNKNOWN STATUS";
  }
}