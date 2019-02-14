#include <stdio.h>
#include <stdlib.h>

/*
 * Simple textCommandLineCanvas  
 * 
 * diegoCode (Diego Codevilla) / ARG / 2019
 * 
 * This program allow to:
 *
 *   - Show 5x7 characters
 *   - draw boxes, horizontal and vertical lines 
 * 
 * 
 * 5x7 characters generated using 
 *      https://omerk.github.io/lcdchargen/
 * 
 */

#define WIDTH   120
#define HEIGHT  80

#define BASE_CHARMAP  32  // ascii of space

void addChar(char banner[HEIGHT][WIDTH], char character, int posx, int posy); 
void addWords(char banner[HEIGHT][WIDTH], char* str, int posx, int posy);
void drawBanner(char banner[HEIGHT][WIDTH]);
void addBox(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posx1, int posy1, char* borders);
void addLine(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posx1, int posy1, char character);
void addLineV(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posy1, char character);
void addLineH(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posx1, char character);

char banner[HEIGHT][WIDTH];

char charmap[127][8] = {
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}, // space    
    {0b00100,0b00100,0b00100,0b00100,0b00100,0b00000,0b00100,0b00000}, // !
    {0b01010,0b01010,0b01010,0b00000,0b00000,0b00000,0b00000,0b00000}, // "  
    {0b01010,0b01010,0b11111,0b01010,0b11111,0b01010,0b01010,0b00000}, // # 
    {0b00100,0b01111,0b10100,0b01110,0b00101,0b11110,0b00100,0b00000}, // $ 
    {0b11000,0b11001,0b00010,0b00100,0b01000,0b10011,0b00011,0b00000}, // %
    {0b01100,0b10010,0b10100,0b01000,0b10101,0b10010,0b01101,0b00000}, // &
    {0b00100,0b00100,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}, // '
    {0b00100,0b01000,0b10000,0b10000,0b10000,0b01000,0b00100,0b00000}, // (
    {0b00100,0b00010,0b00001,0b00001,0b00001,0b00010,0b00100,0b00000}, // ) 
    {0b00000,0b00100,0b10101,0b01110,0b10101,0b00100,0b00000,0b00000}, // *
    {0b00000,0b00100,0b00100,0b11111,0b00100,0b00100,0b00000,0b00000}, // +
    {0b00000,0b00000,0b00000,0b00000,0b00110,0b00010,0b00100,0b00000}, // ,
    {0b00000,0b00000,0b00000,0b11111,0b00000,0b00000,0b00000,0b00000}, // -
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b01100,0b01100,0b00000}, // .
    {0b00000,0b00001,0b00010,0b00100,0b01000,0b10000,0b00000,0b00000}, // /
    {0b01110,0b10001,0b10011,0b10101,0b11001,0b10001,0b01110,0b00000}, // 0
    {0b00100,0b01100,0b00100,0b00100,0b00100,0b00100,0b01110,0b00000}, // 1
    {0b01110,0b10001,0b00001,0b00110,0b01000,0b10000,0b11111,0b00000}, // 2
    {0b01110,0b10001,0b00001,0b01110,0b00001,0b10001,0b01110,0b00000}, // 3
    {0b00010,0b00110,0b01010,0b10010,0b11111,0b00010,0b00010,0b00000}, // 4
    {0b11111,0b10000,0b11110,0b00001,0b00001,0b10001,0b01110,0b00000}, // 5
    {0b00110,0b01000,0b10000,0b11110,0b10001,0b10001,0b01110,0b00000}, // 6
    {0b11111,0b00001,0b00010,0b00100,0b01000,0b01000,0b01000,0b00000}, // 7
    {0b01110,0b10001,0b10001,0b01110,0b10001,0b10001,0b01110,0b00000}, // 8
    {0b01110,0b10001,0b10001,0b01111,0b00001,0b00010,0b01100,0b00000}, // 9
    {0b00000,0b01100,0b01100,0b00000,0b01100,0b01100,0b00000,0b00000}, // :
    {0b00000,0b01100,0b01100,0b00000,0b01100,0b00100,0b01000,0b00000}, // ;
    {0b00010,0b00100,0b01000,0b10000,0b01000,0b00100,0b00010,0b00000}, // <
    {0b00000,0b00000,0b11111,0b00000,0b11111,0b00000,0b00000,0b00000}, // =
    {0b01000,0b00100,0b00010,0b00001,0b00010,0b00100,0b01000,0b00000}, // >
    {0b01110,0b10001,0b00001,0b00010,0b00100,0b00000,0b00100,0b00000}, // ?
    {0b01110,0b10001,0b00001,0b01101,0b10101,0b10101,0b01110,0b00000}, // @    
    {0x04, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x00}, // A   
    {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E, 0x00}, // B   
    {0b01110,0b10000,0b10000,0b10000,0b10000,0b10000,0b01110,0b00000}, // C   
    {0b11110,0b10001,0b10001,0b10001,0b10001,0b10001,0b11110,0b00000}, // D   
    {0b11111,0b10000,0b10000,0b11110,0b10000,0b10000,0b11111,0b00000}, // E
    {0b11111,0b10000,0b10000,0b11110,0b10000,0b10000,0b10000,0b00000},  // F
    {0b01110,0b10001,0b10000,0b10011,0b10001,0b10001,0b01111,0b00000}, // G
    {0b10001,0b10001,0b10001,0b11111,0b10001,0b10001,0b10001,0b00000},  // H
    {0b01110,0b00100,0b00100,0b00100,0b00100,0b00100,0b01110,0b00000},  // I
    {0b00111,0b00010,0b00010,0b00010,0b00010,0b10010,0b01100,0b00000}, // J
    {0b10001,0b10010,0b10100,0b11000,0b10100,0b10010,0b10001,0b00000}, // K
    {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b11111,0b00000},  // L
    {0b10001,0b11011,0b10101,0b10101,0b10001,0b10001,0b10001,0b00000}, //M
    {0b10001,0b11001,0b10101,0b10011,0b10001,0b10001,0b10001,0b00000}, // N
    {0b01110,0b10001,0b10001,0b10001,0b10001,0b10001,0b01110,0b00000}, // O
    {0b11110,0b10001,0b10001,0b11110,0b10000,0b10000,0b10000,0b00000}, // P
    {0b01110,0b10001,0b10001,0b10001,0b10101,0b10010,0b01101,0b00000}, // Q
    {0b11110,0b10001,0b10001,0b11110,0b10100,0b10010,0b10001,0b00000}, // R
    {0b01110,0b10001,0b10000,0b01110,0b00001,0b10001,0b01110,0b00000}, // S
    {0b11111,0b00100,0b00100,0b00100,0b00100,0b00100,0b00100,0b00000}, // T
    {0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b01110,0b00000}, // U
    {0b10001,0b10001,0b10001,0b10001,0b10001,0b01010,0b00100,0b00000}, // V
    {0b10001,0b10001,0b10001,0b10101,0b10101,0b10101,0b01010,0b00000}, // W
    {0b10001,0b10001,0b01010,0b00100,0b01010,0b10001,0b10001,0b00000}, // X
    {0b10001,0b10001,0b10001,0b01010,0b00100,0b00100,0b00100,0b00000},  // Y
    {0b11111,0b00001,0b00010,0b00100,0b01000,0b10000,0b11111,0b00000}, // Z
    {0b01110,0b01000,0b01000,0b01000,0b01000,0b01000,0b01110,0b00000}, // [ 
    {0b00000,0b10000,0b01000,0b00100,0b00010,0b00001,0b00000,0b00000}, // '\'
    {0b01110,0b00010,0b00010,0b00010,0b00010,0b00010,0b01110,0b00000}, // ]
    {0b00100,0b01010,0b10001,0b00000,0b00000,0b00000,0b00000,0b00000}, // ^
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b00000}, // _
    {0b01000,0b00100,0b00010,0b00000,0b00000,0b00000,0b00000,0b00000}, //`
    {0b00000,0b00000,0b01010,0b10101,0b10001,0b01010,0b00100,0b00000}, // heart  (97)
    {0b00000,0b00000,0b01010,0b00000,0b10001,0b10001,0b01110,0b00000}, // smile (98)
};

