# 3D Studio

This project is part of the Computer Graphics course at the Umeå University. The aim is to create a 3D Studio where you can 3D and 2D models and adding lights and other effects.

## Compiling

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

## Keyboard shortcuts

This section lists all the differnet keybinds used for navigating and using the 3D studio. A graphical user interface (GUI) will be developed which will be the primary way of navigation and transformation but these keyboard shortcuts are made for an enhanced user experience.

The keyboard shortcust are summarized in the table below.

<div align=center>

| Key        | Function                                                     | Progress    |
|:----------:|:------------------------------------------------------------:|:-----------:|
| W          | **Moves** camera **forward**                                 | Done        |
| S          | **Moves** camera **backwards**                               | Done        |
| A          | **Moves** camera **left**                                    | Done        |
| D          | **Moves** camera **right**                                   | Done        |
| E          | **Moves** camera **up**                                      | Done        |
| Q          | **Moves** camera **down**                                    | Done        |
| I          | **Translate** object **up** (*0.1 units*)                    | Done        |
| K          | **Translate** object **down** (*0.1 units*)                  | Done        |
| J          | **Translate** object **left** (*0.1 units*)                  | Done        |
| L          | **Translate** object **right** (*0.1 units*)                 | Done        |
| Y          | **Translate** object **forward** (*0.1 units*)               | Done        |
| H          | **Translate** object **backwards** (*0.1 units*)             | Done        |
| Arrow Up   | **Rotate** object **up** (*10.0°*)                           | Done        |
| Arrow Down | **Rotate** object **down** (*10.0°*)                         | Done        |
| Arrow Left | **Rotate** object **left** (*10.0°*)                         | Done        |
| Arrow Right| **Rotate** object **right** (*10.0°*)                        | Done        |
| COMMA      | **Rotate** object **left** on *z-axis* (*10.0°*)             | Done        |
| PERIOD     | **Rotate** object **right** on z-axis (*10.0°*)              | Done        |
| R          | **Resets** the objects transformation                        | Done        |
| F1         | Toggles the **Loaded Objects** window                        | Done        |
| F2         | Toggles the **Object Translation** window                    | Done        |
| F3         | Toggles the **Object Material** window                       | Done        |
| F4         | Toggles the **Object Information** window                    | Done        |
| F5         | Toggles the **Camera** window                                | Done        |
| F6         | Toggles the **Light Sources** window                         | Done        |
| F9         | Toggles the **Log** window                                   | Done        |
| F10        | Toggles the **Keyboard Shortcut Reference** window          | Done        |

</div>

It is worth mentioning that this table is subject to change during development of the program. Therefore it is good practice to always check this table if any uncertainties occur.

## TODO

This section contains the things that are not yet implemented in the project. After each programming session a new list with the things that are to be done in the next session will be created. Each list will be labeled by the date which corresponds to the date when the list was created. The lists are ordered with the latest date being presented at the top and descending. This makes it easier to organize the project and gives a good idea of what needs to be done.

### 2023-01-19

- Fixed the problems that was given from the feedback from the teacher.
- I was cringe that did not fix these problems beforehand ngl.

### 2023-01-08

- Finally done with all the documentation regarding the code.
- Last thing to fix is the presentation this thursday.
- Really fun project!

### 2024-01-02

- Still unsure whether to implement skybox and ground at all or not.
- Continue to work on the presentation.
- Refactor and fix project structure.
- Illustrate the architecture in UML or other ways.
- Continue to comment the code.

### 2023-12-31

- Think all features that were deemed to be implemented is done.
- Perhaps implement skyboxes and ground for the program.
- Video for skybox tutorial: <https://www.youtube.com/watch?v=8sVvxeKI9Pk>
- StackOverflow discussion about ground plane: <https://stackoverflow.com/questions/12965161/rendering-infinitely-large-plane>
- Continue to refactor and fix project structure.
- Continue to comment the code.

### 2023-12-23

- Fix such that every FACE are rendered instead of each shape. (Maybe good/bad we shall see after the test!).
- Fix the problem when the object becomes pure black if not all coefficients are set.
- Continue to refactor and fix project structure.
- Continue to comment the code.

### 2023-12-22

