# Campus-Map-Game
Multiplayer Game Set in the IIT Delhi Campus 

---------------------------------------------

Installation Instructions :

OS Details : Ubuntu

Required Libraries :

-SDL2

\>>>> sudo apt-get install libsdl2-dev

-SDL2_ttf

\>>>> sudo apt-get install libsdl2-ttf-dev

-SDL2_image

\>>>> sudo apt-get install libsdl2-image-dev

Compilation Instructions :

Run the Server makefile :

\>>>> make

\>>>> ./s.o

Run the Client makefile :

\>>>> make

\>>>> ./s.o

use this command to remove s.o file 

\>>>> make clean

Socketing :

Server will give its IP address to client and client will use that address to find location of player in game. Server will also find the location of client . By using socketing, we can make this 2 player game.

Sources Referred :   

-https://www.geeksforgeeks.org/socket-programming-cc/

-https://lazyfoo.net/tutorials/SDL/index.php