int main() {

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            banner[y][x] = ' ';
        }
    }
         
    addWords(banner, "ABCDEFGHIJKLM", 4, 5);
    addWords(banner, "NOPQRSTUVWXYZ", 4, 14);
    addWords(banner, "!\"#$%&'()*+,-", 4, 23);
    addWords(banner, "./0123456789:", 4, 32);
    addWords(banner, ";<=>?@[\\]^_`", 4, 41);
    addWords(banner, "a b cd", 4, 50);
    
    addBox(banner, 1, 1, 99, 62,  "-\\|/-\\|/");
    addBox(banner, 0, 0, 100, 63, "********");
    
    addLineH(banner, 2, 13, 97, '_');
    addLineV(banner, 90, 3, 62, '|');
    addLineV(banner, 92, 3, 62, '^');
    
    drawBanner(banner);
    
    return 0;
}

void drawBanner(char banner[HEIGHT][WIDTH]){
    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            putchar(banner[y][x]);
        }
        putchar('\n');
    }
}

void addLineH(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posx1, char character){       
    for(int p = posx0; p < posx1; p++) {
        banner[posy0][p] = character;
    }
}

void addLineV(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posy1, char character){       
    for(int p = posy0; p < posy1; p++) {
        banner[p][posx0] = character;
    }
}

void addBox(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posx1, int posy1, char* borders) {
    banner[posy0][posx0] = *(borders + 7);
    banner[posy0][posx1] = *(borders + 1);
    banner[posy1][posx0] = *(borders + 5);
    banner[posy1][posx1] = *(borders + 3);
    
    for(int p = posx0 + 1; p < posx1; p++) {
            banner[posy0][p] = *borders;
            banner[posy1][p] = *(borders + 4);
    }
 
    for (int i = posy0 + 1; i < posy1; i++) {
        banner[i][posx0] = *(borders + 2);
        banner[i][posx1] = *(borders + 6);
    }
    
}

void addWords(char banner[HEIGHT][WIDTH], char* str, int posx, int posy) {
    int x_offs = 0; 
    while(*str) {
        addChar(banner, *str, posx + x_offs, posy);
        str++;
        x_offs+=6;
    }
}

void addChar(char banner[HEIGHT][WIDTH], char character, int posx, int posy) {
    for(int i = 0; i < 8; i++) {
        for(int p = 0; p < 5; p++) {
            banner[i + posy][p + posx] = charmap[character - BASE_CHARMAP][i] & (1 << (4 - p)) ? '*' : ' ';
        }
    }
}
