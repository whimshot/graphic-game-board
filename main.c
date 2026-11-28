/**
 * @file main.c
 * @brief Main entry point and input loop for a two-player board game on PicoCalc.
 *
 * Summary:
 * Initializes the PicoCalc display, backlights, and initial board state, then
 * runs a blocking keyboard loop to handle game actions:
 * - Arrow Keys: Move the selection cursor within board boundaries.
 * - Return / Enter: Place the active player's disc (alternating white and black)
 *   at the current cursor position.
 * - Escape / Q / q: Exit back to the PicoCalc shell.
 */

#include "pico/bootrom.h"

#include "graphic-game-board.h"

int main()
{
    // Initialize standard I/O, PicoCalc hardware, and hide the text cursor
    stdio_init_all();
    picocalc_init();
    lcd_enable_cursor(false);

    // Set both LCD and keyboard backlights to maximum brightness (255)
    sb_write_lcd_backlight(255);
    sb_write_keyboard_backlight(255);

    // Set up the initial game board and game state
    //
    // void init_board(uint8_t (*b)[BOARD_SIZE][BOARD_SIZE], int board_type);
    //
    // Options for boardtype are
    //      CHECKERS
    //      REVERSI
    init_board(&board, CHECKERS);
    render_board(&board);

    // Track current turn: -1 represents white disc, 1 represents black disc
    int disc_color = -1;

    // Main game loop: continuously poll for keyboard input
    while (true)
    {
        uint8_t key = (uint8_t)keyboard_get_key(); // Wait for and read the next key press

        switch (key)
        {
        // Exit the application when Escape or Q/q is pressed
        case KEY_ESC:
        case 'Q':
        case 'q':
            picocalc_shell_exit();
            break;

        // Place a piece at the current selector position and switch turns
        case KEY_RETURN:
        case KEY_ENTER:
            select_square(&board, selector_r, selector_c);
            break;

        case KEY_UP:
            if (selector_r > 0)
            {
                move_selector(selector_r - 1, selector_c);
            }
            break;

        case KEY_DOWN:
            if (selector_r < BOARD_SIZE - 1)
            {
                move_selector(selector_r + 1, selector_c);
            }
            break;

        case KEY_RIGHT:
            if (selector_c < BOARD_SIZE - 1)
            {
                move_selector(selector_r, selector_c + 1);
            }
            break;

        case KEY_LEFT:
            if (selector_c > 0)
            {
                move_selector(selector_r, selector_c - 1);
            }
            break;

        case KEY_DEL:
            reset_usb_boot(0, 0);
            break;

        // Ignore any unmapped keys
        default:
            break;
        }
    }
}
