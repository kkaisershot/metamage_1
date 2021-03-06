/*
	traps.cc
	--------
*/

#include "traps.hh"

// Standard C
#include <stdlib.h>

// C99
#include <stdint.h>

// iota
#include "iota/iterator.hh"
#include "iota/strings.hh"

// gear
#include "gear/hexadecimal.hh"

// plus
#include "plus/var_string.hh"

// text-input
#include "text_input/feed.hh"
#include "text_input/get_line_from_feed.hh"

// poseven
#include "poseven/extras/fd_reader.hh"
#include "poseven/functions/open.hh"
#include "poseven/functions/read.hh"


namespace tool
{
	
	namespace p7 = poseven;
	
	typedef uint16_t offset_t;
	
	static offset_t global_name_offsets[ 0x1000 ];  // all possible A-traps
	
	static plus::var_string global_name_data;
	
	
	static void read_traps( p7::fd_t fd )
	{
		global_name_data.reserve( 12 * 1024 );
		
		global_name_data.assign( STR_LEN( "<unknown trap>" ) + 1 );
		
		text_input::feed feed;
		
		p7::fd_reader reader( fd );
		
		while ( const plus::string* s = get_line_bare_from_feed( feed, reader ) )
		{
			const iota::span line( s->data(), s->size() );
			
			if ( line.size() < STRLEN( "A123 _X" ) )
			{
				break;
			}
			
			const uint16_t trap_word = gear::decode_16_bit_hex( &line[0] );
			
			const char* trap_name = &line[ STRLEN( "A123 " ) ];
			
			const char* line_end = &*line.end();
			
			global_name_offsets[ trap_word & 0x0FFF ] = global_name_data.size();
			
			global_name_data.append( trap_name, line_end );
			
			global_name_data += '\0';
		}
	}
	
	static void try_to_read_traps()
	{
		const char* traps_file = getenv( "D68K_TRAPS" );
		
		if ( traps_file == NULL )
		{
			traps_file = "/etc/traps";
		}
		
		try
		{
			read_traps( p7::open( traps_file, p7::o_rdonly ) );
		}
		catch ( ... )
		{
		}
	}
	
	const char* get_trap_name( unsigned short trap_word )
	{
		static bool done = (try_to_read_traps(), true );
		
		const size_t offset = global_name_offsets[ trap_word & 0x0FFF ];
		
		return offset != 0 ? &global_name_data[ offset ] : NULL;
	}
	
}
