/*
	macos.cc
	--------
*/

// Mac OS
#ifndef __TRAPS__
#include <Traps.h>
#endif
#ifndef __EVENTS__
#include <Events.h>
#endif

// POSIX
#include <unistd.h>

// Standard C
#include <stdlib.h>

// command
#include "command/get_option.hh"

// gear
#include "gear/parse_decimal.hh"

// macos
#include "CopyBits.hh"
#include "Cursor.hh"
#include "Debugger.hh"
#include "Drag.hh"
#include "Events.hh"
#include "Fonts.hh"
#include "GrafPorts.hh"
#include "Handles.hh"
#include "InitGraf.hh"
#include "Menus.hh"
#include "OSEvents.hh"
#include "OSUtils.hh"
#include "QDUtils.hh"
#include "Pen.hh"
#include "Rects.hh"
#include "Region-ops.hh"
#include "Regions.hh"
#include "Segments.hh"
#include "Windows.hh"
#include "options.hh"


#define STR_LEN( s )  "" s, (sizeof s - 1)


enum
{
	Opt_linger = 'L',  // linger on ExitToShell
	Opt_qdsync = 'Q',
	
	Opt_last_byte = 255,
	
	Opt_events_fd,
};

static command::option options[] =
{
	{ "linger",  Opt_linger },
	{ "qd-sync", Opt_qdsync },
	
	{ "events-fd", Opt_events_fd, command::Param_required },
};


void* SysEvtBuf : 0x0146;
QHdr EventQueue : 0x014A;
short SysEvtCnt : 0x0154;

unsigned long ScrnBase : 0x0824;
Point         Mouse    : 0x0830;

void* os_trap_table     [] : 1 * 1024;
void* toolbox_trap_table[] : 3 * 1024;

#define OSTRAP( Proc )  (os_trap_table     [ _##Proc & 0x00FF ] = &Proc##_patch)
#define TBTRAP( Proc )  (toolbox_trap_table[ _##Proc & 0x03FF ] = &Proc##_patch)

enum
{
	_BeginUpdate      = _BeginUpDate,
	_CheckUpdate      = _CheckUpDate,
	_EndUpdate        = _EndUpDate,
	_ReallocateHandle = _ReallocHandle,
	_SetPortBits      = _SetPBits,
	_SetRectRgn       = _SetRecRgn,
};


static void initialize_low_memory_globals()
{
	const short n_max_events = 20;
	
	const int event_size = 4 + sizeof (EvQEl);
	
	SysEvtBuf = calloc( event_size, n_max_events );
	SysEvtCnt = n_max_events - 1;
	
	ScrnBase = 0x0001A700;
	
	*(long*) &Mouse = 0x000F000F;  // 15, 15
}

static void install_MemoryManager()
{
	OSTRAP( NewHandle        );  // A022
	OSTRAP( DisposeHandle    );  // A023
	OSTRAP( SetHandleSize    );  // A024
	OSTRAP( GetHandleSize    );  // A025
	OSTRAP( ReallocateHandle );  // A027
	OSTRAP( EmptyHandle      );  // A02B
	OSTRAP( NewEmptyHandle   );  // A066
}

static void install_OSUtils()
{
	OSTRAP( Delay     );  // A03B
	TBTRAP( Enqueue   );  // A96E
	TBTRAP( Dequeue   );  // A96F
	TBTRAP( TickCount );  // A975
	TBTRAP( SysBeep   );  // A9C8
}

static void install_QuickDraw()
{
	TBTRAP( InitCursor );  // A850
	TBTRAP( SetCursor  );  // A851
	TBTRAP( HideCursor );  // A852
	TBTRAP( ShowCursor );  // A853
	
	TBTRAP( Random );  // A861
	
	TBTRAP( InitPort    );  // A86D
	TBTRAP( InitGraf    );  // A86E
	TBTRAP( OpenPort    );  // A86F
	TBTRAP( SetPort     );  // A873
	TBTRAP( GetPort     );  // A874
	TBTRAP( SetPortBits );  // A875
	TBTRAP( PortSize    );  // A876
	TBTRAP( MovePortTo  );  // A877
	
	TBTRAP( SetClip     );  // A879
	TBTRAP( GetClip     );  // A87A
	TBTRAP( ClipRect    );  // A87B
	TBTRAP( BackPat     );  // A87C
	TBTRAP( ClosePort   );  // A87D
	
	TBTRAP( StdLine );  // A890
	TBTRAP( LineTo  );  // A891
	TBTRAP( Line    );  // A892
	TBTRAP( MoveTo  );  // A893
	TBTRAP( Move    );  // A894
	
	TBTRAP( HidePen     );  // A896
	TBTRAP( ShowPen     );  // A897
	TBTRAP( GetPenState );  // A898
	TBTRAP( SetPenState );  // A899
	TBTRAP( GetPen      );  // A89A
	TBTRAP( PenSize     );  // A89B
	TBTRAP( PenMode     );  // A89C
	TBTRAP( PenPat      );  // A89D
	TBTRAP( PenNormal   );  // A89E
	
	TBTRAP( StdRect    );  // A8A0
	TBTRAP( FrameRect  );  // A8A1
	TBTRAP( PaintRect  );  // A8A2
	TBTRAP( EraseRect  );  // A8A3
	TBTRAP( InverRect  );  // A8A4
	TBTRAP( FillRect   );  // A8A5
	TBTRAP( EqualRect  );  // A8A6
	TBTRAP( SetRect    );  // A8A7
	TBTRAP( OffsetRect );  // A8A8
	TBTRAP( InsetRect  );  // A8A9
	TBTRAP( SectRect   );  // A8AA
	TBTRAP( UnionRect  );  // A8AB
	TBTRAP( Pt2Rect    );  // A8AC
	TBTRAP( PtInRect   );  // A8AD
	TBTRAP( EmptyRect  );  // A8AE
	
	TBTRAP( StdRgn      );  // A8D1
	TBTRAP( FrameRgn    );  // A8D2
	TBTRAP( PaintRgn    );  // A8D3
	TBTRAP( EraseRgn    );  // A8D4
	TBTRAP( InverRgn    );  // A8D5
	TBTRAP( FillRgn     );  // A8D6
	
	TBTRAP( BitMapToRegion );  // A8D7
	
	TBTRAP( NewRgn      );  // A8D8
	TBTRAP( DisposeRgn  );  // A8D9
	TBTRAP( CopyRgn     );  // A8DC
	TBTRAP( SetEmptyRgn );  // A8DD
	TBTRAP( SetRectRgn  );  // A8DE
	TBTRAP( RectRgn     );  // A8DF
	TBTRAP( OffsetRgn   );  // A8E0
	TBTRAP( InsetRgn    );  // A8E1
	TBTRAP( EmptyRgn    );  // A8E2
	TBTRAP( SectRgn     );  // A8E4
	TBTRAP( UnionRgn    );  // A8E5
	TBTRAP( DiffRgn     );  // A8E6
	TBTRAP( XOrRgn      );  // A8E7
	TBTRAP( PtInRgn     );  // A8E8
	
	TBTRAP( StdBits     );  // A8EB
	TBTRAP( CopyBits    );  // A8EC
	
	TBTRAP( GetPattern  );  // A9B8
	TBTRAP( GetCursor   );  // A9B9
}