- Implement such that material files can be used.
- Try to make the project loosely coupled.
- Decide which of the bonus tasks should be done.
- Continue to refactor and fix project structure.
- Start to comment the code.

### 2023-12-21

- Done with the base criterias for the part 3 of the project.
- Decide which of the bonus tasks should be done.
- Give different vertex buffers to each object and make them able to draw themselves.
- Make it posible to render multiple objects.
- Add ground floor.
- Continue to refactor and fix project structure.
- Start to comment the code.
- Try to fix the texture mapping, when rotating the object the texture is static but should follow the object!

### 2023-12-20

- Move the lightning calculations from the vertex shader to the fragment shader for per-pixel shading.
- Fix the weird lightning that happens when loading a object.
- Start with the texture mapping.
- Continue to refactor the code and fix project structure, the technical dept is starting to get huge...
- Continue with part 3 of the project.

### 2023-12-19

- Project structure is somewhat better but must continue to imporve it for better scalability.
- Started implementing the lightning with per-vertex lightning.
- The way I load information from the loader is wrong, fix it!
- Continue with part 3 of the project.

### 2023-12-05

- Fix the resizing of the object that is rendered.
- Fix the structure for the project to increase scalability and ease of development.
- Start with part 3 of the project.

### 2023-12-01

- Fix the bug with the calculations of the lightning from the workshop code.
- Fix project structure.
- Make everything ready for the part 2 presentation.

### 2023-11-24

- Continue to implement the GUI.
- Seperate some of the gui code into own namespace.
- Divide the whole project into smaller containers for better sustainability.
- Document the written code for better understandability.
- Refactor the updateView for better readability.

### 2023-11-22

- Fix the weird rotation when camera is near the center (0,0,0).
- Change the way of using keyCallback so that it matches the *Dear ImGui* ways instead of pure *GLFW*.
- Implement the Object transformation window.
- Finish the content for the About popup.
- Figure out a nice way of handling settings of the application.
- Continue implementation of GUI.
- Document the written code for better understandability.
- Refactor the code for better readability.

### 2023-11-20

- Compute the vectors n, u and v which should be used when producing the view matrix.
- Understand and produce the projection matrix.
- Refactor some code and hook up values to the GUI.
- Add additional GUI elements, such as textSeperators and other stuff.

### 2023-11-16

- Demonstrate the project on friday (2023-11-17).
- Begin with part 2 of the project.

### 2023-11-15

- Make it possible to store and load multiple shapes into the buffer.
- Maybe add some output information of the object files upon loading.

### 2023-11-14

- Continue to refactor different parts of the code.
- Add further comments to the code.
- Check if the GUI-code has been uploaded to the course site.
- Further learn how to develop GUI in Dear ImGui.

### 2023-11-10

- Continue implementation on the loading of objects by pressing the '**o**' button.
- Fix the crash when loading new object.
- Scale loaded objects to fit in the *NDC cube*. (Scaling or adjusting vertex data?)
- Refactor the **loader** class.
- Document the written code.

### 2023-11-08

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

#### OpenGL Reference pages

    https://registry.khronos.org/OpenGL-Refpages/gl4/

#### OpenGL Reference card

    https://www.khronos.org/files/opengl43-quick-reference-card.pdf

#### OpenGL Wiki

    https://www.khronos.org/opengl/wiki/

### Dear ImGui Resources

This section includes the resources for the **Dear ImGui** library.

#### Dear ImGui GitHub Page

    https://github.com/ocornut/imgui

#### Dear ImGui Introduction Tutorial

    https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html

### GLM Resources

This section includes links to resources that can help with using the GLM library.

#### GLM Manual

    https://github.com/g-truc/glm/blob/master/manual.md

### GLFW Resources

This section presents the different resources that helped on how to use the GLFW library.

#### GLFW Documentation

    https://www.glfw.org/docs/latest/

### tiny_obj_loader Resources

This section includes different resources that helped to understand how to use the **tiny_obj_loader** class.
This loader was mainly used for the inital parsing of the *obj files.

#### tiny_obj_loader GitHub Page

    https://github.com/tinyobjloader/tinyobjloader

## Miscellaneous links and other stuff

Found a website with free object files to download. These include materials, color and more.

    https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html
