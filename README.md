Super Stack Up
==============

Super Stack Up is a game whose goal is to stack objects as high as possible, without them falling.

Goals
-----
* Box2D physics powered (2D) game engine
* Player movement of objects using the mouse
* Scroll/Zoom ability
* Score based on highest block reached in a tower

Build Details
-------------

Super Stack Up (SSU) requires the following libraries in order to build:
>SDL

>SDL_ttf

>Box2D

On any Linux system with the proper libraries installed, SSU should build via the following steps

>make

You can then find the binary in the "bin" directory
If something does go wrong, take a look at the Makefile and edit it to your needs.

Current Priorities
------------------
* Text rendering with freetype
* Calculate highest point (Box2D AABB tests)