#include <WiFiS3.h>
#include <Adafruit_NeoPixel.h>

char ssid[] = "YOUR NETWORK";
char password[] = "NETWORK PASSWORD";

// LED strip configuration
#define LED_PIN 7
#define LED_COUNT 216
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

IPAddress local_IP(192, 168, 12, 101);  //Change static IP here
IPAddress gateway(192, 168, 12, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Optional

WiFiServer server(80);

// Global variables to store the current color and state
int currentR = 0, currentG = 0, currentB = 0;
bool isCloudy = false, isStormy = false, isRainy = false, isFire = false, isTavern = false, isForest = false, isJungle = false, isOcean = false;
bool isBoss = false, isLab = false, isToxic = false, isRainbow = false;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
bool secondFlash = false;
bool justFlashed = false;

// Variables for managing non-blocking delays
unsigned long lastUpdate = 0; // Last update time for animations
const unsigned long updateInterval = 50; // Interval for updating animations

void setup() {
  // Start the Serial communication to send messages to the computer
  Serial.begin(115200);
  delay(10);

  // Initialize the LED strip
  strip.begin();
  strip.show();

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

  // Start the server
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    // Wait until the client sends some data
    Serial.println("New Client.");
    while (!client.available()) {
      delay(1);
    }

    // Read the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request
    if (request.indexOf("/effect10") != -1) {
      resetEffects();
      isForest = true;
    } else if (request.indexOf("/effect11") != -1) {
      resetEffects();
      isJungle = true;
    } else if (request.indexOf("/effect12") != -1) {
      resetEffects();
      isOcean = true;
    } else if (request.indexOf("/effect13") != -1) {
      resetEffects();
      isBoss = true;
    } else if (request.indexOf("/effect14") != -1) {
      resetEffects();
      isLab = true;
    } else if (request.indexOf("/effect15") != -1) {
      resetEffects();
      isToxic = true;
    } else if (request.indexOf("/effect16") != -1) {
      resetEffects();
      isRainbow = true;
    } else if (request.indexOf("/effect1") != -1) {
      resetEffects();
      effect1(); 
    } else if (request.indexOf("/effect2") != -1) {
      resetEffects();
      effect2();
    } else if (request.indexOf("/effect3") != -1) {
      resetEffects();
      effect3();
    } else if (request.indexOf("/effect4") != -1) {
      resetEffects();
      effect4();
    } else if (request.indexOf("/effect5") != -1) {
      resetEffects();
      isCloudy = true;
    } else if (request.indexOf("/effect6") != -1) {
      resetEffects();
      isStormy = true;
    } else if (request.indexOf("/effect7") != -1) {
      resetEffects();
      isRainy = true;
    } else if (request.indexOf("/effect8") != -1) {
      resetEffects();
      isFire = true;
    } else if (request.indexOf("/effect9") != -1) {
      resetEffects();
      isTavern = true;
    } else if (request.indexOf("/setcolor") != -1) {
      resetEffects();
      handleSetColor(request);
    }

    // Send the response to the client
    sendHTMLResponse(client);

    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= updateInterval) {
    lastUpdate = currentMillis;

    if (isCloudy) {
      cloudyEffect();
    }

    if (isStormy) {
      stormyEffect();
    }

    if (isRainy) {
      rainyEffect();
    }

    if (isFire) {
      fireEffect();
    }

    if (isTavern) {
      tavernEffect();
    }

    if (isForest) {
      forestEffect();
    }

    if (isJungle) {
      jungleEffect();
    }

    if (isOcean) {
      oceanEffect();
    }

    if (isBoss) {
      bossEffect();
    }

    if (isLab) {
      labEffect();
    }

    if (isToxic) {
      toxicEffect();
    }

    if (isRainbow) {
      rainbowEffect();
    }
  }
}

