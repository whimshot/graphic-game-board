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

// Board layout constants (8x8 grid of 36x36 px cells = 288x288 px)
// The board is square so we don't have to store separate x and y
// constants.
#define BOARD_SIZE 8 // 8x8 board = 64 squares
#define CELL_SIZE 36
#define SCREEN_PX 320
#define BOARD_PX (BOARD_SIZE * CELL_SIZE)      // 288x288 pixel board
#define OFFSET_PX ((SCREEN_PX - BOARD_PX) / 2) // 16px margin left, right, top, and bottom

// RGB565 Color Helper
#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define COLOR_BG RGB565(20, 24, 28)
#define COLOR_BOARD RGB565(2, 64, 2)
#define COLOR_GRID RGB565(2, 4, 2)
#define COLOR_BLACK_DISC RGB565(0, 0, 0)
#define COLOR_WHITE_DISC RGB565(240, 240, 245)
#define COLOR_SELECTION_BOX RGB565(255, 215, 0)

// Piece states
enum
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

// Game state
static uint8_t board[BOARD_SIZE][BOARD_SIZE];
static int selection_box_r = 2;
static int selection_box_c = 3;

static void init_board(void)
{
    memset(board, EMPTY, sizeof(board));
    // Standard Othello starting position
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

void draw_selection_box(int new_c, int new_r)
{
    // if the selection box position has changed erase the old selection box
    if (new_r != selection_box_r || new_c != selection_box_c)
    {
        int lx = OFFSET_PX + CELL_SIZE * selection_box_c;       // left selection box bar
        int rx = OFFSET_PX + CELL_SIZE * (selection_box_c + 1); // right selection box bar
        int ly = OFFSET_PX + CELL_SIZE * selection_box_r;       // top selection box bar
        int ry = OFFSET_PX + CELL_SIZE * (selection_box_r + 1); // bottom selection box bar
        int l = CELL_SIZE + 2;                           // selection box border bar length
        int w = 2;                                       // selection box border bar width
        lcd_solid_rectangle(COLOR_GRID, lx, ly, w, l);   // Left
        lcd_solid_rectangle(COLOR_GRID, lx, ly, l, w);   // Top
        lcd_solid_rectangle(COLOR_GRID, rx, ly, w, l);   // Right
        lcd_solid_rectangle(COLOR_GRID, lx, ry, l, w);   // Bottom
    }
    int lx = OFFSET_PX + CELL_SIZE * new_c;          // left selection box bar
    int rx = OFFSET_PX + CELL_SIZE * (new_c + 1);    // right selection box bar
    int ly = OFFSET_PX + CELL_SIZE * new_r;          // top selection box bar
    int ry = OFFSET_PX + CELL_SIZE * (new_r + 1);    // bottom selection box bar
    int l = CELL_SIZE + 2;                           // selection box border bar length
    int w = 2;                                       // selection box border bar width
    lcd_solid_rectangle(COLOR_SELECTION_BOX, lx, ly, w, l); // Left
    lcd_solid_rectangle(COLOR_SELECTION_BOX, lx, ly, l, w); // Top
    lcd_solid_rectangle(COLOR_SELECTION_BOX, rx, ly, w, l); // Right
    lcd_solid_rectangle(COLOR_SELECTION_BOX, lx, ry, l, w); // Bottom

    // set new selection box position
    selection_box_c = new_c;
    selection_box_r = new_r;
}

void render_graphic_board(void)
{
    // Draw green felt board
    lcd_solid_rectangle(COLOR_BOARD, OFFSET_PX, OFFSET_PX, BOARD_PX, BOARD_PX);

    // Draw grid lines
    int girdStart = OFFSET_PX;
    for (int i = girdStart; i < BOARD_PX; i += CELL_SIZE)
    {
        lcd_solid_rectangle(COLOR_GRID, i, OFFSET_PX, 2, BOARD_PX);
        lcd_solid_rectangle(COLOR_GRID, OFFSET_PX, i, BOARD_PX, 2);
    }

    for (int r = 0; r < BOARD_SIZE; r++)
    {
        for (int c = 0; c < BOARD_SIZE; c++)
        {
            if (board[r][c] == EMPTY)
                continue;

            int cx = (OFFSET_PX + CELL_SIZE * r) + CELL_SIZE / 2;
            int cy = (OFFSET_PX + CELL_SIZE * c) + CELL_SIZE / 2;
            // put a dot in the center of the cell
            uint16_t disc_color = (board[r][c] == BLACK) ? COLOR_BLACK_DISC : COLOR_WHITE_DISC;
            lcd_solid_rectangle(disc_color, cx, cy, 3, 3);
        }
    }
    draw_selection_box(selection_box_r, selection_box_c);
}

int main()
{
    stdio_init_all();

    sb_init();
    keyboard_init();
    lcd_init();
    picocalc_init();
    lcd_enable_cursor(false);

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
            break;
        case KEY_LEFT:
            if (selection_box_c > 0){
                draw_selection_box(selection_box_c - 1, selection_box_r);
                printf("\033[Hselection_box_c: %d, selection_box_r: %d", selection_box_c, selection_box_r);
            }
            break;
        case KEY_RIGHT:
            if (selection_box_c < BOARD_SIZE - 1){
                draw_selection_box(selection_box_c + 1, selection_box_r);
                printf("\033[Hselection_box_c: %d, selection_box_r: %d", selection_box_c, selection_box_r);
            }
            break;
        case KEY_UP:
            if (selection_box_r > 0){
                draw_selection_box(selection_box_c, selection_box_r - 1);
                printf("\033[Hselection_box_c: %d, selection_box_r: %d", selection_box_c, selection_box_r);
            }
            break;
        case KEY_DOWN:
            if (selection_box_r < BOARD_SIZE - 1){
                draw_selection_box(selection_box_c, selection_box_r + 1);
                printf("\033[Hselection_box_c: %d, selection_box_r: %d", selection_box_c, selection_box_r);
            }
            break;
        default:
            break;
        }
    }
}
