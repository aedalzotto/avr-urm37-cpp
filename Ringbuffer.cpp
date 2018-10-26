/**
 * @file Ringbuffer.cpp
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (C) 2018 by Angelo Elias Dalzotto
 * 
 * @brief This is a simple ringbuffer library. It is intended for
 * microcontrollers but can be used with any C++ compiler.
 * 
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Ringbuffer.h>

Ringbuffer::Ringbuffer(uint16_t size)
{
    buffer = nullptr;

    buffer = (uint8_t*)malloc(size);
    if(!buffer){
        error = Error::OUT_OF_MEMORY;
        size = 0;
        full = true;
    } else {
        error = Error::NO_ERROR;
        this->size = size;
        full = false;
    }

    head = tail = 0;
    empty = true;
}

Ringbuffer::~Ringbuffer()
{
    free(buffer);
    buffer = nullptr;
}

Ringbuffer::Error Ringbuffer::get_error()
{
    return error;
}

bool Ringbuffer::write(uint8_t byte)
{
    if(full){
        error = Error::BUFFER_FULL;
        return false;
    }

    buffer[tail++] = byte;

    tail %= size;
    if(tail == head)
        full = true;

    empty = false;

    return true;
}

bool Ringbuffer::write(uint8_t *buff_ptr, uint16_t size)
{
    if(full){
        error = Error::BUFFER_FULL;
        return false;
    }

    if(tail >= head){
        uint16_t bottom = this->size - tail;
        if(bottom + head >= size){
            memcpy(buffer+tail, buff_ptr, bottom);
            memcpy(buffer, buff_ptr+bottom, size-bottom);
            tail += size;
            tail %= this->size;
        } else {
            error = Error::BUFFER_FULL;
            return false;
        }
    } else {
        if(head - tail >= size){
            memcpy(buffer+tail, buff_ptr, size);
            tail += size;
        } else {
            error = Error::BUFFER_FULL;
            return false;
        }
    }

    if(tail == head)
        full = true;

    empty = false;

    return true;    
}

uint8_t Ringbuffer::read()
{
    if(empty){
        error = Error::BUFFER_EMPTY;
        return -1;
    }

    uint8_t ret = buffer[head++];
    head %= size;

    if(head == tail)
        empty = true;

    full = false;

    return ret;
}

bool Ringbuffer::read(uint8_t *buff_ptr, uint16_t size)
{
    if(empty){
        error = Error::BUFFER_EMPTY;
        return -1;
    }

    if(tail >= head){
        if(tail - head >= size){
            memcpy(buff_ptr, buffer+head, size);
            head += size;
        } else {
            error = Error::BUFFER_EMPTY;
            return false;
        }
    } else {
        uint16_t bottom = this->size - head;
        if(bottom + tail >= size){
            memcpy(buff_ptr, buffer+head, bottom);
            memcpy(buff_ptr+bottom, buffer, size-bottom);
            head += size;
            head %= this->size;
        } else {
            error = Error::BUFFER_EMPTY;
            return false;
        }
    }

    if(tail == head)
        empty = true;

    full = false;

    return true;
}

uint16_t Ringbuffer::available()
{
    return tail >= head ? tail - head : size - head + tail;
}

bool Ringbuffer::is_empty()
{
    return empty;
}