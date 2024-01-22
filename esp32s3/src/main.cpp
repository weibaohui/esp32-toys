#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <TFT_eSPI.h>
#include <SPI.h>
#define led_pin 12
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 21, 15, U8X8_PIN_NONE);
TFT_eSPI tft = TFT_eSPI(480,272);       // Invoke custom library

unsigned long targetTime = 0;
byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11; // Colour order is RGB 5+6+5 bits each


const char *ssid = "qqqq";
const char *password = "cmcccmcc";
const String url = "https://v2.jokeapi.dev/joke/Programming";


void displayText(char *str) {
    u8g2.clearBuffer();
    /* 将字符串显示在屏幕中央 */
    u8g2.drawUTF8(u8g2.getDisplayWidth() / 2 - u8g2.getUTF8Width(str) / 2,
                  u8g2.getDisplayHeight() / 2 + u8g2.getMaxCharHeight() / 2, str);

    u8g2.sendBuffer();
}

void display(const char *str) {
    Serial.println(str);
    u8g2.clearBuffer();
//    u8g2.drawStr(0,15,str);
//    int length = str.length();
//    if (length > 15) {
//        int lines = length / 15;
//        for (int i = 0; i < lines; i++) {
//            u8g2.setCursor(0, i*30);
//            u8g2.print(str[30*i,30*i+30]);
//        }
//    }else{
        u8g2.setCursor(0, 15);
    u8g2.drawStr(0,15,str);
//    }
    u8g2.sendBuffer();


//    tft.setCursor(0, 10);    // Set cursor to x = 90, y = 210 without changing the font
    tft.setTextColor(TFT_GREEN); // This time we will use green text on a black background
    tft.setTextFont(4); // Select font 2
    //Text will wrap to the next line if needed, by luck it breaks the lines at a space..
    tft.println(str);

}

void displayWelcome() {
    char *str = "正在启动...";
    displayText(str);
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
    u8g2.clearBuffer();
    u8g2.setCursor(0, 10);
    u8g2.println("\nLoading joke...");

    String joke = getJoke();
    u8g2.clearDisplay();
    display(joke.c_str());
    u8g2.sendBuffer();
}


void setup() {
    Serial.begin(115200);
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, HIGH);

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);

    targetTime = millis() + 1000;

    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.begin();
    u8g2.enableUTF8Print();
    displayWelcome();/* 显示欢迎语，可去除 */


    WiFi.begin(ssid, password, 6);
    u8g2.clearBuffer();
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(100);
        Serial.println("正在等待wifi");
        u8g2.print(".");
        u8g2.sendBuffer();
    }
    char str[50];
    sprintf(str, "IP:%s", WiFi.localIP().toString().c_str());
    display(str);
    Serial.printf("WIFI 地址：%s",WiFi.localIP().toString().c_str());

    delay(3000);
    nextJoke();


}


void rainbow_fill() {
    // The colours and state are not initialised so the start colour changes each time the function is called

    for (int i = 479; i > 0; i--) {
        // Draw a vertical line 1 pixel wide in the selected colour
        tft.drawFastHLine(0, i, tft.width(),
                          colour); // in this example tft.width() returns the pixel width of the display
        // This is a "state machine" that ramps up/down the colour brightnesses in sequence
        switch (state) {
            case 0:
                green++;
                if (green == 64) {
                    green = 63;
                    state = 1;
                }
                break;
            case 1:
                red--;
                if (red == 255) {
                    red = 0;
                    state = 2;
                }
                break;
            case 2:
                blue++;
                if (blue == 32) {
                    blue = 31;
                    state = 3;
                }
                break;
            case 3:
                green--;
                if (green == 255) {
                    green = 0;
                    state = 4;
                }
                break;
            case 4:
                red++;
                if (red == 32) {
                    red = 31;
                    state = 5;
                }
                break;
            case 5:
                blue--;
                if (blue == 255) {
                    blue = 0;
                    state = 0;
                }
                break;
        }
        colour = red << 11 | green << 5 | blue;
    }
}


void loop() {
//    if (targetTime < millis()) {
//        targetTime = millis() + 10000;
//
//        rainbow_fill(); // Fill the screen with rainbow colours

//        // The standard AdaFruit font still works as before
//        tft.setTextColor(TFT_GOLD); // Background is not defined so it is transparent
//        tft.setCursor (100, 5);
//        tft.setTextFont(1);        // Select font 1 which is the Adafruit GLCD font
//        tft.print("Original Adafruit font!");
//
//        // The new larger fonts do not need to use the .setCursor call, coords are embedded
//        tft.setTextColor(TFT_GREENYELLOW); // Do not plot the background colour
//        // Overlay the black text on top of the rainbow plot (the advantage of not drawing the background colour!)
//        tft.drawCentreString("Font size 2", 160, 14, 2); // Draw text centre at position 120, 14 using font 2
//        tft.drawCentreString("Font size 4", 160, 30, 4); // Draw text centre at position 120, 30 using font 4
//        tft.drawCentreString("12.34", 160, 54, 6);       // Draw text centre at position 120, 54 using font 6
//        tft.drawCentreString("12.34 is in font 6", 160, 92, 2); // Draw text centre at position 120, 92 using font 2
//        // Note the x, y position is the top left corner of the first character printed!
//
//        // draw a floating point number
//        float pi = 3.14159; // Value to print
//        int precision = 3;  // Number of digits after decimal point
//        int xpos = 130;     // x position
//        int ypos = 110;     // y position
//        int font = 2;       // font number 2
//        xpos += tft.drawFloat(pi, precision, xpos, ypos, font); // Draw rounded number and return new xpos delta for next print position
//        tft.drawString(" is pi", xpos, ypos, font);             // Continue printing from new x position
//
//        tft.setTextSize(1);           // We are using a text size multiplier of 1
//
//        tft.setTextColor(TFT_RED);  // Set text colour to black, no background (so transparent)
//        tft.setCursor(76, 150, 4);    // Set cursor to x = 76, y = 150 and use font 4
//        tft.println("Transparent...");  // As we use println, the cursor moves to the next line
//
//        tft.setCursor(70, 175);    // Set cursor to x = 70, y = 175
//        tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text colour to white and background to black
//        tft.println("White on black");
//
//        tft.setTextFont(4);        // Select font 4 without moving cursor
//        tft.setCursor(00, 210);    // Set cursor to x = 90, y = 210 without changing the font
//        tft.setTextColor(TFT_WHITE);
//
//        // By using the print class we can use all the formatting features like printing HEX
//        tft.print(57005, HEX);    // Cursor does no move to next line
//        tft.println(48879, HEX);  // print and move cursor to next line
//
//        tft.setTextColor(TFT_GREEN, TFT_BLACK); // This time we will use green text on a black background
//        tft.setTextFont(2); // Select font 2
//        //Text will wrap to the next line if needed, by luck it breaks the lines at a space..
//        tft.println(" Ode to a Small Lump of Green Putty I Found in My Armpit One Midsummer Morning ");
//
//        tft.drawCentreString("34.56", 160, 300, 7);       // Draw text centre at position 120, 54 using font 6
//        tft.drawCentreString("34.56 is in font 7", 160, 350, 2); // Draw text centre at position 120, 92 using font 2
//
//        tft.drawCentreString("78.90", 160, 370, 8);       // Draw text centre at position 120, 54 using font 6
//        tft.drawCentreString("78.90 is in font 8", 160, 450, 2); // Draw text centre at position 120, 92 using font 2
//    }
}

