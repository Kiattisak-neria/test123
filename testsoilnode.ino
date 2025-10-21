
#include <Wire.h>
#include<WiFi.h>
#include<PubSubClient.h>
#include<NTPClient.h>
#include<WiFiUdp.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

#define WiFi_SSID "............"
#define WiFi_PASSWORD "kkkkoooo"

int Rsoil=36;
int soil1=0;

const char *mqtt_broker = "broker.hivemq.com";
const int mqtt_Port = 1883;

const char *topic_soil1 = "soil3";

  WiFiClient espClient;
  PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WiFi_SSID,WiFi_PASSWORD);
while(WiFi.status() != WL_CONNECTED)
  {
delay(500);
  Serial.print(".w");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

client.setServer(mqtt_broker, 1883);
lcd.init();
lcd.backlight();
delay(500);
}

void mq(){
  client.publish(topic_soil1,String(soil1).c_str());
}

void reconnect() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESPClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds"); 
      delay(5000);
   }}}

void aggg(){
    HTTPClient http;
        String url = "https://script.google.com/macros/s/AKfycbzeqH_fO_7LG8OfSLq2YoilfLIOkkIeCWsj5TynDeQoS9L2RCwwGELQOkakgq_CrNxT/exec?data3="+String(soil1);
        Serial.println("Making a request");
        http.begin(url.c_str()); //Specify the URL and certificate
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode = http.GET();
        String payload;
        if (httpCode > 0) { //Check for the returning code
          payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
        }
        else {
          Serial.println("Error on HTTP request");
          delay(1000);
        }
        http.end();
}

void LLCd(){
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Moisture");
  lcd.setCursor(1,1);
  lcd.print("Moisture=");
  lcd.setCursor(10,1);
  lcd.print(soil1);
}


     
void loop() { 
  //soil1=analogRead(Rsoil);
  //LLCd();
  soil1+=1;
  client.loop();
  reconnect();
    mq();
    Serial.println(soil1);
    delay(3000);
   //aggg();
}

