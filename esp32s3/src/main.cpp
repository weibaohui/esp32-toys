#include <Arduino.h>
//#include <u8flib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "u8g2_for_TFT_eSPI.h"

#define led_pin 12
TFT_eSPI tft = TFT_eSPI(480, 272);       // Invoke custom library
U8g2_for_TFT_eSPI u8f;


const char *ssid = "qqqq";
const char *password = "cmcccmcc";
const String url = "https://v2.jokeapi.dev/joke/Programming";


void display(const char *str) {
    Serial.println(str);
    tft.fillScreen(TFT_BLACK);
    u8f.setForegroundColor(TFT_WHITE);      // apply color
    u8f.setFontMode(0);
    u8f.drawUTF8(10,10,str);

}

void displayWelcome() {
    char *str = "正在启动...";
    display(str);
}

String getJoke() {
    HTTPClient http;
    http.useHTTP10(true);
    http.begin(url);
    http.GET();
    String result = http.getString();

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, result);

    // Test if parsing succeeds.
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return "<error>";
    }

    String type = doc["type"].as<String>();
    String joke = doc["joke"].as<String>();
    String setup = doc["setup"].as<String>();
    String delivery = doc["delivery"].as<String>();
    http.end();
    return type.equals("single") ? joke : setup + "  " + delivery;
}

void nextJoke() {



    String joke = getJoke();
    display(joke.c_str());
}


void setup() {
    Serial.begin(115200);
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, HIGH);

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(20);
    tft.begin();
    u8f.begin(tft);                     // connect u8f procedures to TFT_eSPI


    u8f.setFont(u8g2_font_wqy12_t_gb2312); // icon font
    u8f.setFontMode(0);

    displayWelcome();/* 显示欢迎语，可去除 */
    delay(2000);


    WiFi.begin(ssid, password, 6);
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(100);
        Serial.println("正在等待wifi");
        u8f.print(".");
    }
    char str[50];
    sprintf(str, "IP:%s", WiFi.localIP().toString().c_str());
    display(str);
    Serial.printf("WIFI 地址：%s", WiFi.localIP().toString().c_str());

    delay(3000);
    nextJoke();


}


void loop() {

    // UTF-8 string: "<" 550 448 664 ">"
}