static void install_Fonts()
{
	TBTRAP( InitFonts );  // A8FE
}

static void install_Windows()
{
	TBTRAP( DrawGrowIcon  );  // A904
	TBTRAP( DragGrayRgn   );  // A905
	
	TBTRAP( ClipAbove     );  // A90B
	TBTRAP( PaintOne      );  // A90C
	TBTRAP( PaintBehind   );  // A90D
	
	TBTRAP( CheckUpdate   );  // A911
	TBTRAP( InitWindows   );  // A912
	TBTRAP( NewWindow     );  // A913
	TBTRAP( DisposeWindow );  // A914
	
	TBTRAP( MoveWindow    );  // A91B
	
	TBTRAP( SizeWindow    );  // A91D
	TBTRAP( TrackGoAway   );  // A91E
	
	TBTRAP( BeginUpdate   );  // A922
	TBTRAP( EndUpdate     );  // A923
	TBTRAP( FrontWindow   );  // A924
	TBTRAP( DragWindow    );  // A925
	TBTRAP( DragTheRgn    );  // A926
	TBTRAP( InvalRgn      );  // A927
	TBTRAP( InvalRect     );  // A928
	TBTRAP( ValidRgn      );  // A929
	TBTRAP( ValidRect     );  // A92A
	
	TBTRAP( GrowWindow    );  // A92B
	TBTRAP( FindWindow    );  // A92C
	TBTRAP( CloseWindow   );  // A92D
}

static void install_Menus()
{
	TBTRAP( InitMenus    );  // A930
	TBTRAP( DrawMenuBar  );  // A937
	TBTRAP( FlashMenuBar );  // A94C
}

static void install_EventManager()
{
	OSTRAP( PostEvent );  // A02F
	
	TBTRAP( WaitNextEvent );  // A860
	TBTRAP( GetNextEvent  );  // A970
}

static void install_SegmentLoader()
{
	TBTRAP( LoadSeg     );  // A9F0
	TBTRAP( Launch      );  // A9F2
	TBTRAP( ExitToShell );  // A9F4
}

static void install_Debugger()
{
	TBTRAP( Debugger );  // A9FF
	TBTRAP( DebugStr );  // ABFF
}

static asm void module_suspend()
{
	JSR      0xFFFFFFF8
}

static char* const* get_options( char** argv )
{
	using command::global_result;
	
	int opt;
	
	++argv;  // skip arg 0
	
	while ( (opt = command::get_option( (char* const**) &argv, options )) > 0 )
	{
		switch ( opt )
		{
			case Opt_linger:
				linger_on_exit = true;
				break;
			
			case Opt_qdsync:
				synchronized_quickdraw = true;
				break;
			
			case Opt_events_fd:
				events_fd = gear::parse_unsigned_decimal( global_result.param );
				break;
			
			default:
				break;
		}
	}
	
	return argv;
}

int main( int argc, char** argv )
{
	if ( argc > 0 )
	{
		char* const* args = get_options( argv );
		
		if ( *args != NULL )
		{
			write( STDERR_FILENO, STR_LEN( "macos: no arguments allowed\n" ) );
			
			_exit( 1 );
		}
	}
	
	initialize_low_memory_globals();
	
	install_MemoryManager();
	
	install_OSUtils();
	
	install_QuickDraw();
	install_Fonts();
	install_Windows();
	install_Menus();
	
	install_EventManager();
	
	install_SegmentLoader();
	
	install_Debugger();
	
	module_suspend();  // doesn't return
}
