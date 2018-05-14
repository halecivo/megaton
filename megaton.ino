#include <Arduino.h>
// Graphical LCD display 128x64
// driver ST7920

#include "U8glib.h"

// LCD connection pins
#define EN 13
#define RW 12
#define RS 11

// OLED display initialization
U8GLIB_ST7920_128X64_1X lcd(EN, RW, RS);

// timestamp of last LCD refresh
long int lastRefresh = 0;

// draw initial screen
void drawIntroScreen(void) {
    // font size settings, this option leads to
    // ca. 15x4 characters
    lcd.setFont(u8g_font_unifont);
    // print position setup, till 128, 64
    lcd.setPrintPos(0, 10);
    lcd.print("   MEGATON");
    lcd.setPrintPos(0, 25);
    lcd.print("music player");
    lcd.setPrintPos(0, 40);
    lcd.print("Run time:");
    lcd.setPrintPos(40, 55);
    lcd.print(millis()/1000);
    lcd.print(" seconds");
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
}

void loop(void) {
    if (millis()-lastRefresh > 100) {
        lcd.firstPage();
        do {
            drawIntroScreen();
        } while( lcd.nextPage() );
        lastRefresh = millis();
    }
    delay(10);
}
