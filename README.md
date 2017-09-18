# G-2 Track Visualiser (Unfinished)

Designed to read in a pre-analysed list of track events ("trackevents.txt") and display them on an interactive scale diagram of the G-2 tracker setup.

* Written in C.

* Requires the `libSDL2`, `libconfig` and `libSDL2_gfx` libraries. These can be linked dynamically or locally.

## Installation

To run the track visualiser:

* Clone this repository to a local folder.

* To locally link libraries, type `git checkout all_local`

* Run `make && make clean` to compile.

* Run `./trackervis <trackevents file>` to run the program on a specified file.


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

## Other

I have put in the option to change UI elements of the program from the file 'trackervis.conf'. It's only got four options right now though (2 UI features and x/y resolution).
