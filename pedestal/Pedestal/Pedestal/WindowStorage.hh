/*
	WindowStorage.hh
	----------------
*/

#ifndef PEDESTAL_WINDOWSTORAGE_HH
#define PEDESTAL_WINDOWSTORAGE_HH

// Mac OS X
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

// Mac OS
#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

// nucleus
#ifndef NUCLEUS_OWNED_HH
#include "nucleus/owned.hh"
#endif

// Nitrogen
#ifndef MAC_WINDOWS_TYPES_WINDOWATTRIBUTES_HH
#include "Mac/Windows/Types/WindowAttributes.hh"
#endif
#ifndef MAC_WINDOWS_TYPES_WINDOWCLASS_HH
#include "Mac/Windows/Types/WindowClass.hh"
#endif
#ifndef MAC_WINDOWS_TYPES_WINDOWDEFPROCID_HH
#include "Mac/Windows/Types/WindowDefProcID.hh"
#endif
#ifndef MAC_WINDOWS_TYPES_WINDOWREF_HH
#include "Mac/Windows/Types/WindowRef.hh"
#endif


namespace Pedestal
{
	
	typedef void (*WindowClosed_proc )( WindowRef window );
	typedef void (*WindowResized_proc)( WindowRef window );
	
	class View;
	
	WindowAttributes get_window_attributes( WindowRef window );
	
	void set_window_view( WindowRef window, View* view );
	
	View* get_window_view( WindowRef window );
	
	void set_window_closed_proc( WindowRef          window,
	                             WindowClosed_proc  proc );
	
	void set_window_resized_proc( WindowRef           window,
	                              WindowResized_proc  proc );
	
	WindowResized_proc get_window_resized_proc( WindowRef window );
	
	void invalidate_window( WindowRef window );
	
	void close_window( WindowRef window );
	
	nucleus::owned< WindowRef >
	//
	CreateWindow( const Rect&           bounds,
	              ConstStr255Param      title     = "\p",
	              bool                  visible   = true,
	              Mac::WindowDefProcID  procID    = Mac::documentProc,
	              bool                  hasGoAway = true );
	
	nucleus::owned< WindowRef > CreateWindow( Mac::WindowClass       wClass,
	                                          Mac::WindowAttributes  attrs,
	                                          const Rect&            bounds );
	
}

#endif
