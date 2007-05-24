/*	==============
 *	GetPathname.cc
 *	==============
 */

#ifndef GETPATHNAME_HH
#include "GetPathname.hh"
#endif

// Nitrogen
#ifndef NITROGEN_FOLDERS_H
#include "Nitrogen/Folders.h"
#endif


static std::string GetMacPathname_Internal( const FSSpec& file )
{
	std::string filename = Nucleus::Convert< std::string >( file.name );
	
	if ( file.parID == fsRtParID )
	{
		return filename;
	}
	
	return GetMacPathname( io::get_preceding_directory( file ) ) + filename;
}

std::string GetMacPathname( const Nitrogen::FSDirSpec& dir )
{
	return GetMacPathname_Internal( Nucleus::Convert< FSSpec >( dir ) ) + ":";
}

std::string GetMacPathname( const FSSpec& file )
{
	bool needsTrailingColon = file.parID == fsRtParID;
	
	return GetMacPathname_Internal( file ) + ( needsTrailingColon ? ":" : "" );
}

static std::string GetPOSIXPathname_Internal( const FSSpec& file )
{
	std::string filename = io::get_filename_string( file );
	
	std::replace( filename.begin(),
	              filename.end(),
	              '/',
	              ':' );
	
	if ( file.parID == fsRtParID )
	{
		if ( file.vRefNum == io::system_root< Nitrogen::FSVolumeRefNum >() )
		{
			return "";
		}
		else
		{
			return std::string( "/Volumes/" ) + filename;
		}
	}
	
	return GetPOSIXPathname( io::get_preceding_directory( file ) ) + filename;
}

std::string GetPOSIXPathname( const Nitrogen::FSDirSpec& dir )
{
	return GetPOSIXPathname_Internal( Nucleus::Convert< FSSpec >( dir ) ) + "/";
}

std::string GetPOSIXPathname( const FSSpec& file )
{
	bool needsTrailingSlash = file.parID == fsRtParID;
	
	return GetPOSIXPathname_Internal( file ) + ( needsTrailingSlash ? "/" : "" );
}