void sendHTMLResponse(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  // Return the HTML page
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("@import url('https://fonts.googleapis.com/css2?family=Uncial+Antiqua&display=swap');");
  client.println("body { font-family: 'Uncial Antiqua', cursive; text-align: center; background-color: black; color: white; position: relative; }");
  client.println("h1 { color: #fff; }");
  client.println(".button { display: inline-block; margin: 10px; padding: 20px; font-size: 20px; background-color: #800000; color: white; border: none; border-radius: 5px; text-decoration: none; }");
  client.println(".button:hover { background-color: #b30000; }");
  client.println(".grid-container { display: grid; grid-template-columns: repeat(4, 1fr); gap: 10px; justify-items: center; z-index: 1; position: relative; }");
  client.println(".input-container { margin-top: 20px; z-index: 1; position: relative; }");
  client.println("input[type='color'] { padding: 10px; margin: 5px; font-size: 16px; }");
  client.println("button { padding: 15px 30px; font-size: 20px; background-color: #800000; color: white; border: none; border-radius: 5px; cursor: pointer; margin-left: 15px; }");
  client.println("button:hover { background-color: #b30000; }");
  client.println("#colorIndicator { display: inline-block; width: 40px; height: 40px; background-color: #000; border: 1px solid #fff; vertical-align: middle; margin-left: 10px; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>LED Control</h1>");
  client.println("<div class='grid-container'>");
  client.println("<a class='button' href=\"/effect1\">Sunrise</a>");
  client.println("<a class='button' href=\"/effect2\">Day</a>");
  client.println("<a class='button' href=\"/effect3\">Dusk</a>");
  client.println("<a class='button' href=\"/effect4\">Night</a>");
  client.println("<a class='button' href=\"/effect5\">Cloudy</a>");
  client.println("<a class='button' href=\"/effect6\">Stormy</a>");
  client.println("<a class='button' href=\"/effect7\">Rainy</a>");
  client.println("<a class='button' href=\"/effect8\">Fire</a>");
  client.println("<a class='button' href=\"/effect9\">Tavern</a>");
  client.println("<a class='button' href=\"/effect10\">Forest</a>");
  client.println("<a class='button' href=\"/effect11\">Jungle</a>");
  client.println("<a class='button' href=\"/effect12\">Ocean</a>");
  client.println("<a class='button' href=\"/effect13\">Boss</a>");
  client.println("<a class='button' href=\"/effect14\">Lab</a>");
  client.println("<a class='button' href=\"/effect15\">Toxic</a>");
  client.println("<a class='button' href=\"/effect16\">Rainbow</a>");
  client.println("</div>");
  client.println("<div class='input-container'>");
  client.println("<p>Set Color: <input type='color' id='colorPicker' onchange='updateColorIndicator()'><div id='colorIndicator'></div><button onclick='setColor()'>Set</button></p>");
  client.println("</div>");
  client.println("<script>");
  client.println("function setColor() {");
  client.println("  var color = document.getElementById('colorPicker').value;");
  client.println("  var rgb = hexToRgb(color);");
  client.println("  rgb = limitRgb(rgb);");
  client.println("  fetch('/setcolor?color=' + rgb.g + ',' + rgb.r + ',' + rgb.b);");
  client.println("}");
  client.println("function updateColorIndicator() {");
  client.println("  var color = document.getElementById('colorPicker').value;");
  client.println("  document.getElementById('colorIndicator').style.backgroundColor = color;");
  client.println("}");
  client.println("function hexToRgb(hex) {");
  client.println("  var bigint = parseInt(hex.substring(1), 16);");
  client.println("  var r = (bigint >> 16) & 255;");
  client.println("  var g = (bigint >> 8) & 255;");
  client.println("  var b = (bigint & 255);");
  client.println("  return {r:r, g:g, b:b};");
  client.println("}");
  client.println("function limitRgb(rgb) {");
  client.println("  rgb.r = Math.min(rgb.r, 180);");
  client.println("  rgb.g = Math.min(rgb.g, 180);");
  client.println("  rgb.b = Math.min(rgb.b, 180);");
  client.println("  return rgb;");
  client.println("}");
  client.println("</script>");
  client.println("</body>");
  client.println("</html>");
}

void resetEffects() {
  isCloudy = false;
  isStormy = false;
  isRainy = false;
  isFire = false;
  isTavern = false;
  isForest = false;
  isJungle = false;
  isOcean = false;
  isBoss = false;
  isLab = false;
  isToxic = false;
  isRainbow = false;
  previousMillis = millis();  // Reset timing
  secondFlash = false;
  justFlashed = false;
}

void effect1() {
  // Sunrise effect
  int startG = 0, startR = 0, startB = 139; // Dark blue
  int endG = 60, endR = 180, endB = 0; // Bright orange
  fadeToColor(currentG, currentR, currentB, startG, startR, startB, 2000);
  fadeToColor(startG, startR, startB, endG, endR, endB, 2000);
  currentG = endG;
  currentR = endR;
  currentB = endB;
}

void effect2() {
  // Day time effect
  int endG = 180, endR = 180, endB = 180; // Bright daylight color
  fadeToColor(currentG, currentR, currentB, endG, endR, endB, 2000);
  currentG = endG;
  currentR = endR;
  currentB = endB;
}

void effect3() {
  // Dusk effect
  int endG = 100, endR = 180, endB = 170; // Pastel purple
  fadeToColor(currentG, currentR, currentB, endG, endR, endB, 2000);
  currentG = endG;
  currentR = endR;
  currentB = endB;
}

