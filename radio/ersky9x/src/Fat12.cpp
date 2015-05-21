/****************************************************************************
*  Copyright (c) 2013 by Michael Blandford. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* Original source of this code: Primoz Beltram
* http://sourceforge.net/projects/fat12ctrlface/files/
*
 * Other Authors (alphabetical order)
 * - Andre Bernet <bernet.andre@gmail.com>
 * - Andreas Weitl
 * - Bertrand Songis <bsongis@gmail.com>
 * - Bryan J. Rentoul (Gruvin) <gruvin@gmail.com>
 * - Cameron Weeks <th9xer@gmail.com>
 * - Erez Raviv
 * - Gabriel Birkus
 * - Jean-Pierre Parisy
 * - Karl Szmutny <shadow@privy.de>
 * - Michal Hlavinka
 * - Pat Mackenzie
 * - Philip Moss
 * - Rob Thomson
 * - Romolo Manfredini <romolo.manfredini@gmail.com>
 * - Thomas Husterer
*
****************************************************************************/



#include <stdint.h>
#include "string.h"
#include "myeeprom.h"
#include "file.h"

#define BLOCKSIZE 512

//------------------------------------------------------------------------------
/**
 * FAT12 boot sector partition.
 */
const char g_FATboot[BLOCKSIZE] = {
    // See http://en.wikipedia.org/wiki/FAT12#Boot_Sector
    0xeb, 0x3c, 0x90, // Jump instruction.
    0x39, 0x58, 0x20, 0x54, 0x45, 0x41, 0x4D, 0x00, // OEM Name
    0x00, 0x02, // Bytes per sector
    0x08, // Sectors per FS cluster.
    0x01, 0x00, // Reserved sector count
    
		0x01, // Number of FATs
    0x10, 0x00, // Number of root directory entries
#ifdef REVX
    0x03, 0x08, // Total sectors = 2051
#else
    0x03, 0x06, // Total sectors = 1539
#endif
    0xf8, // Media descriptor
    0x01, 0x00, // Sectors per FAT table
    0x08, 0x00, // Sectors per track
    0x40, 0x00, // Number of heads
    0x00, 0x00, 0x00, 0x00, // Number of hidden sectors
    
		0x00, 0x00, 0x00, 0x00, // Large number of sectors.
    0x00, // Physical drive number
    0x00, // Reserved
    0x29, // Extended boot signature
    0xCE, 0xFA, 0x5C, 0xD1, // Disk ID (serial number)
    0x56, 0x49, 0x52, 0x54, 0x46, 0x53, 0x20, 0x20, 0x20, 0x20, 0x20, // Volume label
    0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20, // FAT file system type
    0x00, 0x00, // OS boot code
    
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
};

