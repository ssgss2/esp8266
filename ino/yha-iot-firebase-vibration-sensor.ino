#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR FIREBASE AUTHORIZATION KEY"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define VIBRATION_PIN D2
#define BUZZER_PIN D3

void setup()
{
  Serial.begin(9600);
  
  setupWiFi();

  pinMode(VIBRATION_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setBool("share/buzz", false);
  if(Firebase.success()){
    Serial.print("Firebase write success!");
  } else {
    Serial.print("Firebase write failed!");
    Serial.println("");
    Serial.print(Firebase.error());
  }
  Serial.println("");
}

int timePassed (int time) {
  int diff = 0;
  if (millis() <= time) {
    diff = (69666 - time) + millis();
  } else {
    diff = millis() - time;
  }
  return diff;
}
long vibration;
int checkFirebaseTime = 0;
bool buzz = false;
bool buzzing = false;
void loop()
{
  if (timePassed (checkFirebaseTime) >= 1000) {
    setVibration();
    buzz = Firebase.getBool("share/buzz");
    if(buzz && Firebase.success() && !buzzing) buzzBuzzer();
    checkFirebaseTime = millis();
  }
}

void buzzBuzzer(){
  buzzing = true;
  Firebase.setBool("share/buzz", false);
  Serial.print("\n");
  Serial.println("Buzzing Buzzer");
  Serial.print("\n");
  //tone( pin number, frequency in hertz, duration in milliseconds);
  tone(BUZZER_PIN,1300,500);
  delay(500);
  digitalWrite(BUZZER_PIN,LOW);
  buzzing = false;
}

void setVibration(){
  delay(10);
  vibration = pulseIn(VIBRATION_PIN, HIGH);
  Serial.print("\n");
  Serial.print("Vibration: ");
  Serial.print(vibration);
  Serial.print("\n");
  Firebase.setInt("share/vibration",vibration);
  if(vibration > 9000){
    Firebase.setBool("share/buzz",true);
  }
}

void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println();
}
