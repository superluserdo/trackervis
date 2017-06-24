# G-2 Track Visualiser (Unfinished)

Designed to read in a pre-analysed list of track events ("trackevents.txt") and display them on an interactive scale diagram of the G-2 tracker setup.

* Written in C.

* Requires the `libSDL2` library.

* `libconfig` and `libSDL2_gfx` libraries optional. Can be linked dynamically or statically.

## Installation

To run the track visualiser:

### With `libconfig` and `libSDL2_gfx` dynamically linked

* Ensure SDL2, SDL2-devel, SDL2_gfx, and libconfig are installed.

* Clone this repository to a local folder.

* Run `make && make clean` to compile.

* Run ./trackervis` to run the program.

### With `libconfig` and `libSDL2_gfx` statically linked

* Ensure SDL2 and SDL2-devel are installed.

* Clone this repository to a local folder.

* Overwrite `makefile` with `makefile-dynamic`.

* Run `make && make clean` to compile.

* Run `LD_LIBRARY_PATH=lib ./trackervis` to run the program.


## Usage

|			|						|
|---------------|-------------------------------|
|ESC		|Quit program.			|
|UP/DN Arrows	|Zoom in/out.			|
|Scroll wheel	|Zoom in/out.			|
|ALT + scroll	|Adjust Y value.		|
|Click +	|drag	Move display around.	|
|L/R Arrows	|Change event number.		|
|T		|Toggle heat tally.		|

## Syntax

`trackevents.txt` is written to by `./TestStraws` in libconfig syntax, then parsed by the libconfig library in `./trackervis`. The file contains one list named `trackevents`. Each list element in `trackevents` contains `module`, `hitnum`, `Ybest`, a list named `hits`, and a list named `line`. Each element of `hits` contains `X`, `Z`, `layer`, and `straw`. Each element of `line` contains `Z1`, `X1`, `Z2`, `X2`.

|Hierarchy	|Type	|
|---------------|-------|
|>trackevents	|list	|
|---->module	|int	|
|---->hitnum	|int	|
|---->hits	|list	|
|-------->X	|float	|
|-------->Z	|float	|
|-------->layer	|int	|
|-------->straw	|int	|
|---->line	|list	|
|-------->Z1	|float	|
|-------->X1	|float	|
|-------->Z2	|float	|
|-------->X2	|float	|
|---->Ybest	|float	|
