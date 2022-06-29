// #include "oled-wing-adafruit.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(AUTOMATIC);

#define TMP36 A0
#define STATUSLED D6

#include "oled-wing-adafruit.h"
boolean reverseColor = true;


OledWingAdafruit display;
//int lastReading = 0;
signed int xpos = 0;
double ypos = 0;
double counter = 0;
bool countersubtracting = true;

void setup()
{
    pinMode(TMP36, INPUT);
    pinMode(STATUSLED, OUTPUT);
    display.setup();

    display.clearDisplay();
    display.display();
}

void doDisplayStuff(int xcounter, int ycounter, double fahrenheit) {
    int color = reverseColor ? WHITE : BLACK;
    display.setCursor(20 + xcounter, 4 + ycounter);
    display.drawFastHLine(15 + xcounter, 0 + ycounter, 96, color);
    display.drawFastHLine(15 + xcounter, 30 + ycounter, 96, color);
    display.drawFastVLine(15 + xcounter, 0 + ycounter, 31, color);
    display.drawFastVLine(111 + xcounter, 0 + ycounter, 31, color);
    // display.drawTriangle(15 + xcounter, 0, 15 + xcounter, 30, 0 + xcounter, 15, WHITE);
    // display.drawTriangle(111 + xcounter, 0, 111 + xcounter, 30, 126 + xcounter, 15, WHITE);
    // //if (lastReading > round(temp)) {
    //     display.fillTriangle(15 + xcounter, 0, 15 + xcounter, 30, 0 + xcounter, 15, WHITE);
    // //} else if (lastReading < round(temp)) {
    //     display.fillTriangle(111 + xcounter, 0, 111 + xcounter, 30, 126 + xcounter, 15, WHITE);
    //}
    //lastReading = round(temp);
    display.println(fahrenheit);
}
void doMoreDisplayStuff(int xpos, int ypos, double fahrenheit) {
    doDisplayStuff(xpos, ypos, fahrenheit);
    doDisplayStuff(xpos - 100, ypos, fahrenheit);
    doDisplayStuff(xpos + 100, ypos, fahrenheit);
    doDisplayStuff(xpos + (100 * 2), ypos, fahrenheit);
    doDisplayStuff(xpos - (100 * 2), ypos, fahrenheit);
}

void loop()
{
    display.loop();
    double reading = analogRead(TMP36);
    double millivolts = (reading / 4095) * 3300; // I looked up the formula before you shared it with the class lol
    double temp = (millivolts - 500) / 10;
    double fahrenheit = (temp * 1.8) + 32;
    reverseColor ? display.clearDisplay() : display.fillScreen(WHITE);
    display.setTextWrap(false);
    display.setTextSize(3);
    reverseColor ? display.setTextColor(WHITE) : display.setTextColor(BLACK);
    int ytemppos = round(ypos);
    doMoreDisplayStuff(xpos, ytemppos, fahrenheit);
    doMoreDisplayStuff(xpos, ytemppos + 31, fahrenheit);
    doMoreDisplayStuff(xpos, ytemppos - 33, fahrenheit);
    doMoreDisplayStuff(xpos, ytemppos - (33 * 2), fahrenheit);
    doMoreDisplayStuff(xpos, ytemppos + (31 * 2), fahrenheit);

    xpos += 2;
    
    counter += countersubtracting ? -1 : 1;
    if (counter > 400 || counter < -400) {
        countersubtracting = !countersubtracting;
        reverseColor = !reverseColor;
    }
    ypos -= counter / (double) 100;
    if (xpos >= 100) {
        xpos = 0;
    }
    if (ypos >= 31) {
        ypos = 0;
    }
    if (ypos < 0) {
        ypos = 31;
    }
    display.display();
}