# Exercise 1
Look at the coordinates of the 2D-triangle and how it appears on screen.

>Where is the 2D-coordinate (0, 0) located in NDC? 

- On the screen, the (0, 0) in NDC is **in the MIDDLE of the screen**.

>What 2D-coordinate has the lower left corner of the window?

- The coordinates of the lower left corner of the window are **(-1, -1)**.

# Exercise 2

Change the z-value in vshader.glsl between -2.0 and 2.0. 

>For which values do we see a figure on the screen? 

- We can see the figure with z-values that are **[-1, 1)**.

>What do you think happens with the triangle when it is not visible?

- When using z-values that are not within these bounds, the verticies are outside the NDC cube and therefore will not be rendered. Everything within this cube will be rendered. The NDC cube has coordinates in -1 and 1 in all of the axises (X,Y,Z).

>Note: You do not need to recompile the program when you only do changes in the shader files (however, you need to restart the program). Why?

- All the shader files are compiled in runtime. We therefore do not need to recompile, simply restart the program.

# Exercise 3

To summarize.

>Which NDC-coordinates are by default projected to the window? 

- The coordinates that are default projected to the window are -1 to 1 in both x and y axis.
    
>What happens with the vertices and lines outside of this cube?

- Those lines are simply not rendered and not shown on the window.

# Exercise 4

In the class OpenGLWindow, create a new function reshape and use glViewport to define a viewport aligned in the lower left corner of the window and equal the window's width and height. Call reshape from the (currently empty) callback function resizeCallback. But before we call glViewport we have to check that we have a valid context. That is done by
adding the following lines in the beginning of reshape:

    if (glfwGetCurrentContext() == nullptr)
        return;

>What happens now when the window is resized?

- Before introducing this change the figure had the same position (in the lower left corner) and had the same scale independent of the window. But after the change the figure scales correctly with the window.

Again, identify glDrawElements. We only have a triangle but try to change the first argument to GL_LINE_STRIP, GL_LINES, GL_POINTS, or GL_TRIANGLES and see what happens.

>If we would like to draw the same triangle using GL_LINES instead, how would that have to affect the contents of *vertices* and *indices*?

- The **GL_LINES** argument displays a line between two points. Therefore, when just using the argument without changing anything only one line will appear. We need to create additional verticies in the *vertices* array so that we have 6 elements and create additional indices in the *indices* array so we have 6 elements there aswell. This will create a line between all the presented points.