#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#if PICO_RP2350
#include "third_party/picocalc-shell/picocalc_shell.h"
#endif

#include "drivers/display.h"
#include "drivers/font.h"
#include "drivers/keyboard.h"
#include "drivers/lcd.h"
#include "drivers/onboard_led.h"
#include "drivers/picocalc.h"
#include "drivers/southbridge.h"

volatile bool user_interrupt = false;

#define LCD_WIDTH 320
#define LCD_HEIGHT 320
#define BOARD_WIDTH 280
#define BOARD_HEIGHT 280
#define BOARD_UPPER_X 20 // upper corner of board (x)
#define BOARD_UPPER_Y 20 // upper corner of board (y)
#define CELL_WIDTH 40
#define CELL_HEIGHT 40
#define LINE_START 60 // Where the first line starts, works for both vertical and horizontal
#define SCREEN_W 320
#define SCREEN_H 320

// Board layout constants (8x8 grid of 36x36 px cells = 288x288 px)
#define BOARD_SIZE 8
#define CELL_SIZE 36
#define BOARD_PX (BOARD_SIZE * CELL_SIZE)
#define OFFSET_X ((SCREEN_W - BOARD_PX) / 2) // 16px margin
#define OFFSET_Y ((SCREEN_H - BOARD_PX) / 2) // 16px margin

// RGB565 Color Helper
#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define COLOR_BG RGB565(20, 24, 28)
#define COLOR_BOARD RGB565(2, 64, 2)
#define COLOR_GRID RGB565(40, 0, 0)
#define COLOR_BLACK_DISC RGB565(0, 0, 0)
#define COLOR_WHITE_DISC RGB565(240, 240, 245)
#define COLOR_CURSOR RGB565(255, 215, 0)

// Piece states
enum
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

// Game state
static uint8_t board[BOARD_SIZE][BOARD_SIZE];
static int cursor_r = 2;
static int cursor_c = 3;

static void init_board(void)
{
    memset(board, EMPTY, sizeof(board));
    // Standard Othello starting position
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

void render_graphic_board(void)
{
    lcd_solid_rectangle(COLOR_BOARD, OFFSET_X, OFFSET_Y, BOARD_PX, BOARD_PX);
    int girdStart = OFFSET_X + CELL_SIZE;
    for (int i = girdStart; i < BOARD_PX; i += CELL_SIZE)
    {
        lcd_solid_rectangle(COLOR_GRID, i, OFFSET_X, 2, BOARD_PX);
        lcd_solid_rectangle(COLOR_GRID, OFFSET_Y, i, BOARD_PX, 2);
    }

    for (int r = 0; r < BOARD_SIZE; r++)
    {
        for (int c = 0; c < BOARD_SIZE; c++)
        {
            if (board[r][c] == EMPTY) continue;

            int cx = (OFFSET_X + CELL_SIZE * r) + CELL_SIZE / 2;
            int cy = (OFFSET_Y + CELL_SIZE * c) + CELL_SIZE / 2;
            // put a dot in the center of the cell
            uint16_t disc_color = (board[r][c] == BLACK) ? COLOR_BLACK_DISC : COLOR_WHITE_DISC;
            lcd_solid_rectangle(disc_color, cx, cy, 3, 3);
        }
    }
}

int main()
{
    stdio_init_all();

    sb_init();
    keyboard_init();
    lcd_init();
    picocalc_init();

    sb_write_lcd_backlight(255);      // Set the LCD backlight to maximum brightness
    sb_write_keyboard_backlight(255); // Set the keyboard backlight to maximum brightness

    init_board();
    render_graphic_board();

    while (true)
    {
        uint8_t key = (uint8_t)keyboard_get_key(); // Get next key pressed
        switch (key)
        {
        case KEY_ESC:
        case 'Q':
        case 'q':
            picocalc_shell_exit();
            break;
        case KEY_RETURN:
        case KEY_ENTER:
            render_graphic_board();
            break;
        default:
            break;
        }
    }
}
