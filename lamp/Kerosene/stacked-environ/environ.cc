/*
	environ.cc
	----------
*/

// Standard C
#include "errno.h"
#include "stdlib.h"

// Kerosene
#include "environ_store.hh"

// getenv
static char* _getenv( const char* name );
#define getenv _getenv
#include "getenv.cc.hh"
#undef getenv


using namespace _relix_libc;

static bool loaded_environ;

static environ_store get_envp()
{
	loaded_environ = load_environ();
	
	return environ_store();
}


char* getenv( const char* name )
{
	return loaded_environ ? environ_store::get( name )
	                      : _getenv( name );
}

int setenv( const char* name, const char* value, int overwriting )
{
	try
	{
		get_envp().set( name, value, overwriting );
	}
	catch ( ... )
	{
		errno = ENOMEM;
		
		return -1;
	}
	
	return 0;
}

int putenv( char* string )
{
	try
	{
		get_envp().put( string );
	}
	catch ( ... )
	{
		errno = ENOMEM;
		
		return -1;
	}
	
	return 0;
}

int unsetenv( const char* name )
{
	try
	{
		get_envp().unset( name );
	}
	catch ( ... )
	{
		errno = ENOMEM;
		
		return -1;
	}
	
	return 0;
}

int clearenv()
{
	try
	{
		get_envp().clear();
	}
	catch ( ... )
	{
		errno = ENOMEM;
		
		return -1;
	}
	
	return 0;
}
