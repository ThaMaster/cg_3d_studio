# cg_3d_studio

This project is part of the Computer Graphics course at the Umeå University. The aim is to create a 3D Studio where you can 3D and 2D models and adding lights and other effects

## Compiling from home.
The line below was used in the makefile for the **LGLFLAGS** to be able to compile at the computer at home. This fixes the error "undefiened reference to symbol 'XConvertSelection". This error seems to be caused by the order of the includes was incorrect.

    pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3

As mentioned, the line above seems to fix the error. If this causes errors on the CS-department computers the line below was the one that was used.

    pkg-config --libs glew glfw3 gl