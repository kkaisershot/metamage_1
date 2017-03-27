/*
	system-info.cc
	--------------
*/

// Mac OS X
#ifdef __APPLE__
#include <CoreServices/CoreServices.h>
#endif

// Mac OS
#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

// Standard C
#include <stdio.h>

// gear
#include "gear/inscribe_decimal.hh"

// mac-sys-utils
#include "mac_sys/gestalt.hh"
#include "mac_sys/get_machine_name.hh"
#include "mac_sys/unit_table.hh"


#define PROGRAM  "system-info"

#define STR_LEN( s )  "" s, (sizeof s - 1)


using mac::sys::gestalt;
using mac::sys::gestalt_defined;


#define M68K  "Motorola 68K"
#define PPC   "PowerPC"
#define X86   "x86"
#define WHAT  "???"

#define MOD_TYPE  "Execution module type:  "
#define COMPILED  "Compiled architecture:  "

static
void compiled()
{
	const char* type = TARGET_RT_MAC_MACHO ? MOD_TYPE "Mach-O"
	                 : TARGET_RT_MAC_CFM   ? MOD_TYPE "CFM"
	                 :                       MOD_TYPE "Code resource";
	
	printf( "%s\n", type );
	
	const char* arch = TARGET_CPU_68K ? COMPILED M68K
	                 : TARGET_CPU_PPC ? COMPILED PPC
	                 : TARGET_CPU_X86 ? COMPILED X86
	                 :                  COMPILED WHAT;
	
	printf( "%s\n", arch );
}

static
void host_env()
{
	const uint32_t sysa = gestalt( 'sysa' );
	const uint32_t sysv = gestalt( 'sysv' );
	
	const char* arch_name = sysa <=  1 ? M68K
	                      : sysa ==  2 ? PPC
	                      : sysa == 10 ? X86
	                      :              WHAT;
	
	char machine_name[ 256 ] = { 0 };
	
	const unsigned char* mnam = mac::sys::get_machine_name();
	
	if ( mnam != NULL )
	{
		memcpy( machine_name, mnam + 1, mnam[ 0 ] );
	}
	
	const char* os_name = sysv < 0x0800 ? "Macintosh System"
	                    : sysv < 0x1000 ? "Mac OS"
	                    :                 "Mac OS X";
	
	uint8_t sys1 = gestalt( 'sys1' );
	uint8_t sys2;
	uint8_t sys3;
	
	if ( sys1 )
	{
		sys2 = gestalt( 'sys2' );
		sys3 = gestalt( 'sys3' );
	}
	else
	{
		sys1 = (sysv >> 8) & 0xF;
		sys2 = (sysv >> 4) & 0xF;
		sys3 = (sysv >> 0) & 0xF;
	}
	
	char a[ 4 ] = { 0 };
	char b[ 4 ] = { 0 };
	char c[ 4 ] = { 0 };
	
	gear::inscribe_unsigned_decimal_r( sys1, a );
	gear::inscribe_unsigned_decimal_r( sys2, b );
	gear::inscribe_unsigned_decimal_r( sys3, c );
	
	printf( "Host CPU architecture:  %s\n", arch_name );
	
	if ( mnam != NULL )
	{
		printf( "Host CPU machine name:  %s\n", machine_name );
	}
	
	printf( "Host operating system:  %s %s.%s.%s\n", os_name, a, b, c );
	
	if ( !! TARGET_RT_MAC_CFM  &&  TARGET_API_MAC_CARBON )
	{
		const uint32_t cbon = gestalt( 'cbon' );
		
		const char a = 0x30 + ((cbon >> 8) & 0xF);
		const char b = 0x30 + ((cbon >> 4) & 0xF);
		const char c = 0x30 + ((cbon >> 0) & 0xF);
		
		printf( "CarbonLib version:      %c.%c.%c\n", a, b, c );
	}
}

static
bool in_SheepShaver()
{
	using mac::types::AuxDCE;
	
	if ( TARGET_RT_MAC_MACHO )
	{
		return false;
	}
	
	const short n = mac::sys::get_unit_table_entry_count();
	
	AuxDCE*** const begin = mac::sys::get_unit_table_base();
	AuxDCE*** const end   = begin + n;
	
	for ( AuxDCE*** it = begin;  it < end;  ++it )
	{
		const unsigned char* name = mac::sys::get_driver_name( *it );
		
		if ( name[ 0 ] == 0 )
		{
			continue;
		}
		
		const bool undotted = name[ 1 ] != '.';
		
		const int cmp = memcmp( &".Display_Video_Apple_Sheep"[ undotted ],
		                        name + 1,
		                        name[ 0 ] );
		
		if ( cmp == 0 )
		{
			return true;
		}
		
		++it;
	}
	
	return false;
}

static
void virt_env()
{
	if ( gestalt( 'a/ux' ) )
	{
		printf( "Paravirtualization:     A/UX\n" );
	}
	
	const bool bbox = gestalt( 'bbox' );
	
	if ( bbox )
	{
		printf( "Paravirtualization:     Blue Box\n" );
	}
	
	if ( TARGET_CPU_68K  &&  gestalt( 'sysa' ) == 2 )
	{
		printf( "68K emulation:          Apple\n" );
	}
	
	if ( TARGET_CPU_68K  &&  gestalt_defined( 'v68k' ) )
	{
		printf( "68K emulation:          v68k\n" );
	}
	else if ( in_SheepShaver() )
	{
		printf( "PPC emulation:          SheepShaver\n" );
	}
	
	if ( !! TARGET_CPU_PPC  &&  TARGET_API_MAC_CARBON  &&  ! bbox )
	{
		const unsigned char* name = mac::sys::get_machine_name();
		
		const bool powerpc = name != NULL  &&  name[ 1 ] == 'P';
		
		if ( ! powerpc )
		{
			printf( "PPC emulation:          Rosetta\n" );
		}
	}
}

int main( int argc, char** argv )
{
	compiled();
	host_env();
	virt_env();
	
	return 0;
}