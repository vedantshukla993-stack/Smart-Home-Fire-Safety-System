#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP32Servo.h>

//===================== WiFi =====================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

//================= ThingSpeak ===================
// Replace with your Write API Key
String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

//================ Pin Definitions ===============
#define MQ2_PIN      34
#define DHT_PIN      4
#define SERVO_PIN    18

#define RED_LED      14
#define GREEN_LED    12
#define BLUE_LED     13

#define DHTTYPE DHT22

//================ Objects =======================
DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo ventServo;

//================ Variables =====================
int smokeValue = 0;

float temperature = 0;
float humidity = 0;

bool fireDetected = false;
bool warningState = false;

unsigned long previousMillis = 0;
unsigned long uploadTimer = 0;

bool blinkState = false;

//============== Function Prototypes =============
void safeMode();
void warningMode();
void fireMode();
void uploadThingSpeak(String status);

//==================== Setup =====================
void setup() {

  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  dht.begin();

  Wire.begin(21,22);

  lcd.init();
  lcd.backlight();

  ventServo.attach(SERVO_PIN);
  ventServo.write(0);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SMART HOME");

  lcd.setCursor(0,1);
  lcd.print("Initializing");

  delay(2000);

  WiFi.begin(ssid,password);

  lcd.clear();
  lcd.print("Connecting WiFi");

  while(WiFi.status()!=WL_CONNECTED){

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  lcd.clear();
  lcd.print("WiFi Connected");

  delay(1500);

  lcd.clear();
}
//================ SAFE MODE =====================
void safeMode() {

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);

  ventServo.write(0);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SYSTEM SECURE");

  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temperature,1);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.print("S:");
  lcd.print(smokeValue);
}

//================ WARNING MODE ==================
void warningMode() {

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);

  ventServo.write(0);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("HIGH TEMP!");

  lcd.setCursor(0,1);
  lcd.print("CHECK AREA");
}

//================ FIRE MODE =====================
void fireMode() {

  blinkState = !blinkState;

  digitalWrite(RED_LED, blinkState);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  ventServo.write(90);

  lcd.clear();

  if(blinkState){

    lcd.setCursor(0,0);
    lcd.print(" FIRE ALARM ");

    lcd.setCursor(0,1);
    lcd.print("EVACUATE NOW!");
  }
  else{

    lcd.setCursor(0,0);
    lcd.print("SMOKE DETECTED");

    lcd.setCursor(0,1);
    lcd.print("WINDOW OPEN");
  }

  delay(300);
}

//============== ThingSpeak Upload ===============
void uploadThingSpeak(String status){

  if(WiFi.status()!=WL_CONNECTED)
    return;

  HTTPClient http;

  String url =
  "http://api.thingspeak.com/update?api_key=" +
  apiKey +
  "&field1=" + String(temperature) +
  "&field2=" + String(humidity) +
  "&field3=" + String(smokeValue) +
  "&field4=" + status;

  http.begin(url);

  int code = http.GET();

  Serial.print("ThingSpeak Response: ");
  Serial.println(code);

  http.end();
}

//======================== LOOP =========================
void loop() {

  // Read sensors
  smokeValue = analogRead(MQ2_PIN);

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // If DHT fails, use previous values
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT Read Failed!");
    return;
  }

  Serial.println("-----------------------------");
  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity    : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Smoke Value : ");
  Serial.println(smokeValue);

  //================== FIRE LOGIC ==================

  fireDetected = false;
  warningState = false;

  if (smokeValue > 2500 || temperature > 50) {

    fireDetected = true;

  }
  else if (temperature >= 40 && temperature <= 50) {

    warningState = true;

  }

  //================== DISPLAY =====================

  if (fireDetected) {

    fireMode();

  }
  else if (warningState) {

    warningMode();

  }
  else {

    safeMode();

  }

  //============== Upload every 20 seconds =========

  if (millis() - uploadTimer > 20000) {

    uploadTimer = millis();

    if (fireDetected) {

      uploadThingSpeak("FIRE");

    }
    else if (warningState) {

      uploadThingSpeak("WARNING");

    }
    else {

      uploadThingSpeak("SAFE");

    }
  }

  delay(1000);
}