

/*
 *
 * arial_8
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : courier_8
 * Date                : 01.03.2013
 * Font size in bytes  : 4202
 * Font width          : 10
 * Font height         : 10
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef COURIER_8_H
#define COURIER_8_H

#define COURIER_8_WIDTH 10
#define COURIER_8_HEIGHT 10

static uint8_t courier_8[] PROGMEM = {
    0x10, 0x6A, // size
    0x0A, // width
    0x0A, // height
    0x20, // first char
    0x60, // char count
    
    // char widths
    0x00, 0x01, 0x03, 0x05, 0x04, 0x04, 0x04, 0x01, 0x02, 0x02, 
    0x05, 0x05, 0x02, 0x04, 0x02, 0x04, 0x04, 0x05, 0x04, 0x04, 
    0x05, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x02, 0x05, 0x05, 
    0x05, 0x04, 0x05, 0x05, 0x05, 0x04, 0x05, 0x05, 0x05, 0x05, 
    0x06, 0x03, 0x05, 0x06, 0x05, 0x06, 0x06, 0x04, 0x05, 0x04, 
    0x06, 0x04, 0x05, 0x06, 0x06, 0x06, 0x05, 0x06, 0x04, 0x02, 
    0x04, 0x02, 0x04, 0x06, 0x02, 0x05, 0x05, 0x04, 0x05, 0x04, 
    0x04, 0x05, 0x06, 0x03, 0x04, 0x05, 0x03, 0x06, 0x06, 0x04, 
    0x05, 0x05, 0x04, 0x04, 0x05, 0x06, 0x06, 0x06, 0x05, 0x06, 
    0x04, 0x03, 0x01, 0x03, 0x04, 0x05, 
    
    // font data
    0x5E, 0x00, // 33
    0x0E, 0x00, 0x0E, 0x00, 0x00, 0x00, // 34
    0xA4, 0x7E, 0xA5, 0x7E, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, // 35
    0x24, 0xEA, 0x33, 0x12, 0x00, 0x00, 0x00, 0x00, // 36
    0x14, 0x3A, 0x5C, 0x28, 0x00, 0x00, 0x00, 0x00, // 37
    0x60, 0x5C, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, // 38
    0x0E, 0x00, // 39
    0x7C, 0x82, 0x00, 0x00, // 40
    0x82, 0x7C, 0x00, 0x00, // 41
    0x04, 0x14, 0x0E, 0x14, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, // 42
    0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, // 43
    0xC0, 0x20, 0x00, 0x00, // 44
    0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, // 45
    0x40, 0x40, 0x00, 0x00, // 46
    0xC0, 0x30, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, // 47
    0x3C, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, // 48
    0x40, 0x42, 0x7E, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, // 49
    0x64, 0x52, 0x4A, 0x44, 0x00, 0x00, 0x00, 0x00, // 50
    0x24, 0x42, 0x52, 0x2C, 0x00, 0x00, 0x00, 0x00, // 51
    0x18, 0x14, 0x52, 0x7E, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, // 52
    0x20, 0x4E, 0x4A, 0x32, 0x00, 0x00, 0x00, 0x00, // 53
    0x78, 0x4E, 0x4A, 0x32, 0x00, 0x00, 0x00, 0x00, // 54
    0x06, 0x02, 0x72, 0x0E, 0x00, 0x00, 0x00, 0x00, // 55
    0x34, 0x4A, 0x4A, 0x34, 0x00, 0x00, 0x00, 0x00, // 56
    0x4C, 0x52, 0x72, 0x1E, 0x00, 0x00, 0x00, 0x00, // 57
    0x48, 0x00, // 58
    0x80, 0x48, 0x40, 0x00, // 59
    0x10, 0x10, 0x28, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, // 60
    0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, // 61
    0x44, 0x44, 0x28, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // 62
    0x04, 0x02, 0x52, 0x0C, 0x00, 0x00, 0x00, 0x00, // 63
    0x7C, 0x82, 0x92, 0xAA, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, // 64
    0x60, 0x5A, 0x16, 0x58, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, // 65
    0x42, 0x7E, 0x4A, 0x4A, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, // 66
    0x3C, 0x42, 0x42, 0x46, 0x00, 0x00, 0x00, 0x00, // 67
    0x42, 0x7E, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, // 68
    0x42, 0x7E, 0x4A, 0x42, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, // 69
    0x42, 0x7E, 0x4A, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, // 70
    0x3C, 0x42, 0x52, 0x32, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // 71
    0x42, 0x7E, 0x48, 0x0A, 0x7E, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 72
    0x42, 0x7E, 0x42, 0x00, 0x00, 0x00, // 73
    0x30, 0x40, 0x42, 0x3E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // 74
    0x42, 0x7E, 0x52, 0x18, 0x66, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 75
    0x42, 0x7E, 0x42, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, // 76
    0x7E, 0x4E, 0x10, 0x4E, 0x7E, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 77
    0x42, 0x7E, 0x4C, 0x32, 0x7E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 78
    0x3C, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, // 79
    0x42, 0x7E, 0x52, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, // 80
    0x3C, 0x42, 0xC2, 0xBC, 0x00, 0x00, 0x00, 0x00, // 81
    0x42, 0x7E, 0x52, 0x12, 0x2C, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 82
    0x66, 0x4A, 0x4A, 0x36, 0x00, 0x00, 0x00, 0x00, // 83
    0x06, 0x42, 0x7E, 0x42, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, // 84
    0x02, 0x3E, 0x40, 0x42, 0x3E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 85
    0x06, 0x3A, 0x40, 0x40, 0x3A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 86
    0x02, 0x3E, 0x42, 0x38, 0x42, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 87
    0x42, 0x66, 0x18, 0x66, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, // 88
    0x02, 0x06, 0x4A, 0x70, 0x4A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89
    0x66, 0x52, 0x4A, 0x66, 0x00, 0x00, 0x00, 0x00, // 90
    0xFE, 0x82, 0x00, 0x00, // 91
    0x01, 0x0E, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, // 92
    0x82, 0xFE, 0x00, 0x00, // 93
    0x04, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, // 94
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // 95
    0x02, 0x04, 0x00, 0x00, // 96
    0x60, 0x68, 0x68, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, // 97
    0x42, 0x7E, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, // 98
    0x30, 0x48, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, // 99
    0x30, 0x48, 0x4A, 0x7E, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, // 100
    0x30, 0x58, 0x58, 0x50, 0x00, 0x00, 0x00, 0x00, // 101
    0x48, 0x7C, 0x4A, 0x4A, 0x00, 0x00, 0x00, 0x00, // 102
    0x30, 0x48, 0x48, 0xF8, 0x08, 0x40, 0x40, 0x40, 0x00, 0x00, // 103
    0x42, 0x7E, 0x50, 0x08, 0x70, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 104
    0x48, 0x7A, 0x40, 0x00, 0x00, 0x00, // 105
    0x08, 0x08, 0x0A, 0xF8, 0x40, 0x40, 0x40, 0x40, // 106
    0x42, 0x7E, 0x10, 0x78, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, // 107
    0x42, 0x7E, 0x40, 0x00, 0x00, 0x00, // 108
    0x48, 0x78, 0x08, 0x70, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 109
    0x48, 0x78, 0x08, 0x08, 0x70, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 110
    0x30, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00, 0x00, // 111
    0x08, 0xF8, 0x48, 0x48, 0x30, 0x40, 0x40, 0x40, 0x00, 0x00, // 112
    0x30, 0x48, 0x48, 0xF8, 0x08, 0x00, 0x00, 0x40, 0x40, 0x40, // 113
    0x48, 0x78, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, // 114
    0x58, 0x58, 0x58, 0x28, 0x00, 0x00, 0x00, 0x00, // 115
    0x08, 0x3C, 0x48, 0x48, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // 116
    0x08, 0x38, 0x40, 0x48, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 117
    0x08, 0x18, 0x60, 0x60, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 118
    0x08, 0x38, 0x48, 0x30, 0x48, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 119
    0x48, 0x68, 0x10, 0x68, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, // 120
    0x08, 0x18, 0xA8, 0x40, 0x38, 0x08, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, // 121
    0x58, 0x68, 0x58, 0x68, 0x00, 0x00, 0x00, 0x00, // 122
    0x10, 0x7C, 0x82, 0x00, 0x00, 0x00, // 123
    0xFE, 0x00, // 124
    0x82, 0x7C, 0x10, 0x00, 0x00, 0x00, // 125
    0x10, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, // 126
    0x7E, 0x42, 0x42, 0x42, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00 // 127
    
};

#endif
