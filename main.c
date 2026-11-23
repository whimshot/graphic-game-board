#include "graphic-game-board.h"

int main()
{
    stdio_init_all();
    picocalc_init();
    lcd_enable_cursor(false);

    sb_write_lcd_backlight(255);      // Set the LCD backlight to maximum brightness
    sb_write_keyboard_backlight(255); // Set the keyboard backlight to maximum brightness

    init_board(&board);
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
            if (selector_c > 0)
            {
                draw_selector(selector_c - 1, selector_r);
            }
            break;
        case KEY_RIGHT:
            if (selector_c < BOARD_SIZE - 1)
            {
                draw_selector(selector_c + 1, selector_r);
            }
            break;
        case KEY_UP:
            if (selector_r > 0)
            {
                draw_selector(selector_c, selector_r - 1);
            }
            break;
        case KEY_DOWN:
            if (selector_r < BOARD_SIZE - 1)
            {
                draw_selector(selector_c, selector_r + 1);
            }
            break;
        default:
            break;
        }
    }
}
