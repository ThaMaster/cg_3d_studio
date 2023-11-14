# 3D Studio

This project is part of the Computer Graphics course at the Umeå University. The aim is to create a 3D Studio where you can 3D and 2D models and adding lights and other effects.

## Manual

### Compiling

Because this program is developed with the **C++** programming language it requires the user to have a compiler that can compile **.cpp** files. Different libraries and header files were also used that were not developed, these include:

- The **C++** programming language.
- The **OpenGL** graphical language.
- The **GLM** library for matrix/vector manipulation.
- The **GLFW** library for window management.
- The **tiny_object_loader** for handling inital parsing of object files.

The list above shows the different dependencies that is required to run the program. If all these are installed, simply using the **MakeFile** to compile should be enough.

### Problems with compiling from home

The line below was used in the makefile for the **LGLFLAGS** to be able to compile at the computer at home. This fixes the error "undefiened reference to symbol 'XConvertSelection". This error seems to be caused by the order of the includes being incorrect.

    pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3

The above line forces the includes to be included in the right order.

As mentioned, the line above seems to fix the error. If this causes errors on the CS-department computers the line below was the one that was used.

    pkg-config --libs glew glfw3 gl

### Keyboard shortcuts

This section lists all the differnet keybinds used for navigating and using the 3D studio. A graphical user interface (GUI) will be developed which will be the primary way of navigation and transformation but these keyboard shortcuts are made for an enhanced user experience.

The keyboard shortcust are summarized in the table below.

<div align="center">

| Key        | Function                                            | Progress    |
|:----------:|:---------------------------------------------------:|:-----------:|
| Arrow Left | Rotate object left (*10.0°*)                        | Done        |
| Arrow Right| Rotate object right (*10.0°*)                       | Done        |
| Arrow Up   | Rotate object up (*10.0°*)                          | Done        |
| Arrow Down | Rotate object Down (*10.0°*)                        | Done        |
| COMMA      | Rotate object left on z-axis (*10.0°*)              | Done        |
| PERIOD     | Rotate object on z-axis (*10.0°*)                   | Done        |
| J          | Translate object left (*0.1 units*)                 | Done        |
| L          | Translate object right (*0.1 units*)                | Done        |
| I          | Translate object up (*0.1 units*)                   | Done        |
| K          | Translate object down (*0.1 units*)                 | Done        |
| Y          | Translate object forward (*0.1 units*)              | Done        |
| H          | Translate object backwards (*0.1 units*)            | Done        |
| R          | Resets the objects transformation                   | Done        |
| O          | Load object from terminal (will change to GUI later)| Done        |
| E          | Moves camera **up**                                 | To come     |
| Q          | Moves camera **down**                               | To come     |
| D          | Moves camera **right**                              | To come     |
| A          | Moves camera **left**                               | To come     |
| W          | Moves camera **forward**                            | To come     |
| S          | Moves camera **backwards**                          | To come     |

</div>

It is worth mentioning that this table is subject to change during development of the program. Therefore it is good practice to always check this table if any uncertainties occur.

## TODO

This section contains the things that are not yet implemented in the project. After each programming session a new list with the things that are to be done in the next session will be created. Each list will be labeled by the date which corresponds to the date when the list was created. The lists are ordered with the latest date being presented at the top and descending. This makes it easier to organize the project and gives a good idea of what needs to be done. 

*2023-11-10*
- Continue implementation on the loading of objects by pressing the '**o**' button.
- Fix the crash when loading new object.
- Scale loaded objects to fit in the *NDC cube*. (Scaling or adjusting vertex data?)
- Refactor the **loader** class.
- Document the written code.

*2023-11-08*
- Implement the ability to **shear** objects in the studio.
- Implement the ability to **load** new objects by pressing the '**O**' int the studio.
- Include the **tiny_object_loader** class into the project.
- Convert the output of the **tiny_object_loader** to glm vectors and store them.
- Refactor for better readability and make the code fit both the C++ language and object oriented programming more.

## Helpful resources when developing the 3D Studio

Below follows some useful links and resources to the different parts of the project. These resources includes reference pages, references cards, manuals, documentation and more.

The different tools that were used includes:

- The **C++** programming language.
- The **OpenGL** graphical language.
- The **GLM** library for matrix/vector manipulation.
- The **GLFW** library for window management.
- The **tiny_obj_loader** for the initial parsing of object files.

### C++ Resources

This section includes all the learning resources used to get accustomed with the C++ programming language. Since this is the first time programming a larger project with the language tutorials and other resources are also included in this section. 

#### C++ Reference Page

    https://en.cppreference.com/w/cpp

#### C++ Tutorial

    https://www.learncpp.com/?utm_content=cmp-true

#### C++ Tour

    https://isocpp.org/tour

#### C++ Documentation Guidelines

    https://developer.lsst.io/cpp/api-docs.html#

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

### tiny_obj_loader Resources

This section includes different resources that helped to understand how to use the **tiny_obj_loader** class. 
This loader was mainly used for the inital parsing of the *obj files.

#### tiny_obj_loader GitHub Page

    https://github.com/tinyobjloader/tinyobjloader