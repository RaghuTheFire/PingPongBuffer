#include <iostream>
#include <array>
#include "ping_pong_buffer.hpp"

/* creates a 32 byte ping-pong-buffer*/
PPBUF_DECLARE(test, 32);

int main() 
{
    unsigned char data = 0;
    unsigned char buf = 0;

    /* fill the active buffer with some raw value */
    while (!ppbuf_get_full_signal(&test, false)) 
    {
        ppbuf_insert_active(&test, &data, sizeof(data));
        std::cout << "data inserted on ping buffer: " << static_cast<unsigned int>(data);
        /* insert odd values in active buffer */
        data += 2;

        /* gets the data on the pong buffer */
        ppbuf_remove_inactive(&test, &buf, sizeof(buf));
        std::cout << "    data got from pong buffer: " << static_cast<unsigned int>(buf) << std::endl;
    }

    /* buffer full lets swap it */
    ppbuf_get_full_signal(&test, true);

    std::cout << "SWAPPING BUFFERS! " << std::endl;

    data = 1;

    /* fill the active buffer with some raw value */
    while (!ppbuf_get_full_signal(&test, false)) 
    {
        ppbuf_insert_active(&test, &data, sizeof(data));
        std::cout << "data inserted on ping buffer: " << static_cast<unsigned int>(data);
        /* insert even values in active buffer */
        data += 2;

        /* gets the data on the pong buffer */
        ppbuf_remove_inactive(&test, &buf, sizeof(buf));
        std::cout << "    data got from pong buffer: " << static_cast<unsigned int>(buf) << std::endl;
    }

    return 0;
}

