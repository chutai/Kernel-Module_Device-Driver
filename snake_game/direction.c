#include "direction.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
bool open_Device()
{

    int i = 0, temp = 0;
    fd = -1;
    for (i = 0; i < (MAX_KEY_OF_BIND - 1); i++)
    {

        bind_array[i] = (0x01 << i); /* [0]UP [1]DOWN [2]LEFT ... */
    }
    bind_array[MAX_KEY_OF_BIND - 1] = xY;
    dir_of_xbox = EMPTY;
    isBind = false;

    fd = open(DEVICE_PATH, O_RDONLY);
    if (-1 == fd)
    {
        return false;
    }
    else
    {
        return true;
    }
}

enum dir get_Direction()
{
    char get_Byte[MAX_SIZE];
    if ((false == isBind)) /* chua co phim nao duoc bind */
    {
        int rc = read(fd, &get_Byte, MAX_SIZE);
        if ((0 < rc))
        {

            int8_t UP_DOWN_LEFT_RIGHT = (int8_t)get_Byte[5];
            int A_B_X_Y = (int)get_Byte[4];
            if (xUP == UP_DOWN_LEFT_RIGHT && valid_button(5, get_Byte[5]))
            {
                dir_of_xbox = UP;

            }
            else if (xDOWN == UP_DOWN_LEFT_RIGHT && valid_button(5, get_Byte[5]))
            {
                dir_of_xbox = DOWN;

            }
            else if (xLEFT == UP_DOWN_LEFT_RIGHT && valid_button(5, get_Byte[5]))
            {
                dir_of_xbox = LEFT;
  
            }
            else if (xRIGHT == UP_DOWN_LEFT_RIGHT && valid_button(5, get_Byte[5]))
            {
                dir_of_xbox = RIGHT;
 
            }
            /* Group ABXY */
            else if (xA == A_B_X_Y && valid_button(4, get_Byte[4]))
            {
                dir_of_xbox = PAUSE;
                while ((0 != (int)get_Byte[4]))
                {
                    read(fd, &get_Byte, MAX_SIZE);
                }
            }
            else if (xB == A_B_X_Y && valid_button(4, get_Byte[4]))
            {
                while (0 != (int8_t)get_Byte[5])
                {
                    read(fd, &get_Byte, MAX_SIZE);
                }
                dir_of_xbox = QUIT;
            }
            else if (xX == A_B_X_Y && valid_button(4, get_Byte[4]))
            {

                dir_of_xbox = X;
            }
            else if ((xY) == A_B_X_Y && valid_button(4, get_Byte[4]))
            {
                dir_of_xbox = Y;
            }
        }
        return dir_of_xbox;
    }
    else /* co phim duoc bind */
    {

        int position = 0;
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            int8_t UP_DOWN_LEFT_RIGHT = (int8_t)get_Byte[5];
            int8_t A_B_X_Y = (int8_t)get_Byte[4];
            if (xY == A_B_X_Y)
            {
                dir_of_xbox = Y;
                return dir_of_xbox;
            }
            if ((0 != UP_DOWN_LEFT_RIGHT) && (valid_button(5, (int8_t)get_Byte[5])))
            {
                while (1 < UP_DOWN_LEFT_RIGHT)
                {
                    UP_DOWN_LEFT_RIGHT = UP_DOWN_LEFT_RIGHT >> 1;
                    position++;
                }
                if (xUP == bind_array[position])
                    dir_of_xbox = UP;
                else if (xDOWN == bind_array[position])
                    dir_of_xbox = DOWN;
                else if (xLEFT == bind_array[position])
                    dir_of_xbox = LEFT;
                else if (xRIGHT == bind_array[position])
                    dir_of_xbox = RIGHT;
                if (xA == bind_array[position])
                    dir_of_xbox = PAUSE;
                else if (xB == bind_array[position])
                    dir_of_xbox = QUIT;
                else if (xX == bind_array[position])
                    dir_of_xbox = X;
 
            }
            if (0 != A_B_X_Y)
            {
                if (xY == A_B_X_Y)
                {
                    dir_of_xbox = Y;
                    return dir_of_xbox;
                }
                while (1 < A_B_X_Y)
                {
                    A_B_X_Y = A_B_X_Y >> 1;
                    position++;
                }
                if (xUP == bind_array[position])
                    dir_of_xbox = UP;
                else if (xDOWN == bind_array[position])
                    dir_of_xbox = DOWN;
                else if (xLEFT == bind_array[position])
                    dir_of_xbox = LEFT;
                else if (xRIGHT == bind_array[position])
                    dir_of_xbox = RIGHT;
                if (xA == bind_array[position])
                    dir_of_xbox = PAUSE;
                else if (xB == bind_array[position])
                    dir_of_xbox = QUIT;
                else if (xX == bind_array[position])
                    dir_of_xbox = X;

            }
        }
        return dir_of_xbox;
    }
}

