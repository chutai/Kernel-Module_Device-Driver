#ifndef _DIRECTION_H_
#define _DIRECTION_H_
#include <stdbool.h>
#define DEVICE_PATH "/dev/tgamepad0"
#define MAX_KEY_OF_BIND 8
#define MAX_SIZE 32
#define xUP (0x01 << 0)
#define xDOWN (0x01 << 1)
#define xLEFT (0x01 << 2)
#define xRIGHT (0x01 << 3)
#define xA (0x01 << 4)
#define xB (0x01 << 5)
#define xX (0x01 << 6)
#define xY (-128)
static int fd;
static int bind_array[MAX_KEY_OF_BIND];
enum dir {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PAUSE,
    QUIT,
    X,
    Y,
    EMPTY
};
bool isBind;
static volatile enum dir dir_of_xbox;
void set_dir_of_xbox(enum dir);
enum dir get_dir_of_xbox(void);
bool open_Device();
enum dir get_Direction(); /* true if Xbox is pressed  */
bool bind_key();
bool valid_button(int pos, unsigned char raw);
void close_file();
char get_char(int value);
#endif