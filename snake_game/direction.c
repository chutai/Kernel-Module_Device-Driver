#include "direction.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
bool open_Device()
{
    fd = -1;
    memset(bind_array, 0, MAX_KEY_OF_BIND * sizeof(bind_array[0]));
    dir_of_xbox = EMPTY;
    fd = open(DEVICE_PATH, O_WRONLY);
    if (-1 == fd)
        return false;
    else
        return true;
}

enum dir get_Direction()
{
    char get_Byte[32];
    if (0 == bind_array[0]) /* chua co phim nao duoc bind */
    {
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            int temp = (int)get_Byte[5];
            if (xUP == temp)
                dir_of_xbox = UP;
            else if (xDOWN == temp)
                dir_of_xbox = DOWN;
            else if (xLEFT == temp)
                dir_of_xbox = LEFT;
            else if (xRIGHT == temp)
                dir_of_xbox = RIGHT;
        }
        return dir_of_xbox;
    }
    else /* co phim duoc bind */
    {
        int position = 0;
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            int temp = (int)get_Byte[5];
            if (0 != temp)
            {
                while (1 != temp)
                {
                    temp = temp >> 1;
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

            }
        }
        return dir_of_xbox;
    }
}

bool bind_key()
{
    for (int i = 0; i < MAX_KEY_OF_BIND; i++)
    {
        bind_array[i] = (1 << i);
    }
    char get_Byte[32];
    int key_bind, key_function;
    int count_to_break = 0;
    printf("Nhap phim muon bind:\n"); /* nhap A, bind voi LEFT */
    while (1)
    {
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            if (0 != (int)get_Byte[5])
            {
                key_bind = (int)get_Byte[5];
                count_to_break++;
                if (2 == count_to_break)
                {
                    count_to_break = 0;
                    break;
                }
            }
        }
    }
    while (1)
    {
        if (-1 != read(fd, &get_Byte, MAX_SIZE))
        {
            if (0 != (int)get_Byte[5])
            {
                key_function = (int)get_Byte[5];
                count_to_break++;
                if (2 == count_to_break)
                {
                    count_to_break = 0;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_KEY_OF_BIND; i++)
    {
        if ((1 << i) == key_bind)
        {
            bind_array[i] == key_function;
            break;
        }
    }
}