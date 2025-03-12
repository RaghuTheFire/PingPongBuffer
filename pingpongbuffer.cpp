#include <cstring>
#include <cstdint>
#include "ping_pong_buffer.hpp"

int ppbuf_insert_active(ppbuf_t *p, void *data, int size) 
{
    int ret = 0;
    unsigned char *ptr;

    if (p == nullptr || data == nullptr || size == 0) 
    {
        ret = -1; // check your parameters
    } 
    else 
    {
        if (size > (p->buffer_size - p->put_index)) 
        {
            ret = -1; // not enough room for new samples
        } 
        else 
        {
            int mem_position = ((p->ping) * p->buffer_size) + p->put_index;
            ptr = static_cast<unsigned char *>(p->buffer_data);

            std::memcpy(&ptr[mem_position], data, size); // copy the contents

            p->put_index += size;
            p->full_signal = (p->put_index >= p->buffer_size);

            ret = 0;
        }
    }
    return ret;
}

int ppbuf_remove_inactive(ppbuf_t *p, void *data, int size) 
{
    int ret = 0;
    unsigned char *ptr;

    if (p == nullptr || data == nullptr || size == 0) 
    {
        ret = -1; // check your parameters
    } 
    else 
    {
        if (size > (p->buffer_size - p->get_index)) 
        {
            ret = -1; // not enough data in sample buffer
        } 
        else 
        {
            int mem_position = ((p->pong) * p->buffer_size) + p->get_index;
            ptr = static_cast<unsigned char *>(p->buffer_data);

            std::memcpy(data, &ptr[mem_position], size); // copy the contents

            p->get_index += size;

            ret = 0;
        }
    }
    return ret;
}

unsigned char *ppbuf_dma_get_active_addr(ppbuf_t* p, int *size) 
{
    if (p == nullptr || size == nullptr) 
    {
        return nullptr; // no valid parameters return an invalid pointer
    } 
    else 
    {
        return static_cast<unsigned char *>(&p->buffer_data[p->pong * p->buffer_size]); // insertion buffer is always the pong
    }
}

unsigned char *ppbuf_dma_get_inactive_addr(ppbuf_t* p, int *size) 
{
    if (p == nullptr || size == nullptr) 
    {
        return nullptr; // no valid parameters return an invalid pointer
    } 
    else 
    {
        return static_cast<unsigned char *>(&p->buffer_data[p->ping * p->buffer_size]); // insertion buffer is always the pong
    }
}

int ppbuf_dma_force_swap(ppbuf_t* p) 
{
    int ret = 0;

    if (p == nullptr) 
    {
        ret = -1;
    } 
    else 
    {
        if (p->full_signal) 
        {
            p->full_signal = false;

            // swap the buffer switches
            p->ping = p->ping ^ p->pong;
            p->pong = p->pong ^ p->ping;
            p->ping = p->ping ^ p->pong;

            p->get_index = 0;
            p->put_index = 0;
        }
    }
    return ret;
}

bool ppbuf_get_full_signal(ppbuf_t *p, bool consume) 
{
    bool ret = (p != nullptr ? p->full_signal : false);

    if (consume && (p != nullptr) && ret)
    {
        p->full_signal = false;

        // swap the buffer switches
        p->ping = p->ping ^ p->pong;
        p->pong = p->pong ^ p->ping;
        p->ping = p->ping ^ p->pong;

        // resets the buffer position
        p->get_index = 0;
        p->put_index = 0;
    }

    return ret;
}

