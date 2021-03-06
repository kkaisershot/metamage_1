/*
	keytrans.cc
	-----------
*/

#include "keytrans.hh"


#define ____ 0x00

const unsigned char keytrans_shift[] =
{
	____,  // NUL
	____,  // Home
	____,
	____,  // Enter
	____,  // End
	____,  // Help
	____,
	____,
	____,  // Backspace / Delete
	____,  // Tab
	____,
	____,  // Page Up
	____,  // Page Down
	____,  // Return
	____,
	____,
	
	____,  // Function keys
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,  // Escape
	____,  // Left
	____,  // Right
	____,  // Up
	____,  // Down
	
	____,  // Space
	____,  // '!'
	____,  // '"'
	____,  // '#'
	____,  // '$'
	____,  // '%'
	____,  // '&'
	'"',   // '\''
	____,  // '('
	____,  // ')'
	____,  // '*'
	____,  // '+'
	'<',   // ','
	'_',   // '-'
	'>',   // '.'
	'?',   // '/'
	
	')',   // '0'
	'!',   // '1'
	'@',   // '2'
	'#',   // '3'
	'$',   // '4'
	'%',   // '5'
	'^',   // '6'
	'&',   // '7'
	'*',   // '8'
	'(',   // '9'
	____,  // ':'
	':',   // ';'
	____,  // '<'
	'+',   // '='
	____,  // '>'
	____,  // '?'
	
	____,  // '@'
	____,  // 'A'
	____,  // 'B'
	____,  // 'C'
	____,  // 'D'
	____,  // 'E'
	____,  // 'F'
	____,  // 'G'
	____,  // 'H'
	____,  // 'I'
	____,  // 'J'
	____,  // 'K'
	____,  // 'L'
	____,  // 'M'
	____,  // 'N'
	____,  // 'O'
	
	____,  // 'P'
	____,  // 'Q'
	____,  // 'R'
	____,  // 'S'
	____,  // 'T'
	____,  // 'U'
	____,  // 'V'
	____,  // 'W'
	____,  // 'X'
	____,  // 'Y'
	____,  // 'Z'
	'{',   // '['
	'|',   // '\\'
	'}',   // ']'
	____,  // '^'
	____,  // '_'
	
	'~',   // '`'
	'A',   // 'a'
	'B',   // 'b'
	'C',   // 'c'
	'D',   // 'd'
	'E',   // 'e'
	'F',   // 'f'
	'G',   // 'g'
	'H',   // 'h'
	'I',   // 'i'
	'J',   // 'j'
	'K',   // 'k'
	'L',   // 'l'
	'M',   // 'm'
	'N',   // 'n'
	'O',   // 'o'
	
	'P',   // 'p'
	'Q',   // 'q'
	'R',   // 'r'
	'S',   // 's'
	'T',   // 't'
	'U',   // 'u'
	'V',   // 'v'
	'W',   // 'w'
	'X',   // 'x'
	'Y',   // 'y'
	'Z',   // 'z'
	____,  // '{'
	____,  // '|'
	____,  // '}'
	____,  // '~'
	____,  // FwdDel
};

const unsigned char keytrans_option[] =
{
	____,  // NUL
	____,  // Home
	____,
	____,  // Enter
	____,  // End
	____,  // Help
	____,
	____,
	____,  // Backspace / Delete
	____,  // Tab
	____,
	____,  // Page Up
	____,  // Page Down
	____,  // Return
	____,
	____,
	
	____,  // Function keys
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,
	____,  // Escape
	____,  // Left
	____,  // Right
	____,  // Up
	____,  // Down
	
	0xCA,  // Space
	0xDA,  // '!'
	0xAE,  // '"'
	0xDC,  // '#'
	0xDD,  // '$'
	0xDE,  // '%'
	0xE0,  // '&'
	0xBE,  // '\''
	0xE1,  // '('
	0xE2,  // ')'
	0xA1,  // '*'
	0xB1,  // '+'
	0xB2,  // ','
	0xD0,  // '-'
	0xB3,  // '.'
	0xD6,  // '/'
	
	0xBC,  // '0'
	0xC1,  // '1'
	0xAA,  // '2'
	0xA3,  // '3'
	0xA2,  // '4'
	0xB0,  // '5'
	0xA4,  // '6'
	0xA6,  // '7'
	0xA5,  // '8'
	0xBB,  // '9'
	0xF2,  // ':'
	0xC9,  // ';'
	0xF8,  // '<'
	0xAD,  // '='
	0xF9,  // '>'
	0xC0,  // '?'
	
	0xDB,  // '@'
	0x81,  // 'A'
	0xF5,  // 'B'
	0x82,  // 'C'
	0xEB,  // 'D'
	0xAB,  // 'E'
	0xEC,  // 'F'
	0xFD,  // 'G'
	0xEE,  // 'H'
	0xF6,  // 'I'
	0xEF,  // 'J'
	0xF0,  // 'K'
	0xF1,  // 'L'
	0xE5,  // 'M'
	0xF7,  // 'N'
	0xAF,  // 'O'
	
	0xB8,  // 'P'
	0xCE,  // 'Q'
	0xE4,  // 'R'
	0xEA,  // 'S'
	0xFF,  // 'T'
	0xAC,  // 'U'
	0xD7,  // 'V'
	0xE3,  // 'W'
	0xFE,  // 'X'
	0xE7,  // 'Y'
	0xFC,  // 'Z'
	0xD2,  // '['
	0xC7,  // '\\'
	0xD4,  // ']'
	0xDF,  // '^'
	0xD1,  // '_'
	
	____,  // '`'
	0x8C,  // 'a'
	0xBA,  // 'b'
	0x8D,  // 'c'
	0xB6,  // 'd'
	0xAB,  // 'e'
	0xC4,  // 'f'
	0xA9,  // 'g'
	0xFA,  // 'h'
	0xF6,  // 'i'
	0xC6,  // 'j'
	0xFB,  // 'k'
	0xC2,  // 'l'
	0xB5,  // 'm'
	0x96,  // 'n'
	0xBF,  // 'o'
	
	0xB9,  // 'p'
	0xCF,  // 'q'
	0xA8,  // 'r'
	0xA7,  // 's'
	0xA0,  // 't'
	0xAC,  // 'u'
	0xC3,  // 'v'
	0xB7,  // 'w'
	0xC5,  // 'x'
	0x5C,  // 'y'
	0xBD,  // 'z'
	0xD3,  // '{'
	0xC8,  // '|'
	0xD5,  // '}'
	0x60,  // '~'
	____,  // FwdDel
};
