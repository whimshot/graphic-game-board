#include "graphic-game-board.h"

volatile bool user_interrupt = false;

static uint8_t board[BOARD_SIZE][BOARD_SIZE];

// Game state
// create the board array
// static uint8_t board[BOARD_SIZE][BOARD_SIZE];
// starting position for the selection box
static int selection_box_r = 2;
static int selection_box_c = 3;

// setup the starting positions
static void init_board(void)
{
    memset(board, EMPTY, sizeof(board));
    // Standard Reversi starting position
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

void draw_selector(int new_c, int new_r)
{
    // if the selection box position has changed erase the old selection box
    if (new_r != selection_box_r || new_c != selection_box_c)
    {
        int lx = OFFSET_PX + CELL_SIZE * selection_box_c;       // left selection box bar
        int rx = OFFSET_PX + CELL_SIZE * (selection_box_c + 1); // right selection box bar
        int ly = OFFSET_PX + CELL_SIZE * selection_box_r;       // top selection box bar
        int ry = OFFSET_PX + CELL_SIZE * (selection_box_r + 1); // bottom selection box bar
        int l = CELL_SIZE + 2;                                  // selection box border bar length
        int w = 2;                                              // selection box border bar width
        lcd_solid_rectangle(COLOR_GRID, lx, ly, w, l);          // Left
        lcd_solid_rectangle(COLOR_GRID, lx, ly, l, w);          // Top
        lcd_solid_rectangle(COLOR_GRID, rx, ly, w, l);          // Right
        lcd_solid_rectangle(COLOR_GRID, lx, ry, l, w);          // Bottom
    }
    int lx = OFFSET_PX + CELL_SIZE * new_c;                 // left selection box bar
    int rx = OFFSET_PX + CELL_SIZE * (new_c + 1);           // right selection box bar
    int ly = OFFSET_PX + CELL_SIZE * new_r;                 // top selection box bar
    int ry = OFFSET_PX + CELL_SIZE * (new_r + 1);           // bottom selection box bar
    int l = CELL_SIZE + 2;                                  // selection box border bar length
    int w = 2;                                              // selection box border bar width
    lcd_solid_rectangle(COLOR_SELECTION_BOX, lx, ly, w, l); // Left
    lcd_solid_rectangle(COLOR_SELECTION_BOX, lx, ly, l, w); // Top
    lcd_solid_rectangle(COLOR_SELECTION_BOX, rx, ly, w, l); // Right
    lcd_solid_rectangle(COLOR_SELECTION_BOX, lx, ry, l, w); // Bottom

    // set new selection box position
    selection_box_c = new_c;
    selection_box_r = new_r;
}

void render_board(uint8_t (*b)[BOARD_SIZE][BOARD_SIZE])
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
            if ((*b)[r][c] == EMPTY)
                continue;

            int x = (OFFSET_PX + CELL_SIZE * r) + 2;
            int y = (OFFSET_PX + CELL_SIZE * c) + 2;
            // put a dot in the center of the cell
            if ((*b)[r][c] == BLACK) {
                lcd_blit(black_disc, x, y, 34, 34);
            }
            else {
                lcd_blit(white_disc, x, y, 34, 34);
            }
        }
    }
    draw_selector(selection_box_r, selection_box_c);
}

int main()
{
    stdio_init_all();
    picocalc_init();
    lcd_enable_cursor(false);

    sb_write_lcd_backlight(255);      // Set the LCD backlight to maximum brightness
    sb_write_keyboard_backlight(255); // Set the keyboard backlight to maximum brightness

    init_board();
    render_board(&board);
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
            if (selection_box_c > 0)
            {
                draw_selector(selection_box_c - 1, selection_box_r);
            }
            break;
        case KEY_RIGHT:
            if (selection_box_c < BOARD_SIZE - 1)
            {
                draw_selector(selection_box_c + 1, selection_box_r);
            }
            break;
        case KEY_UP:
            if (selection_box_r > 0)
            {
                draw_selector(selection_box_c, selection_box_r - 1);
            }
            break;
        case KEY_DOWN:
            if (selection_box_r < BOARD_SIZE - 1)
            {
                draw_selector(selection_box_c, selection_box_r + 1);
            }
            break;
        default:
            break;
        }
    }
}