void effect4() {
  // Night effect
  int endG = 0, endR = 0, endB = 139; // Dark blue
  fadeToColor(currentG, currentR, currentB, endG, endR, endB, 2000);
  currentG = endG;
  currentR = endR;
  currentB = endB;
}

void cloudyEffect() {
  static bool isDim = false;
  static unsigned long dimDuration = 0;
  static unsigned long brightDuration = 0;
  int num = 0;
  if (isDim) {
    Serial.println("Brightening up");
    fadeToColor(currentG / 10, currentR / 10, currentB / 10, currentG, currentR, currentB, 2000);
    isDim = false;
    num = random(0, 30000);
    brightDuration = millis() + num; // Bright for 10-30 seconds
    delay(num);
  } else {
    Serial.println("Dimming down");
    fadeToColor(currentG, currentR, currentB, currentG / 10, currentR / 10, currentB / 10, 2000);
    isDim = true;
    num = random(5000, 20000);
    dimDuration = millis() + num; // Dim for 5-20 seconds
    delay(num);
  }
  interval = isDim ? dimDuration - millis() : brightDuration - millis();
}

void stormyEffect() {
  static int stormColors[10][3] = {
    {16, 16, 80}, {16, 16, 80}, {16, 16, 80}, {16, 16, 80},
    {16, 16, 80}, {16, 16, 80}, {16, 16, 80}, {16, 16, 80},
    {16, 16, 80}, {200, 200, 200}  // Lightning color
  };

  fadeToColor(currentG, currentR, currentB, 32, 32, 96, 100);
  static int prevColorIndex = 0;
  int colorIndex = random(0, 10);

  // Ensure we don't pick the same color twice in a row
  while (colorIndex == prevColorIndex) {
    colorIndex = random(0, 10);
  }

  int newG = stormColors[colorIndex][0];
  int newR = stormColors[colorIndex][1];
  int newB = stormColors[colorIndex][2];

  if(colorIndex == 9 && random(0,4) == 3){
    fadeToColor(newG, newR, newB, 32, 32, 96, 50);
  }
  fadeToColor(newG, newR, newB, 32, 32, 96, 100);
  
  prevColorIndex = colorIndex;

  interval = 100; // Quick flicker interval
  currentG = 32;
  currentR = 32;
  currentB = 96;
}

void rainyEffect() {
  static int rainColors[20][3] = {
    {32, 32, 96}, {32, 32, 96}, {32, 32, 96}, {32, 32, 96},
    {32, 32, 96}, {32, 32, 96}, {32, 32, 96}, {32, 32, 96},
    {32, 32, 96}, {32, 32, 96}, {32, 32, 96}, {32, 32, 96},
    {32, 32, 96}, {32, 32, 96}, {32, 32, 96}, {32, 32, 96},
    {32, 32, 96}, {32, 32, 96}, {32, 32, 96}, {200, 200, 200}  // Lightning color
  };
  fadeToColor(currentG, currentR, currentB, 64, 64, 128, 100);
  static int prevColorIndex = 0;
  int colorIndex = random(0, 20);

  // Ensure we don't pick the same color twice in a row
  while (colorIndex == prevColorIndex) {
    colorIndex = random(0, 20);
  }

  int newG = rainColors[colorIndex][0];
  int newR = rainColors[colorIndex][1];
  int newB = rainColors[colorIndex][2];

  fadeToColor(newG, newR, newB, 64, 64, 128, 100);
  prevColorIndex = colorIndex;

  interval = 100; // Quick flicker interval
  currentG = 64;
  currentR = 64;
  currentB = 128;
}

