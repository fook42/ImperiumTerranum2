# ImperiumTerranum2
Improved &amp; adopted C version of ImperiumTerranum2 from Oxygenic/virtualworlds

original game: https://www.virtualworlds.de/IT/

How to play
---

1) download the original game archive from here: 
* https://www.virtualworlds.de/IT/IT2000.lha

or
* http://aminet.net/game/strat/IT2000.lha

2) unpack & install this game
use the 2 install scripts first:
* CopyLibs  - to copy medplayer.libary and fonts to your system-directories
* Install_Language  - lets you select german/english language for ingame texts

3) optional: run the original game by the icon "IMPERIUM_TERRANUM"

4) copy release-files of IT2-C Version from here : https://github.com/fook42/ImperiumTerranum2/releases
- IT2C and IT2C.icon

5) start the improved version of this game by IT2C from now on.

Changing Screenmodes
---
This version supports the selection of 2 screenmodes for the game (640x512x6 and 320x256x6).
For this, a screenmode-requester is implemented which stores the selection in the game-icon (IT2C.icon) as 2 tooltypes "BigScreenMode" and "LowScreenMode".
Once these tooltypes exists the game won't ask again for new screenmodes.

To open up the screenmode-requester again, simply remove the tooltypes from the icon.

Issues
---
Savegames from original version of IT2 and IT2-C version are NOT 100% compatible - loading of a wrong savegame leads to memory corruption and can cause system crashes!
Make sure to use the right savegame for the right version of IT2.

How to compile
---

now using CMAKE:

create a folder "build", and use cmake to generate a Makefile with the given toolchain-file

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../CMake/toolchain-amigagcc.cmake ..

run make to generate the IT2C binary:

make -j4

currently the toolchain-file is written to use installed amigagcc from bebbo: https://github.com/bebbo/amiga-gcc
other compilers are usable as well.


HINT:
medplayer headers and lib are needed for compilation!