bool bind_key()
{
    printf("MOVE:\ta:LEFT \td:RIGHT \n\tw:UP \ts:DOWN\n\tA:PAUSE\tB:QUIT\n");

    char get_Byte[MAX_SIZE];
    int8_t key_bind = 0, key_function = 0;
    printf("%s\n", "Do you want bind a Key: "); /* nhap A, bind voi LEFT */
    while (1)
    {
        //printf("in wwhile bind: \n");
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            if (0 != (int8_t)get_Byte[5]) 
            {
                key_bind = (int8_t)get_Byte[5];
                do
                {
                    read(fd, &get_Byte, MAX_SIZE);
                } while (0 != (int8_t)get_Byte[5]);
                break;
            }
            if (0 != (int8_t)get_Byte[4]) 
            {
                key_bind = (int8_t)get_Byte[4];
                do
                {
                    read(fd, &get_Byte, MAX_SIZE);
                } while (0 != (int8_t)get_Byte[4]);
                break;
            }
        }
    }

    //printf("pass wwhile bind: \n");
    if ((0 != key_bind) && (xY != key_bind))
    {
        isBind = true;
        char temp = get_char(key_bind);
        printf("%c", temp);
        printf("%s", " ---> Key: ");
    }
    else
    {
        isBind = false;
        printf("No! Cancel!\n");
        return false;
    }

    while (1)
    {
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            if (0 != (int8_t)get_Byte[5]) 
            {
                key_function = (int8_t)get_Byte[5];
                do
                {
                    read(fd, &get_Byte, MAX_SIZE);
                } while (0 != (int8_t)get_Byte[5]);
                break;
            }
            if (0 != (int8_t)get_Byte[4]) 
            {
                key_function = (int8_t)get_Byte[4];
                do
                {
                    read(fd, &get_Byte, MAX_SIZE);
                } while (0 != (int8_t)get_Byte[4]);
                break;
            }
        }
    }

    for (int i = 0; i < MAX_KEY_OF_BIND; i++)
    {
        if ((0x01 << i) == key_bind)
        {
            //printf("vi tri phim da nhap %d:\n", i);
            bind_array[i] = key_function;
            break;
        }
    }

    if (0 != key_bind)
    {
        printf("%c\n", get_char(key_function));
        printf("Bind success\n");
        return true;
    }
    else
    {
        printf("Error\n");
        return false;
    }
}

bool valid_button(int pos, unsigned char raw)
{
    unsigned char buff[MAX_SIZE];
    int rc;
    int count = 0;
    if (pos < 4 || pos > 5)
    {
        return false;
    }
    while (count < 2)
    {
        rc = read(fd, buff, MAX_SIZE);
        if (rc < 18)
        {
            continue;
        }
        if (raw == buff[pos])
        {
            count++;
        }
        else
        {
            return false;
        }
    }
    return true;
}
void close_file()
{
    close(fd);
}

void set_dir_of_xbox(enum dir direction)
{
    dir_of_xbox = direction;
}
enum dir get_dir_of_xbox(void)
{
    return dir_of_xbox;
}

char get_char(int value)
{
    if (xUP == value)
    {
        return 'w';
    }
    else if (xDOWN == value)
    {
        return 's';
    }
    else if (xLEFT == value)
    {
        return 'a';
    }
    else if (xRIGHT == value)
    {
        return 'd';
    }
    else if (xA == value) /* Group ABXY */
    {
        return 'A';
    }
    else if (xB == value)
    {
        return 'B';
    }
    else if (xX == value)
    {
        return 'X';
    }
    else if ((xY) == value)
    {
        return 'Y';
    }
    return '?';
}
