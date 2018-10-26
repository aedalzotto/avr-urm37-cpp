/**
 * @file Ringbuffer.h
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

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

class Ringbuffer {
public:

    /** 
     * Enum Error
     * Enumerates the possible errors in the library. 
     */
    enum class Error {
        NO_ERROR,
        OUT_OF_MEMORY,
        BUFFER_OVERRUN,
        BUFFER_EMPTY
    };

    /**
     * @brief Ringbuffer class constructor.
     * 
     * @details It allocates the memory for the buffer.
     * 
     * @param size The buffer size.
     */
    Ringbuffer(uint16_t size);
    
    /**
     * @brief Ringbuffer class destructor.
     * 
     * @details Frees the memory used by the buffer.
     */
    ~Ringbuffer();

    /**
     * @brief Ringbuffer error getter.
     * 
     * @returns The error state of the object.
     */
    Error get_error();

    /**
     * @brief Writes a byte to the buffer.
     * 
     * @param byte The byte to be written.
     */
    bool write(uint8_t byte);
    
    /**
     * @brief Write an array to the buffer.
     * 
     * @param *buff_ptr Pointer to the array to be written.
     * @param size The size of the arary to be written.
     * 
     * @returns True if success, false if fails.
     */
    bool write(uint8_t *buff_ptr, uint16_t size);
    
    /**
     * @brief Reads a byte from the buffer.
     * 
     * @returns A byte read.
     */
    uint8_t read();
    
    /**
     * @brief Reads an array from the buffer.
     * 
     * @param *buff_ptr The pointer to the destination array.
     * @param size The number of bytes to read.
     * 
     * @returns True if success, false if fails.
     */
    bool read(uint8_t *buff_ptr, uint16_t size);
    
    /**
     * @brief Gets the number of bytes in the buffer.
     * 
     * @returns Number of bytes available to read.
     */
    uint16_t available();
    
    /**
     * @brief Gets if the buffer is empty.
     * 
     * @returns True if empty.
     */
    bool is_empty();

    /**
     * @brief Gets if the buffer is full.
     * 
     * @returns True if full;
     */
    bool is_full();

private:
    uint8_t *buffer;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    bool full;
    bool empty;
    Error error;
    
};

#endif /* _RINGBUFFER_H_ */