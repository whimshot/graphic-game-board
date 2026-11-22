#include <stdio.h>
#include "pico/stdlib.h"

#if PICO_RP2350
    #include "third_party/picocalc-shell/picocalc_shell.h"
#endif

#include "drivers/font.h"
#include "drivers/keyboard.h"
#include "drivers/lcd.h"
#include "drivers/southbridge.h"

// RGB565 Color Helper
#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define COLOR_BG         RGB565(20, 24, 28)
#define COLOR_BOARD      RGB565(0, 128, 64)
#define COLOR_GRID       RGB565(0, 60, 30)
#define COLOR_BLACK_DISC RGB565(18, 18, 22)
#define COLOR_WHITE_DISC RGB565(240, 240, 245)
#define COLOR_CURSOR     RGB565(255, 215, 0)

int main()
{
    stdio_init_all();

    sb_init();
    lcd_init();

    lcd_solid_rectangle(COLOR_BOARD, 10, 10, 100, 100);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
