/*	==========
 *	AsyncIO.cc
 *	==========
 */

#include "Genie/Utilities/AsyncIO.hh"

// MoreFunctional
#include "PointerToFunction.hh"

// Nucleus
#include "Nucleus/NAssert.h"

// Nitrogen
#include "Nitrogen/Gestalt.h"


#define CALLBACK  more::ptr_fun( AsyncYield )


namespace Nitrogen
{
	
	static const Gestalt_Selector gestaltMacOSCompatibilityBoxAttr = Gestalt_Selector( ::gestaltMacOSCompatibilityBoxAttr );
	
	template <> struct GestaltDefault< gestaltMacOSCompatibilityBoxAttr > : GestaltAttrDefaults {};
	
}

namespace Genie
{
	
	namespace N = Nitrogen;
	
	
	static bool RunningInClassic()
	{
		static bool inClassic = N::Gestalt_Bit< N::gestaltMacOSCompatibilityBoxAttr, gestaltMacOSCompatibilityBoxPresent >();
		
		return inClassic;
	}
	
	
	// Save our PSN so we can wake up at interrupt time.
	static N::ProcessSerialNumber gPSN = N::GetCurrentProcess();
	
	
#if TARGET_CPU_68K && !TARGET_RT_MAC_CFM
	
	static pascal void WakeUp()
	
#else
	
	static pascal void WakeUp( ParamBlockRec* )
	
#endif
	{
		::WakeUpProcess( &gPSN );
	}
	
	static IOCompletionUPP gWakeUp = ::NewIOCompletionUPP( WakeUp );
	
	// Async read, throws eofErr
	SInt32 FSRead( N::FSFileRefNum  file,
	               SInt32           requestCount,
	               void *           buffer,
	               Async            async,
	               ThrowEOF_OnZero  policy )
	{
		return N::FSRead( file,
		                  requestCount,
		                  buffer,
		                  CALLBACK,
		                  gWakeUp,
		                  policy );
	}
	
	// Async read, returns zero
	SInt32 FSRead( N::FSFileRefNum  file,
	               SInt32           requestCount,
	               void *           buffer,
	               Async            async,
	               ThrowEOF_Never   policy )
	{
		return N::FSRead( file,
		                  requestCount,
		                  buffer,
		                  CALLBACK,
		                  gWakeUp,
		                  policy );
	}
	
	
	// Async write
	SInt32 FSWrite( N::FSFileRefNum  file,
	                SInt32           requestCount,
	                const void *     buffer,
	                Async            async )
	{
		return N::FSWrite( file,
		                   requestCount,
		                   buffer,
		                   CALLBACK,
		                   gWakeUp );
	}
	
	
	// Asynchronous, throws FNFErr
	void FSpGetCatInfo( const FSSpec&  item,
	                    CInfoPBRec&    pb,
	                    Async          async,
	                    FNF_Throws     policy )
	{
		// Calling the asynchronous variant of FSpGetCatInfo() reliably elicits
		// System Error 27 (dsFSErr: file system map has been trashed) in Classic
		// (eventually), but only when calling StatFile() from CheckProgramFile().
		
		// UPDATE
		// ...until now.  So now we shoot with a shorter barrel and disable
		// async I/O for FSpGetCatInfo( ..., FNF_Throws ) generally.
		
		// On infrequent (but not rare) occasion, ioResult is set and control is
		// returned to the application (allowing it to destroy the parameter block)
		// before the completion routine has run.  Sadly, polling a flag set by
		// the completion routine only delays the crash instead of avoiding it.
		// Apparently this is a bug in the .BlueBoxShared driver.
		
		if ( RunningInClassic() )
		{
			N::FSpGetCatInfo( item, pb, policy );
		}
		else
		{
			N::FSpGetCatInfo( item,
			                  pb,
			                  CALLBACK,
			                  gWakeUp,
			                  policy );
		}
	}
	
	// Asynchronous, returns false on fnfErr
	bool FSpGetCatInfo( const FSSpec&  item,
	                    CInfoPBRec&    pb,
	                    Async          async,
	                    FNF_Returns    policy )
	{
		return N::FSpGetCatInfo( item,
		                         pb,
		                         CALLBACK,
		                         gWakeUp,
		                         FNF_Returns() );
	}
	
}

