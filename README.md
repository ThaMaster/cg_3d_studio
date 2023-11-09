# 3D Studio

This project is part of the Computer Graphics course at the Umeå University. The aim is to create a 3D Studio where you can 3D and 2D models and adding lights and other effects.

## Manual

### Keyboard shortcuts

This section lists all the differnet keybinds used for navigating and using the 3D studio. A graphical user interface (GUI) will be developed which will be the primary way of navigation and transformation but these keyboard shortcuts are made for an enhanced user experience.

The keyboard shortcust are summarized in the table below.

<center>

| Key        | Function                                            |
|:----------:|:---------------------------------------------------:|
| Arrow Left | Rotate object left (*10.0°*)                        | 
| Arrow Right| Rotate object right (*10.0°*)                       |
| Arrow Up   | Rotate object up (*10.0°*)                          |
| Arrow Down | Rotate object Down (*10.0°*)                        |
| J          | Translate object left (*0.1 units*)                 |
| L          | Translate object right (*0.1 units*)                |
| I          | Translate object up (*0.1 units*)                   |
| K          | Translate object down (*0.1 units*)                 |
| O          | Load object from terminal (will change to GUI later)|

</center>

## TODO

This section contains the things that are not yet implemented in the project. After each programming session a new list with the things that are to be done in the next session will be created. Each list will be labeled by the date which corresponds to the date when the list was created. The lists are ordered with the latest date being presented at the top and descending. This makes it easier to organize the project and gives a good idea of what needs to be done. 

*2023-11-08*
- Implement the ability to **shear** objects in the studio.
- Implement the ability to **load** new objects by pressing the '**O**' int the studio.
- Include the **tiny_object_loader** class into the project.
- Convert the output of the **tiny_object_loader** to 
- Refactor for better readability and make the code fit both the C++ language and object oriented programming more.

## Compiling from home.
The line below was used in the makefile for the **LGLFLAGS** to be able to compile at the computer at home. This fixes the error "undefiened reference to symbol 'XConvertSelection". This error seems to be caused by the order of the includes being incorrect.

    pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3

The above line forces the includes to be included in the right order.

As mentioned, the line above seems to fix the error. If this causes errors on the CS-department computers the line below was the one that was used.

    pkg-config --libs glew glfw3 gl


## Helpful resources when developing the 3D Studio

Below follows some useful links and resources to the different parts of the project. These resources includes reference pages, references cards, manuals, documentation and more.

The different tools that were used includes:

- The **C++** programming language.
- The **OpenGL** graphical language.
- The **GLM** library for matrix/vector manipulation.
- The **GLFW** library for window management.
- The **tiny_object_loader** for loading object files.

### C++ Resources

This section includes all the learning resources used to get accustomed with the C++ programming language. Since this is the first time programming a larger project with the language tutorials and other resources are also included in this section. 

#### C++ Reference Page

    https://en.cppreference.com/w/cpp

#### C++ Tutorial

    https://www.learncpp.com/?utm_content=cmp-true

#### C++ Tour

    https://isocpp.org/tour

### OpenGL Resources

This section includes some useful links and resources that will help on how to use the OpenGL language. These resources include reference pages, a reference card and the created wikipedia page.

#### OpenGL Reference pages:
    https://registry.khronos.org/OpenGL-Refpages/gl4/

#### OpenGL Reference card:
    https://www.khronos.org/files/opengl43-quick-reference-card.pdf

#### OpenGL Wiki:
    https://www.khronos.org/opengl/wiki/

### GLM Resources

This section includes links to resources that can help with using the GLM library.

#### GLM Manual

    https://github.com/g-truc/glm/blob/master/manual.md

### GLFW Resources

This section presents the different resources that helped on how to use the GLFW library.

#### GLFW Documentation:
    https://www.glfw.org/docs/latest/

### tiny_object_loader Resources

This section includes different resources that helped to understand how to use the **tiny_object_loader** class.

#### tiny_object_loader GitHub Page

    https://github.com/tinyobjloader/tinyobjloader