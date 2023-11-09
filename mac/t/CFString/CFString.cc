/*
	CFString.cc
	-----------
*/

// Standard C
#include <stdlib.h>
#include <string.h>

// iota
#include "iota/strings.hh"

// Nitrogen
#include "Nitrogen/CFString.hh"

#undef check

// tap-out
#include "tap/check.hh"
#include "tap/test.hh"


namespace n = nucleus;
namespace N = Nitrogen;


static const unsigned n_tests = 4 + 4;


static void UTF8_from_MacRoman()
{
	const char* macRoman = "foo\xA5";
	
	n::owned< CFStringRef > string = N::CFStringCreateWithCString( macRoman,
	                                                               Carbon::kCFStringEncodingMacRoman );
	
	const CFIndex length = CFStringGetLength( string );
	
	EXPECT( length == 4 );
	
	const CFRange range = CFRangeMake( 0, length );
	
	char buffer[ 16 ];
	
	CFIndex n_bytes = 0;
	
	CFIndex result = CFStringGetBytes( string,
	                                   range,
	                                   kCFStringEncodingUTF8,
	                                   UInt8( 0 ),
	                                   false,
	                                   (UInt8*) &buffer[ 0 ],
	                                   sizeof buffer,
	                                   &n_bytes );
	
	EXPECT( result == 4 );
	
	EXPECT( n_bytes == 6 );
	
	EXPECT( memcmp( buffer, STR_LEN( "foo\xE2\x80\xA2" ) ) == 0 );
}

static void MacRoman_from_UTF8()
{
	const char* utf8 = "foo\xE2\x80\xA2";
	
	n::owned< CFStringRef > string = N::CFStringCreateWithCString( utf8,
	                                                               Carbon::kCFStringEncodingUTF8 );
	
	const CFIndex length = CFStringGetLength( string );
	
	EXPECT( length == 4 );
	
	const CFRange range = CFRangeMake( 0, length );
	
	char buffer[ 16 ];
	
	CFIndex n_bytes = 0;
	
	CFIndex result = CFStringGetBytes( string,
	                                   range,
	                                   kCFStringEncodingMacRoman,
	                                   UInt8( 0 ),
	                                   false,
	                                   (UInt8*) &buffer[ 0 ],
	                                   sizeof buffer,
	                                   &n_bytes );
	
	EXPECT( result == 4 );
	
	EXPECT( n_bytes == 4 );
	
	EXPECT( memcmp( buffer, STR_LEN( "foo\xA5" ) ) == 0 );
}

int main( int argc, char** argv )
{
	tap::start( "CFString", n_tests );
	
	UTF8_from_MacRoman();
	
	MacRoman_from_UTF8();
	
	return 0;
}