const char g_FAT[BLOCKSIZE] =
{
#ifdef REVX
		0xF8, 0xFF, 0xFF, 0x03, 0x40, 0x00, 0x05, 0x60, 0x00, 0x07, 0x80, 0x00, 0x09, 0xA0, 0x00, 0x0B,
		0xC0, 0x00, 0x0D, 0xE0, 0x00, 0x0F, 0x00, 0x01, 0x11, 0x20, 0x01, 0x13, 0x40, 0x01, 0x15, 0x60,
		0x01, 0x17, 0x80, 0x01, 0x19, 0xA0, 0x01, 0x1B, 0xC0, 0x01, 0x1D, 0xE0, 0x01, 0x1F, 0x00, 0x02,
		0x21, 0x20, 0x02, 0x23, 0x40, 0x02, 0x25, 0x60, 0x02, 0x27, 0x80, 0x02, 0x29, 0xA0, 0x02, 0x2B,
		0xC0, 0x02, 0x2D, 0xE0, 0x02, 0x2F, 0x00, 0x03, 0x31, 0x20, 0x03, 0x33, 0x40, 0x03, 0x35, 0x60,
		0x03, 0x37, 0x80, 0x03, 0x39, 0xA0, 0x03, 0x3B, 0xC0, 0x03, 0x3D, 0xE0, 0x03, 0x3F, 0x00, 0x04,
		0x41, 0x20, 0x04, 0x43, 0x40, 0x04, 0x45, 0x60, 0x04, 0x47, 0x80, 0x04, 0x49, 0xA0, 0x04, 0x4B,
		0xC0, 0x04, 0x4D, 0xE0, 0x04, 0x4F, 0x00, 0x05, 0x51, 0x20, 0x05, 0x53, 0x40, 0x05, 0x55, 0x60,
		0x05, 0x57, 0x80, 0x05, 0x59, 0xA0, 0x05, 0x5B, 0xC0, 0x05, 0x5D, 0xE0, 0x05, 0x5F, 0x00, 0x06,
		0x61, 0x20, 0x06, 0x63, 0x40, 0x06, 0x65, 0x60, 0x06, 0x67, 0x80, 0x06, 0x69, 0xA0, 0x06, 0x6B,
		0xC0, 0x06, 0x6D, 0xE0, 0x06, 0x6F, 0x00, 0x07, 0x71, 0x20, 0x07, 0x73, 0x40, 0x07, 0x75, 0x60,
		0x07, 0x77, 0x80, 0x07, 0x79, 0xA0, 0x07, 0x7B, 0xC0, 0x07, 0x7D, 0xE0, 0x07, 0x7F, 0x00, 0x08,
		0x81, 0xF0, 0xFF, 0x83, 0x40, 0x08, 0x85, 0x60, 0x08, 0x87, 0x80, 0x08, 0x89, 0xA0, 0x08, 0x8B,
		0xC0, 0x08, 0x8D, 0xE0, 0x08, 0x8F, 0x00, 0x09, 0x91, 0x20, 0x09, 0x93, 0x40, 0x09, 0x95, 0x60,
		0x09, 0x97, 0x80, 0x09, 0x99, 0xA0, 0x09, 0x9B, 0xC0, 0x09, 0x9D, 0xE0, 0x09, 0x9F, 0x00, 0x0A,
		0xA1, 0x20, 0x0A, 0xA3, 0x40, 0x0A, 0xA5, 0x60, 0x0A, 0xA7, 0x80, 0x0A, 0xA9, 0xA0, 0x0A, 0xAB,
		0xC0, 0x0A, 0xAD, 0xE0, 0x0A, 0xAF, 0x00, 0x0B, 0xB1, 0x20, 0x0B, 0xB3, 0x40, 0x0B, 0xB5, 0x60,
		0x0B, 0xB7, 0x80, 0x0B, 0xB9, 0xA0, 0x0B, 0xBB, 0xC0, 0x0B, 0xBD, 0xE0, 0x0B, 0xBF, 0x00, 0x0C,
		0xC1, 0x20, 0x0C, 0xC3, 0x40, 0x0C, 0xC5, 0x60, 0x0C, 0xC7, 0x80, 0x0C, 0xC9, 0xA0, 0x0C, 0xCB,
		0xC0, 0x0C, 0xCD, 0xE0, 0x0C, 0xCF, 0x00, 0x0D, 0xD1, 0x20, 0x0D, 0xD3, 0x40, 0x0D, 0xD5, 0x60,
		0x0D, 0xD7, 0x80, 0x0D, 0xD9, 0xA0, 0x0D, 0xDB, 0xC0, 0x0D, 0xDD, 0xE0, 0x0D, 0xDF, 0x00, 0x0E,
		0xE1, 0x20, 0x0E, 0xE3, 0x40, 0x0E, 0xE5, 0x60, 0x0E, 0xE7, 0x80, 0x0E, 0xE9, 0xA0, 0x0E, 0xEB,
		0xC0, 0x0E, 0xED, 0xE0, 0x0E, 0xEF, 0x00, 0x0F, 0xF1, 0x20, 0x0F, 0xF3, 0x40, 0x0F, 0xF5, 0x60,
		0x0F, 0xF7, 0x80, 0x0F, 0xF9, 0xA0, 0x0F, 0xFB, 0xC0, 0x0F, 0xFD, 0xE0, 0x0F, 0xFF, 0x00, 0x10,
		0x01, 0xF1, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#else
		0xF8, 0xFF, 0xFF, 0x03, 0x40, 0x00, 0x05, 0x60, 0x00, 0x07, 0x80, 0x00, 0x09, 0xA0, 0x00, 0x0B,
		0xC0, 0x00, 0x0D, 0xE0, 0x00, 0x0F, 0x00, 0x01, 0x11, 0x20, 0x01, 0x13, 0x40, 0x01, 0x15, 0x60,
		0x01, 0x17, 0x80, 0x01, 0x19, 0xA0, 0x01, 0x1B, 0xC0, 0x01, 0x1D, 0xE0, 0x01, 0x1F, 0x00, 0x02,
		0x21, 0x20, 0x02, 0x23, 0x40, 0x02, 0x25, 0x60, 0x02, 0x27, 0x80, 0x02, 0x29, 0xA0, 0x02, 0x2B,
		0xC0, 0x02, 0x2D, 0xE0, 0x02, 0x2F, 0x00, 0x03, 0x31, 0x20, 0x03, 0x33, 0x40, 0x03, 0x35, 0x60,
		0x03, 0x37, 0x80, 0x03, 0x39, 0xA0, 0x03, 0x3B, 0xC0, 0x03, 0x3D, 0xE0, 0x03, 0x3F, 0x00, 0x04,
		0x41, 0x20, 0x04, 0x43, 0x40, 0x04, 0x45, 0x60, 0x04, 0x47, 0x80, 0x04, 0x49, 0xA0, 0x04, 0x4B,
		0xC0, 0x04, 0x4D, 0xE0, 0x04, 0x4F, 0x00, 0x05, 0x51, 0x20, 0x05, 0x53, 0x40, 0x05, 0x55, 0x60,
		0x05, 0x57, 0x80, 0x05, 0x59, 0xA0, 0x05, 0x5B, 0xC0, 0x05, 0x5D, 0xE0, 0x05, 0x5F, 0x00, 0x06,
		0x61, 0x20, 0x06, 0x63, 0x40, 0x06, 0x65, 0x60, 0x06, 0x67, 0x80, 0x06, 0x69, 0xA0, 0x06, 0x6B,
		0xC0, 0x06, 0x6D, 0xE0, 0x06, 0x6F, 0x00, 0x07, 0x71, 0x20, 0x07, 0x73, 0x40, 0x07, 0x75, 0x60,
		0x07, 0x77, 0x80, 0x07, 0x79, 0xA0, 0x07, 0x7B, 0xC0, 0x07, 0x7D, 0xE0, 0x07, 0x7F, 0x00, 0x08,
		0x81, 0xF0, 0xFF, 0x83, 0x40, 0x08, 0x85, 0x60, 0x08, 0x87, 0x80, 0x08, 0x89, 0xA0, 0x08, 0x8B,
		0xC0, 0x08, 0x8D, 0xE0, 0x08, 0x8F, 0x00, 0x09, 0x91, 0x20, 0x09, 0x93, 0x40, 0x09, 0x95, 0x60,
		0x09, 0x97, 0x80, 0x09, 0x99, 0xA0, 0x09, 0x9B, 0xC0, 0x09, 0x9D, 0xE0, 0x09, 0x9F, 0x00, 0x0A,
		0xA1, 0x20, 0x0A, 0xA3, 0x40, 0x0A, 0xA5, 0x60, 0x0A, 0xA7, 0x80, 0x0A, 0xA9, 0xA0, 0x0A, 0xAB,
		0xC0, 0x0A, 0xAD, 0xE0, 0x0A, 0xAF, 0x00, 0x0B, 0xB1, 0x20, 0x0B, 0xB3, 0x40, 0x0B, 0xB5, 0x60,
		0x0B, 0xB7, 0x80, 0x0B, 0xB9, 0xA0, 0x0B, 0xBB, 0xC0, 0x0B, 0xBD, 0xE0, 0x0B, 0xBF, 0x00, 0x0C,
		0xC1, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif
};


