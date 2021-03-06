// setsid.hh
// ---------
//
// Maintained by Joshua Juran

// Part of the Nitrogen project.
//
// Written 2009 by Joshua Juran.
//
// This code was written entirely by the above contributor, who places it
// in the public domain.


#ifndef POSEVEN_FUNCTIONS_SETSID_HH
#define POSEVEN_FUNCTIONS_SETSID_HH

// poseven
#ifndef POSEVEN_TYPES_PID_T_HH
#include "poseven/types/pid_t.hh"
#endif


namespace poseven
{
	
	pid_t setsid();
	
}

#endif