void fireEffect() {
  static int fireColors[6][3] = {{30, 180, 0}, {50, 180, 0}, {60, 180, 0}, {70, 180, 0}, {80, 180, 0}, {10,35,0}}; // Red-orange shades
  static int prevColorIndex = 0;
  int colorIndex = random(0, 6);

  // Ensure we don't pick the same color twice in a row
  while (colorIndex == prevColorIndex) {
    colorIndex = random(0, 3);
  }

  int newG = fireColors[colorIndex][0];
  int newR = fireColors[colorIndex][1];
  int newB = fireColors[colorIndex][2];

  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 100);
  prevColorIndex = colorIndex;

  interval = 80; // Quick flicker interval
  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void tavernEffect() {
  static int tavernColors[5][3] = {{100,255,20}, {50,255,10}, {10,255,10}, {100,225,60}, {120,255,40}}; // Yellow-Purple shades
  static int prevColorIndex = 0;
  int colorIndex = random(0, 5);

  // Ensure we don't pick the same color twice in a row
  while (colorIndex == prevColorIndex) {
    colorIndex = random(0, 5);
  }

  int newG = tavernColors[colorIndex][0];
  int newR = tavernColors[colorIndex][1];
  int newB = tavernColors[colorIndex][2];

  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 1000);
  prevColorIndex = colorIndex;

  interval = 100; // Quick flicker interval
  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void forestEffect() {
  static int baseG = 255, baseR = 50, baseB = 50; // Forest green
  static int pulseInterval = 3000; // 3 seconds for a full pulse cycle
  unsigned long currentMillis = millis();
  float brightness = random(25,100);
  int newG = baseG * brightness/100;
  int newR = baseR * brightness/100;
  int newB = baseB * brightness/100;

  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 500);

  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void jungleEffect() {
  static int baseG = 255, baseR = 64, baseB = 110; // Jungle green
  static int pulseInterval = 3000; // 3 seconds for a full pulse cycle
  unsigned long currentMillis = millis();
  float brightness = random(25,100);
  int newG = baseG * brightness/100;
  int newR = baseR * brightness/100;
  int newB = baseB * brightness/100;

  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 500);

  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void oceanEffect() {
  static int baseG = 255, baseR = 30, baseB = 255; // Ocean blue
  static int pulseInterval = 3000; // 3 seconds for a full pulse cycle
  unsigned long currentMillis = millis();
  float brightness = random(25,100);
  int newG = baseG * brightness/100;
  int newR = baseR * brightness/100;
  int newB = baseB * brightness/100;

  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 500);
  
  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void bossEffect() {
  // Boss effect: intense red
  int newG = 0, newR = 255, newB = 0;
  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 100);
  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void labEffect() {
  // Lab effect: flickering white
  int newG = 180, newR = 180, newB = 180;
  currentG = newG;
  currentR = newR;
  currentB = newB;
  bool flicker = random(0, 1);
  if(flicker == 1){
    int newG = 20, newR = 20, newB = 20;
  }

  fadeToColor(newG, newR, newB, currentG, currentR, currentB, 50);
}

void toxicEffect() {
  // Toxic effect: pulsing lime green/yellow
  static int baseG = 180, baseR = 180, baseB = 0; // Toxic lime green/yellow
  float brightness = random(25,100);
  int newG = baseG * brightness/100;
  int newR = baseR * brightness/100;
  int newB = baseB * brightness/100;

  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 500);

  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void rainbowEffect() {
  // Rainbow effect: chase through all colors
  static int currentPixel = 0;
  int rainbowColors[7][3] = {
    {0, 180, 0}, {127, 180, 0}, {180, 180, 0}, {180, 0, 0}, {0, 0, 180}, {0, 75, 130}, {0, 139, 180}
  };


  for (int i = 0; i < strip.numPixels(); i++) {
    int colorIndex = (i + currentPixel) % 7;

    fadeToColor(currentG, currentR, currentB, rainbowColors[colorIndex][0], rainbowColors[colorIndex][1], rainbowColors[colorIndex][2], 500);
      currentG = rainbowColors[colorIndex][0];
      currentR = rainbowColors[colorIndex][1];
      currentB = rainbowColors[colorIndex][2];
  }
  strip.show();
  currentPixel = (currentPixel + 1) % strip.numPixels();
}

void handleSetColor(String request) {
  int colorStart = request.indexOf("color=") + 6;
  int colorEnd = request.indexOf(" ", colorStart);
  if (colorEnd == -1) {
    colorEnd = request.length();
  }
  String colorStr = request.substring(colorStart, colorEnd);
  Serial.println("Color: " + colorStr);
  
  int comma1 = colorStr.indexOf(',');
  int comma2 = colorStr.indexOf(',', comma1 + 1);
  
  int newG = colorStr.substring(0, comma1).toInt();
  int newR = colorStr.substring(comma1 + 1, comma2).toInt();
  int newB = colorStr.substring(comma2 + 1).toInt();

  newG = constrain(newG, 0, 180);
  newR = constrain(newR, 0, 180);
  newB = constrain(newB, 0, 180);
  
  fadeToColor(currentG, currentR, currentB, newG, newR, newB, 1000); // 1000 ms fade duration
  
  currentG = newG;
  currentR = newR;
  currentB = newB;
}

void fadeToColor(int g1, int r1, int b1, int g2, int r2, int b2, int duration) {
  int steps = 50; // Number of steps in the fade
  int delayTime = duration / steps; // Delay between each step

  for (int i = 0; i <= steps; i++) {
    int g = g1 + ((g2 - g1) * i / steps);
    int r = r1 + ((r2 - r1) * i / steps);
    int b = b1 + ((b2 - b1) * i / steps);
    setColor(g, r, b);
    delay(delayTime);
  }
}

void setColor(int g, int r, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(g, r, b));
  }
  strip.show();
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