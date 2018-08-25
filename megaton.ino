#include <Arduino.h>
// Graphical LCD display 128x64
// driver ST7920

#include "U8glib.h"

// LCD connection pins
#define EN 13
#define RW 12
#define RS 11

typedef enum {
    BTN_PLAY = 4,
    BTN_REW = 6,
    BTN_FF = 3,
    BTN_STOP = 5,
    BTN_PAUSE = 2
} ButtonPin;

typedef enum {
    EBTN_PLAY = 0,
    EBTN_REW,
    EBTN_FF,
    EBTN_STOP,
    EBTN_PAUSE
} ButtonID;

typedef struct {
    ButtonID id;
    ButtonPin pin;
    int lastState;
} Button;

Button buttons[5] = {
        {.id = EBTN_PLAY, .pin = BTN_PLAY, .lastState = 1},
        {.id = EBTN_REW, .pin = BTN_REW, .lastState = 1},
        {.id = EBTN_FF, .pin = BTN_FF, .lastState = 1},
        {.id = EBTN_STOP, .pin = BTN_STOP, .lastState = 1},
        {.id = EBTN_PAUSE, .pin = BTN_PAUSE, .lastState = 1},
};

// OLED display initialization
U8GLIB_ST7920_128X64_1X lcd(EN, RW, RS);

// timestamp of last LCD refresh
long int lastRefresh = 0;

String line0 = "LINE0          ";
String line1 = "LINE1          ";
String line2 = "LINE2          ";
String line3 = "LINE3          ";

String * lines[] = {&line0, &line1, &line2, &line3};

// draw initial screen
void draw(void) {
    // font size settings, this option leads to
    // ca. 15x4 characters
    lcd.setFont(u8g_font_unifont);
    // print position setup, till 128, 64
    lcd.setPrintPos(0, 10);
    lcd.print(line0);
    lcd.setPrintPos(0, 25);
    lcd.print(line1);
    lcd.setPrintPos(0, 40);
    lcd.print(line2);
    lcd.setPrintPos(0, 55);
    lcd.print(line3);
}

void setup(void) {
    // white color for print
    if ( lcd.getMode() == U8G_MODE_R3G3B2 ) {
        lcd.setColorIndex(255);
    }
    // maximal light intensity
    else if ( lcd.getMode() == U8G_MODE_GRAY2BIT ) {
        lcd.setColorIndex(3);
    }
    // pixel visibility
    else if ( lcd.getMode() == U8G_MODE_BW ) {
        lcd.setColorIndex(1);
    }

    pinMode(BTN_PLAY, INPUT_PULLUP);
    pinMode(BTN_REW, INPUT_PULLUP);
    pinMode(BTN_FF, INPUT_PULLUP);
    pinMode(BTN_STOP, INPUT_PULLUP);
    pinMode(BTN_PAUSE, INPUT_PULLUP);

    Serial.begin(115200);
    Serial.println("I READY");
}

String readString;
unsigned int x, y;
char str[64];

void loop(void) {
    if (Serial.available()) {
        Serial.println("I READ");
        readString = Serial.readStringUntil('\n');

        switch(readString[0]){
            case 'P': {
                sscanf(readString.substring(1).c_str(), "%u %u %[^\n]",&x,&y,str);
                String * line = lines[y];
                String change = line->substring(0, x);
                change += str;
                change += line->substring(x + strlen(str));
                *line = change;
                break;
            }
            case 'C': {
                unsigned int len;
                sscanf(readString.substring(1).c_str(), "%u %u %u",&y,&x,&len);
                if(len == 0)
                {
                    len = 16;
                }
                String * line = lines[y];
                for (unsigned int i = x; (i < (x+len)) && (i < 16); ++i) {
                    line->setCharAt(i,' ');
                }
                break;
            }
            default:
                break;
        }
    }

    for (int j = 0; j <= EBTN_PAUSE; ++j) {
        int state = digitalRead(buttons[j].pin);
        if(buttons[j].lastState != state)
        {
            Serial.print("B ");
            Serial.print(j);
            Serial.print(" ");
            Serial.println(!state);
            buttons[j].lastState = state;
        }
    }

    if (millis()-lastRefresh > 100) {
        lcd.firstPage();
        do {
            draw();
            if(str[0] != '\0'){
                lcd.setFont(u8g_font_unifont);
                lcd.setPrintPos(x,y);
                lcd.print(str);
            }
        } while( lcd.nextPage() );
        lastRefresh = millis();
    }
    str[0] = '\0';
    delay(10);
}
