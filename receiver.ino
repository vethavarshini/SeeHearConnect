// receiver


//------------------------------------------------------------BLYNK
// #define BLYNK_TEMPLATE_ID "TMPL3HVkW304s"
// #define BLYNK_TEMPLATE_NAME "smart door"
// #define BLYNK_AUTH_TOKEN "lVjM0QuH5YpJS7kYHSbkbcbBGfbhgQzt"
#define BLYNK_TEMPLATE_ID "TMPL3s99Dqo1P"
#define BLYNK_TEMPLATE_NAME "MyfirstProject"
#define BLYNK_AUTH_TOKEN "8UMpZHPVl5NXSZmJgorkUzHOCMIzTs4M"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
// send using url fetch
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
// BlynkTimer timer;

//------------------------------------------------------------Wifi 
char swe_token[] = BLYNK_AUTH_TOKEN;
char swe_wifi_id[] = "Galaxy S20 FE 5G1B3A";  // wifi name
char swe_wifi_key[] = "stmhwifi";  // wifi password

//------------------------------------------------------------Sensor
#define BLYNK_AUTH_TOKEN_SEND "lVjM0QuH5YpJS7kYHSbkbcbBGfbhgQzt"
#define blynkSend     V0    // to sender
#define blynkSendStr "V0"    // to sender
#define blynkReceive  V1    // from sender
#define inputPin      36    // D36
#define outputPin     12    // D12


bool last_state = false;
unsigned long previousMillis = 0;
const long interval = 10000;

//------------------------------------------------------------Setup
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);
  Serial.println('\n');
  //////////// BLYNk
  Serial.println("Completing to BLYNK...");
  Blynk.begin(swe_token, swe_wifi_id, swe_wifi_key);
  Serial.println("\nConnected to BLYNK!!!");
  Serial.println("Setup Completed..............>>>>\n\n");
  WiFi.begin(swe_wifi_id, swe_wifi_key);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop()
{
  Blynk.run();
  int curr_ip = analogRead(inputPin);
  // Serial.print("curr_ip");
  // Serial.println(curr_ip);
  if (curr_ip >= 555 && !last_state) {
    // Blynk.virtualWrite(blynkSend, HIGH);
    sendToDevice(1);
    Serial.print("Input High =");
    Serial.print(curr_ip);
    Serial.println(" -> sending... 1");
    previousMillis = millis();
    last_state = true;
  } else if (curr_ip < 555 && last_state) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // Blynk.virtualWrite(blynkSend, LOW);
      sendToDevice(0);
      Serial.print("Input Low =");
      Serial.print(curr_ip);
      Serial.println(" -> sending... 0");
      // delay(2000);
      last_state = false;
    }
  }
}

void sendToDevice(int curr_ip) {
  HTTPClient http;
  String urlFetch = "https://script.google.com/macros/s/AKfycbzU28CQn3BRNqW2qexIVg-dOWPgaLAqYxXzCaI-VINpC-xwyUqL-Yw4I0sXn54Z0Pu9ng/exec";
  http.begin(urlFetch);
  http.addHeader("Content-Type", "application/json");
  String jsonBody = "{\"command\": \"updateBlynk\", \"blynkToken\": \"" + String(BLYNK_AUTH_TOKEN_SEND) + "\",\"blynkRequest\":\"" + String(blynkSendStr) + "=" + String(curr_ip) + "\"}";
  int httpCode = http.POST(jsonBody);
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("HTTP Fetch result:");
    Serial.println(payload);
  }
  else {
    Serial.println("Error on HTTP request");
  }
  http.end();
}

BLYNK_WRITE(blynkReceive)
{
  int curr_op = param.asInt();
  Serial.print("curr op --> ");
  Serial.println(curr_op);
  if(curr_op == 1) {
    digitalWrite(outputPin, HIGH);
    Serial.println("Output Pin HIGH");
  } else {
    digitalWrite(outputPin, LOW);
    Serial.println("Output Pin LOW");
  }
}
