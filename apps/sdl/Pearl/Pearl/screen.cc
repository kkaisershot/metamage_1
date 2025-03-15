/*
	screen.cc
	---------
*/

#include "Pearl/screen.hh"

// POSIX
#include <fcntl.h>
#include <unistd.h>

// libsdl2
#if defined(__MSYS__) || defined(__CYGWIN__)
// MSYS/Cygwin doesn't know about _beginthreadex / _endthreadex
#define SDL_beginthread NULL
#define SDL_endthread NULL
#endif
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_thread.h>

// frontend-common
#include "frend/coprocess.hh"
#include "frend/cursor.hh"
#include "frend/make_cursor.hh"
#include "frend/make_raster.hh"
#include "frend/raster_updating.hh"
#include "frend/update_fifo.hh"

// rasterlib
#include "raster/clut.hh"
#include "raster/clut_detail.hh"
#include "raster/raster.hh"
#include "raster/relay_detail.hh"
#include "raster/sync.hh"

// v68k-cursor
#include "cursor/cursor.hh"

// Pearl
#include "Pearl/events.hh"


namespace Pearl
{

using raster::raster_desc;
using raster::raster_load;

static bool monitoring;

static SDL_Thread* raster_thread;

static
void raster_event_loop( const raster::raster_note* note )
{
	const SDL_EventType eventClass = (SDL_EventType) pearl_event_class;
	const Sint32        repaintDue = kEventPearlUpdate;
	const Sint32        screenBits = kEventPearlScreenBits;
	const Sint32        cursorBits = kEventPearlCursorBits;
	const Sint32        clutBits   = kEventPearlCLUTBits;

	SDL_UserEvent repaint_due = { eventClass };
	SDL_UserEvent screen_bits = { eventClass };
	SDL_UserEvent cursor_bits = { eventClass };
	SDL_UserEvent clut_bits   = { eventClass };
	repaint_due.code = repaintDue;
	screen_bits.code = screenBits;
	cursor_bits.code = cursorBits;
	clut_bits.code   = clutBits;

	uint32_t raster_seed = 0;
	uint16_t cursor_seed = 0;
	uint32_t clut_seed   = 0;

	const raster::sync_relay* sync = find_sync( note );
	const raster::clut_data*  clut = find_clut( note );

	while ( monitoring  &&  sync->status == raster::Sync_ready )
	{
		using frend::cursor_state;

	#if ! defined(__MSYS__)  &&  ! defined(__CYGWIN__)

		close( open( UPDATE_FIFO, O_WRONLY ) );

	#else

		usleep( 8333 );

	#endif

		if ( clut  &&  clut_seed != clut->seed )
		{
			clut_seed = clut->seed;

			SDL_PushEvent( (SDL_Event*) &clut_bits );
		}

		if ( cursor_state  &&  cursor_state->seed != cursor_seed )
		{
			cursor_seed = cursor_state->seed;

			SDL_PushEvent( (SDL_Event*) &cursor_bits );
		}

		if ( raster_seed != sync->seed )
		{
			raster_seed = sync->seed;

			SDL_PushEvent( (SDL_Event*) &screen_bits );
		}

		SDL_PushEvent( (SDL_Event*) &repaint_due );
	}
}

static
int raster_thread_entry( void* arg )
{
	raster_event_loop( (const raster::raster_note*) arg );

	SDL_QuitEvent quitEvent = { SDL_QUIT };
	return SDL_PushEvent( (SDL_Event*) &quitEvent ) ? 0 : 1;
}

raster_monitor::raster_monitor( const raster::raster_load& load )
{
	pearl_event_class = SDL_RegisterEvents( 1 );

	monitoring = true;

	raster_thread = SDL_CreateThread( &raster_thread_entry, NULL, (void*) &load.meta->note );
}

raster_monitor::~raster_monitor()
{
	monitoring = false;

	SDL_WaitThread( raster_thread, NULL );
}

static const char raster_path[] = "screen.skif";
static const char cursor_path[] = "cursor.skif";

emulated_screen::emulated_screen( int bindir_fd, const char* works_path )
:
	live_cursor       ( cursor_path ),
	live_raster       ( raster_path ),
	monitored_raster  ( live_raster.get() ),
	launched_coprocess( bindir_fd, works_path )
{
	events_fd = launched_coprocess.socket();
}

}
