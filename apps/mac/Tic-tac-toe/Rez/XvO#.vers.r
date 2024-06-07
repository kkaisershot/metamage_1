#define COPYRIGHT  "2018-2024 Josh Juran"

#define COPY "\0xA9"

#define MAJOR 0
#define MINOR 3
#define POINT 0

#define VERSION  "0.3"

type 'XvO#' as 'STR ';

resource 'XvO#' (0, "Tic-tac-toe")
{
	"Copyright " COPY " " COPYRIGHT
};

resource 'vers' (1)
{
	MAJOR,
	MINOR << 4 | POINT,
	release,
	0,
	smRoman,
	VERSION,
	VERSION ", " COPY COPYRIGHT
};
