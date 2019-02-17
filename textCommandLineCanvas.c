#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Simple textCommandLineCanvas  
 * 
 * diegoCode (Diego Codevilla) / ARG / 2019
 *
 * CC-BY-SA 4
 * 
 * This program allow to:
 *
 *   - Show 5x7 characters
 *   - draw boxes, horizontal and vertical lines 
 *   - draw with four orientations (in deg): 0, 90, 180, 270 (CW)
 *   - align text: center, left, right
 * 
 * 5x7 characters generated using 
 *      https://omerk.github.io/lcdchargen/
 * 
 */

// buffer width & height
#define WIDTH   80		
#define HEIGHT  80

// first ascii character implemented in the character map
#define BASE_CHARMAP  32  // first ascii defined - 32 = ascii of space

// width of the characters (number of buffer positions) 
#define CHAR_WIDTH	5
#define CHAR_CHARACTERS	'#' 

// constants used for alignment
#define CENTER	-1
#define	LEFT	-2
#define	RIGHT	-3

// constants used for orientation
#define	ORIENTATION_0	0
#define	PORTRAIT		0
#define	ORIENTATION_90	1
#define	LANDSCAPE		1
#define	ORIENTATION_180	3
#define	ORIENTATION_270	2

void clearCanvas(char canvas[HEIGHT][WIDTH]);
void drawCanvas(char canvas[HEIGHT][WIDTH], char orientation);
void addChar(char canvas[HEIGHT][WIDTH], char character, int posx, int posy); 
void addWords(char canvas[HEIGHT][WIDTH], char* str, int posx, int posy);
void addBox(char canvas[HEIGHT][WIDTH], int posx0, int posy0, int posx1, int posy1, char* borders);
void addLine(char canvas[HEIGHT][WIDTH], int posx0, int posy0, int posx1, int posy1, char character);
void addLineV(char canvas[HEIGHT][WIDTH], int posx0, int posy0, int posy1, char character);
void addLineH(char canvas[HEIGHT][WIDTH], int posx0, int posy0, int posx1, char character);

// buffer definition 
char banner[HEIGHT][WIDTH];

/*
 * 5x7 character table
 */
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
    {0b00100,0b01010,0b10001,0b10001,0b11111,0b10001,0b10001,0b00000}, // A   
    {0b11110,0b10001,0b10001,0b11110,0b10001,0b10001,0b11110,0b00000}, // B   
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
    
    clearCanvas(banner);
         
    addWords(banner, "ABCDEFGHIJKL", 4, 5);
    addWords(banner, "MNOPQRSTUVWX", 4, 14);
    addWords(banner, "YZ1234567890", 4, 23);
    addWords(banner, "!\"#$%&'()*+,", 4, 32);
    addWords(banner, "-./:;<=>?@[\\", 4, 41); 
    addWords(banner, "]^_`", 4, 50);
    addWords(banner, "a b", CENTER, 59);
        
    addBox(banner, 1, 1, WIDTH - 2, HEIGHT - 2,  "-\\|/-\\|/");
    addBox(banner, 0, 0, WIDTH - 1, HEIGHT - 1, "********");
       
    drawCanvas(banner, ORIENTATION_0);
    drawCanvas(banner, ORIENTATION_90);
       
    return 0;
}

/*
 * Fill buffer with spaces 
 */
void clearCanvas(char banner[HEIGHT][WIDTH]) {
    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            banner[y][x] = ' ';
        }
    }
}

/*
 * Prints buffer content on screen with the given orientation 
 * (ORIENTATION_0, ORIENTATION_90, ORIENTATION_180, ORIENTATION_270)
 */
void drawCanvas(char banner[HEIGHT][WIDTH], char orientation){	
    switch (orientation) {
		default:
		case ORIENTATION_0:	
	    for(int y = 0; y < HEIGHT; y++) {
			for(int x = 0; x < WIDTH; x++) {
				putchar(banner[y][x]);
			}
			putchar('\n');
		}		
		break;
		case ORIENTATION_90:
		for(int x = 0; x < HEIGHT; x++) {
			for(int y = WIDTH - 1; y >= 0; y--) {			
				putchar(banner[y][x]);
			}
			putchar('\n');
		}
		break;
		case ORIENTATION_270:
		for(int x = HEIGHT - 1; x >= 0; x--) {
			for(int y = 0; y < WIDTH; y++) {			
				putchar(banner[y][x]);
			}
			putchar('\n');
		}
		break;
		case ORIENTATION_180:
	    for(int y = HEIGHT - 1; y >= 0; y--) {
			for(int x = WIDTH - 1; x >= 0; x--) {
				putchar(banner[y][x]);
			}
			putchar('\n');
		}		
		break;
	}    
}

/*
 * Add an horizontal line to the buffer 
 * 
 * posx0, posy0: start point
 * posx1: end of line 
 * character: character used to represent the line
 * 
 */
void addLineH(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posx1, char character){       
    for(int p = posx0; p < posx1; p++) {
        banner[posy0][p] = character;
    }
}

/*
 * Add a vertical line to the buffer 
 * 
 * posx0, posy0: start point
 * posy1: end of line 
 * character: character used to represent the line
 * 
 */
void addLineV(char banner[HEIGHT][WIDTH], int posx0, int posy0, int posy1, char character){       
    for(int p = posy0; p < posy1; p++) {
        banner[p][posx0] = character;
    }
}

/*
 * Add a box (rectangle) to the buffer
 * 
 * posx0, posy0: upper-left coordinates 
 * posx1, posy1: lower-right coordinates
 *  
 * borders: an 8-char string that specifies the characters used 
 *    		to draw the box. These characters' order is cockwise
 * 			from the upper border (Ej. if borders is "-1|2-3|4" 
 * 			the box will be:
 * 
 *                      4-------1
 * 			|	|
 * 			|	|
 * 			3-------2
 * 
 */
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

/*
 * Adds the given string to the buffer in the position specified 
 * by posx, posy
 */
void addWords(char banner[HEIGHT][WIDTH], char* str, int posx, int posy) {
    int x_offs = 0; 
    switch (posx) {
		case CENTER:
			posx = (WIDTH - (strlen(str) * CHAR_WIDTH)) / 2 - 1;
			break;
			
		case LEFT:
			posx = 0;
			break;
			
		case RIGHT:
			posx = WIDTH - (strlen(str) * CHAR_WIDTH) - 2;
			break;
	}
	
    while(*str) {
        addChar(banner, *str, posx + x_offs, posy);
        str++;
        x_offs+=6;
    }
}

/*
 * Adds the given character to the buffer in the position 
 * specified by posx, posy 
 */
void addChar(char banner[HEIGHT][WIDTH], char character, int posx, int posy) {
    for(int i = 0; i < 8; i++) {
        for(int p = 0; p < 5; p++) {
            banner[i + posy][p + posx] = charmap[character - BASE_CHARMAP][i] & (1 << (4 - p)) ? CHAR_CHARACTERS : ' ';
        }
    }
}
