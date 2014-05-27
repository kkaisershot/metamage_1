/*
	gear/hexidecimal.hh
	-------------------
*/

#ifndef GEAR_HEXIDECIMAL_HH
#define GEAR_HEXIDECIMAL_HH


namespace gear
{
	
	// (hex-digit & 0x1f) -> numeric nibble
	extern unsigned char decoded_hex_table[];
	
	// (nibble & 0x0f) -> ASCII hex digit
	extern char encoded_hex_table[];
	extern char encoded_HEX_table[];
	
	
	inline unsigned char decoded_hex_digit( char c )
	{
		return decoded_hex_table[ c & 0x1f ];
	}
	
	inline char encoded_hex_char( unsigned char d )
	{
		return encoded_hex_table[ d & 0x0f ];
	}
	
	inline char encoded_HEX_char( unsigned char d )
	{
		return encoded_HEX_table[ d & 0x0f ];
	}
	
	
	unsigned char decode_8_bit_hex( const char* s );
	
	unsigned short decode_16_bit_hex( const char* s );
	
	unsigned decode_32_bit_hex( const char* s );
	
	void encode_8_bit_hex( unsigned char x, char* s );
	void encode_8_bit_HEX( unsigned char x, char* s );
	
	void encode_16_bit_hex( unsigned short x, char* s );
	void encode_16_bit_HEX( unsigned short x, char* s );
	
	void encode_32_bit_hex( unsigned x, char* s );
	void encode_32_bit_HEX( unsigned x, char* s );
	
	
	inline unsigned short hexidecimal_magnitude( unsigned x )
	{
		const unsigned short result = x >= 0x10000000 ? 8
		                            : x >= 0x01000000 ? 7
		                            : x >= 0x00100000 ? 6
		                            : x >= 0x00010000 ? 5
		                            : x >= 0x00001000 ? 4
		                            : x >= 0x00000100 ? 3
		                            : x >= 0x00000010 ? 2
		                            :                   1;
		
		return result;
	}
	
	void inscribe_n_hex_digits( char* p, unsigned long x, unsigned short n );
	void inscribe_n_HEX_digits( char* p, unsigned long x, unsigned short n );
	
}

#endif