//	File Attributes
//	0 	0x01 	Read Only.
//	1 	0x02 	Hidden.
//	2 	0x04 	System.
//	3 	0x08 	Volume Label.
//	4 	0x10 	Subdirectory.
//	5 	0x20 	Archive.
//	6 	0x40 	Device.
//	7 	0x80 	Reserved.

typedef struct
{
    uint8_t name[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t create_time_ms;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t ea_index;
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t start_cluster;
    uint32_t file_size;
} FATDirEntry_t;

// First 16 FAT root directory entries (1 sector)
const FATDirEntry_t g_DIRroot[16] =
{
    {
        { 'E', 'R', 'S', 'K', 'Y', '_', '9', 'X'},
        { ' ', ' ', ' '},
        0x08,		// Volume
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { 'E', 'R', 'S', 'K', 'Y', '9', 'X', ' '},
        { 'B', 'I', 'N'},
        0x26,		// Archive, hidden, system, read-only
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0002,
        0x00080000
    },
    {
        { 'F', 'I', 'R', 'M', 'W', 'A', 'R', 'E'},
        { 'B', 'I', 'N'},
        0x26,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0082,
#ifdef REVX
        0x00080000
#else
        0x00040000
#endif
    },
    {
        { 'A', '2', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '3', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '4', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '5', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '6', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '7', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '8', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', '9', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', 'A', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { 'A', 'C', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x27,
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0000,
        0x00000000
    },
    {
        { '\0', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
//    {
//        { 'A', 'E', ' ', ' ', ' ', ' ', ' ', ' '},
//        { ' ', ' ', ' '},
//        0x27,
//        0x00,
//        0x3E,
//        0xA301,
//        0x3D55,
//        0x3D55,
//        0x0000,
//        0xA302,
//        0x3D55,
//        0x0000,
//        0x00000000
//    }
    {
        { '\0', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    }
};


int fat12Read( uint8_t *buffer, uint16_t sector )
{
	if (sector == 0)
	{
	  memcpy( buffer, g_FATboot, BLOCKSIZE ) ;
	  return TRUE;
	}
	else if ( sector == 1/*Reserved sector count*/)
	{
	  // FAT table.
	  memcpy( buffer, g_FAT, BLOCKSIZE);
	  return TRUE;
	}
	else if ( sector == 2)
	{
	  // Directory
	  memcpy( buffer, g_DIRroot, BLOCKSIZE ) ;
	  return TRUE;
	}

	sector -= 3 ;
	if ( sector < 1024 )
	{
		ee32_read_512( sector, buffer ) ;
	}	
#ifdef REVX
	else if ( sector < 2048 )
#else
	else if ( sector < 1536 )
#endif
	{
		uint32_t address ;
		address = sector - 1024 ;
		address *= 512 ;
		address += 0x00400000 ;
  	memcpy( buffer, (uint8_t *)address, BLOCKSIZE ) ;
	}
	else
	{
    memset( buffer, 0, BLOCKSIZE ) ;
	}
	return TRUE;
}


uint32_t program( uint32_t *address, uint32_t *buffer )	;		// size is 256 bytes
uint32_t ee32_write( const uint8_t *buffer, uint32_t sector, uint32_t count ) ;

int fat12Write( const uint8_t *buffer, uint16_t sector, uint32_t count )
{
	// TO DO, actually write to the EEPROM
	if ( sector >= 3 )
	{
		sector -= 3 ;
		// Look for a 4K block boundary, writing 4K+
		if ( sector < 1024 )
		{
			ee32_write( buffer, sector, count ) ;
		}	
#ifdef REVX
		else if ( sector < 2048 )
#else
		else if ( sector < 1536 )
#endif
		{
			// FIRMWARE
			uint32_t address ;
			address = sector - 1024 ;
			address *= 512 ;
			address += 0x00400000 ;
			// Write to flash
			uint32_t i ;
			while ( count )
			{
				for ( i = 0 ; i < 2 ; i += 1 )
				{
					if ( address >= 0x00408000 )		// Protect bootloader
					{
						program( (uint32_t *)address, (uint32_t *) buffer ) ;	// size is 256 bytes
					}
					address += 256 ;
					buffer += 256 ;
				}
				count -= 1 ;
			}
		}
	}
  return TRUE;
}


