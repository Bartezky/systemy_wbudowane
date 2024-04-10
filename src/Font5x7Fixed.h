/**
** 5x7 font
*
*
* Author Rob Jennings
*/
#pragma once
#include <Adafruit_GFX.h>

const uint8_t Font5x7FixedBitmaps[] PROGMEM = {
  0xFA, 0xB4, 0x52, 0xBE, 0xAF, 0xA9, 0x40, 0x23, 0xE8, 0xE2, 0xF8, 0x80,
  0xC6, 0x44, 0x44, 0x4C, 0x60, 0x64, 0xA8, 0x8A, 0xC9, 0xA0, 0xD8, 0x00,
  0x6A, 0xA4, 0x00, 0x95, 0x58, 0x00, 0x25, 0x5D, 0xF7, 0x54, 0x80, 0x21,
  0x3E, 0x42, 0x00, 0xD0, 0xF8, 0x00, 0xF0, 0x08, 0x88, 0x88, 0x00, 0x74,
  0x67, 0x5C, 0xC5, 0xC0, 0x59, 0x24, 0xB8, 0x00, 0x74, 0x42, 0x22, 0x23,
  0xE0, 0xF8, 0x88, 0x20, 0xC5, 0xC0, 0x11, 0x95, 0x2F, 0x88, 0x40, 0xFC,
  0x21, 0xE0, 0xC5, 0xC0, 0x32, 0x21, 0xE8, 0xC5, 0xC0, 0xF8, 0x44, 0x44,
  0x21, 0x00, 0x74, 0x62, 0xE8, 0xC5, 0xC0, 0x74, 0x62, 0xF0, 0x89, 0x80,
  0xF3, 0xC0, 0xF3, 0x60, 0x12, 0x48, 0x42, 0x10, 0xF8, 0x3E, 0x00, 0x84,
  0x21, 0x24, 0x80, 0x74, 0x42, 0x22, 0x00, 0x80, 0x74, 0x6B, 0x7B, 0xC1,
  0xC0, 0x22, 0xA3, 0xF8, 0xC6, 0x20, 0xF4, 0x63, 0xE8, 0xC7, 0xC0, 0x74,
  0x61, 0x08, 0x45, 0xC0, 0xE4, 0xA3, 0x18, 0xCB, 0x80, 0xFC, 0x21, 0xE8,
  0x43, 0xE0, 0xFC, 0x21, 0xE8, 0x42, 0x00, 0x74, 0x61, 0x38, 0xC5, 0xC0,
  0x8C, 0x63, 0xF8, 0xC6, 0x20, 0xE9, 0x24, 0xB8, 0x00, 0x38, 0x84, 0x21,
  0x49, 0x80, 0x8C, 0xA9, 0x8A, 0x4A, 0x20, 0x84, 0x21, 0x08, 0x43, 0xE0,
  0x8E, 0xEB, 0x18, 0xC6, 0x20, 0x8C, 0x73, 0x59, 0xC6, 0x20, 0x74, 0x63,
  0x18, 0xC5, 0xC0, 0xF4, 0x63, 0xE8, 0x42, 0x00, 0x74, 0x63, 0x1A, 0xC9,
  0xA0, 0xF4, 0x63, 0xEA, 0x4A, 0x20, 0x7C, 0x20, 0xE0, 0x87, 0xC0, 0xF9,
  0x08, 0x42, 0x10, 0x80, 0x8C, 0x63, 0x18, 0xC5, 0xC0, 0x8C, 0x63, 0x18,
  0xA8, 0x80, 0x8C, 0x63, 0x1A, 0xEE, 0x20, 0x8C, 0x54, 0x45, 0x46, 0x20,
  0x8C, 0x54, 0x42, 0x10, 0x80, 0xF8, 0x44, 0x44, 0x43, 0xE0, 0xF2, 0x49,
  0x38, 0x00, 0x82, 0x08, 0x20, 0x80, 0xE4, 0x92, 0x78, 0x00, 0x22, 0xA2,
  0x00, 0xF8, 0x88, 0x80, 0x61, 0x79, 0x70, 0x88, 0xE9, 0x99, 0xE0, 0x78,
  0x88, 0x70, 0x11, 0x79, 0x99, 0x70, 0x69, 0xF8, 0x70, 0x25, 0x4E, 0x44,
  0x40, 0x79, 0x71, 0xE0, 0x88, 0xE9, 0x99, 0x90, 0xBE, 0x10, 0x11, 0x19,
  0x60, 0x88, 0x9A, 0xCA, 0x90, 0xFE, 0x00, 0xDD, 0x6B, 0x18, 0x80, 0xE9,
  0x99, 0x90, 0x69, 0x99, 0x60, 0xE9, 0xE8, 0x80, 0x79, 0x71, 0x10, 0xE9,
  0x88, 0x80, 0x78, 0x61, 0xE0, 0x44, 0xE4, 0x45, 0x20, 0x99, 0x99, 0x60,
  0x8C, 0x62, 0xA2, 0x00, 0x8C, 0x6B, 0x55, 0x00, 0x8A, 0x88, 0xA8, 0x80,
  0x99, 0x71, 0xE0, 0xF2, 0x48, 0xF0, 0x29, 0x44, 0x88, 0x00, 0xFE, 0x00,
  0x89, 0x14, 0xA0, 0x00, 0x00, 0x0D, 0xB0, 0x00
};

const GFXglyph Font5x7FixedGlyphs[] PROGMEM = {
  {     0,   0,   1,   3,    0,    0 }   // ' '
 ,{     0,   1,   7,   3,    1,   -7 }   // '!'
 ,{     1,   3,   2,   4,    0,   -7 }   // '"'
 ,{     2,   5,   7,   6,    0,   -7 }   // '#'
 ,{     7,   5,   7,   6,    0,   -7 }   // '$'
 ,{    12,   5,   7,   6,    0,   -7 }   // '%'
 ,{    17,   5,   7,   6,    0,   -7 }   // '&'
 ,{    22,   2,   3,   3,    0,   -7 }   // '''
 ,{    24,   2,   7,   3,    0,   -7 }   // '('
 ,{    27,   2,   7,   3,    0,   -7 }   // ')'
 ,{    30,   5,   7,   6,    0,   -7 }   // '*'
 ,{    35,   5,   5,   6,    0,   -6 }   // '+'
 ,{    39,   2,   2,   3,    0,   -2 }   // ','
 ,{    40,   5,   1,   6,    0,   -4 }   // '-'
 ,{    42,   2,   2,   3,    0,   -2 }   // '.'
 ,{    43,   5,   5,   6,    0,   -6 }   // '/'
 ,{    47,   5,   7,   6,    0,   -7 }   // '0'
 ,{    52,   3,   7,   4,    0,   -7 }   // '1'
 ,{    56,   5,   7,   6,    0,   -7 }   // '2'
 ,{    61,   5,   7,   6,    0,   -7 }   // '3'
 ,{    66,   5,   7,   6,    0,   -7 }   // '4'
 ,{    71,   5,   7,   6,    0,   -7 }   // '5'
 ,{    76,   5,   7,   6,    0,   -7 }   // '6'
 ,{    81,   5,   7,   6,    0,   -7 }   // '7'
 ,{    86,   5,   7,   6,    0,   -7 }   // '8'
 ,{    91,   5,   7,   6,    0,   -7 }   // '9'
 ,{    96,   2,   5,   3,    0,   -6 }   // ':'
 ,{    98,   2,   6,   3,    0,   -6 }   // ';'
 ,{   100,   4,   7,   5,    0,   -7 }   // '<'
 ,{   104,   5,   3,   6,    0,   -5 }   // '='
 ,{   107,   4,   7,   5,    0,   -7 }   // '>'
 ,{   111,   5,   7,   6,    0,   -7 }   // '?'
 ,{   116,   5,   7,   6,    0,   -7 }   // '@'
 ,{   121,   5,   7,   6,    0,   -7 }   // 'A'
 ,{   126,   5,   7,   6,    0,   -7 }   // 'B'
 ,{   131,   5,   7,   6,    0,   -7 }   // 'C'
 ,{   136,   5,   7,   6,    0,   -7 }   // 'D'
 ,{   141,   5,   7,   6,    0,   -7 }   // 'E'
 ,{   146,   5,   7,   6,    0,   -7 }   // 'F'
 ,{   151,   5,   7,   6,    0,   -7 }   // 'G'
 ,{   156,   5,   7,   6,    0,   -7 }   // 'H'
 ,{   161,   3,   7,   6,    1,   -7 }   // 'I'
 ,{   165,   5,   7,   6,    0,   -7 }   // 'J'
 ,{   170,   5,   7,   6,    0,   -7 }   // 'K'
 ,{   175,   5,   7,   6,    0,   -7 }   // 'L'
 ,{   180,   5,   7,   6,    0,   -7 }   // 'M'
 ,{   185,   5,   7,   6,    0,   -7 }   // 'N'
 ,{   190,   5,   7,   6,    0,   -7 }   // 'O'
 ,{   195,   5,   7,   6,    0,   -7 }   // 'P'
 ,{   200,   5,   7,   6,    0,   -7 }   // 'Q'
 ,{   205,   5,   7,   6,    0,   -7 }   // 'R'
 ,{   210,   5,   7,   6,    0,   -7 }   // 'S'
 ,{   215,   5,   7,   6,    0,   -7 }   // 'T'
 ,{   220,   5,   7,   6,    0,   -7 }   // 'U'
 ,{   225,   5,   7,   6,    0,   -7 }   // 'V'
 ,{   230,   5,   7,   6,    0,   -7 }   // 'W'
 ,{   235,   5,   7,   6,    0,   -7 }   // 'X'
 ,{   240,   5,   7,   6,    0,   -7 }   // 'Y'
 ,{   245,   5,   7,   6,    0,   -7 }   // 'Z'
 ,{   250,   3,   7,   4,    0,   -7 }   // '['
 ,{   254,   5,   5,   6,    0,   -6 }   // '\'
 ,{   258,   3,   7,   4,    0,   -7 }   // ']'
 ,{   262,   5,   3,   6,    0,   -7 }   // '^'
 ,{   265,   5,   1,   6,    0,   -1 }   // '_'
 ,{   266,   3,   3,   4,    0,   -7 }   // '`'
 ,{   268,   4,   5,   5,    0,   -5 }   // 'a'
 ,{   271,   4,   7,   5,    0,   -7 }   // 'b'
 ,{   275,   4,   5,   5,    0,   -5 }   // 'c'
 ,{   278,   4,   7,   5,    0,   -7 }   // 'd'
 ,{   282,   4,   5,   5,    0,   -5 }   // 'e'
 ,{   285,   4,   7,   5,    0,   -7 }   // 'f'
 ,{   289,   4,   5,   5,    0,   -5 }   // 'g'
 ,{   292,   4,   7,   5,    0,   -7 }   // 'h'
 ,{   296,   1,   7,   2,    0,   -7 }   // 'i'
 ,{   297,   4,   7,   5,    0,   -7 }   // 'j'
 ,{   301,   4,   7,   5,    0,   -7 }   // 'k'
 ,{   305,   1,   7,   2,    0,   -7 }   // 'l'
 ,{   307,   5,   5,   6,    0,   -5 }   // 'm'
 ,{   311,   4,   5,   5,    0,   -5 }   // 'n'
 ,{   314,   4,   5,   5,    0,   -5 }   // 'o'
 ,{   317,   4,   5,   5,    0,   -5 }   // 'p'
 ,{   320,   4,   5,   5,    0,   -5 }   // 'q'
 ,{   323,   4,   5,   5,    0,   -5 }   // 'r'
 ,{   326,   4,   5,   5,    0,   -5 }   // 's'
 ,{   329,   4,   7,   5,    0,   -7 }   // 't'
 ,{   333,   4,   5,   5,    0,   -5 }   // 'u'
 ,{   336,   5,   5,   6,    0,   -5 }   // 'v'
 ,{   340,   5,   5,   6,    0,   -5 }   // 'w'
 ,{   344,   5,   5,   6,    0,   -5 }   // 'x'
 ,{   348,   4,   5,   5,    0,   -5 }   // 'y'
 ,{   351,   4,   5,   5,    0,   -5 }   // 'z'
 ,{   354,   3,   7,   4,    0,   -7 }   // '{'
 ,{   358,   1,   7,   2,    0,   -7 }   // '|'
 ,{   360,   3,   7,   4,    0,   -7 }   // '}'
 ,{   364,   4,   7,   5,    0,   -7 }   // '~'
};

const GFXfont Font5x7Fixed PROGMEM = {
  (uint8_t  *)Font5x7FixedBitmaps,
  (GFXglyph *)Font5x7FixedGlyphs,
  0x20, 0x7E, 7};