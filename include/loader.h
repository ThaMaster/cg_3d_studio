#pragma once

#include <glm/glm.hpp>
#include <cstring>

#include "object.h"
#include "tiny_obj_loader.h"

using namespace std;

/**
 * Class representing a object loader. The class uses the tiny_obj_loader.h 
 * (https://github.com/tinyobjloader/tinyobjloader) to handle the inital parsing of 
 * the object files.
 * 
 * Stores information about an object such as:
 *      - Vertex Coordinates
 *      - Indices
 *      - Vertex Normals
 *      - Texture Coordinates
 *      - Color values
 * 
 * The loader can store multiple shapes of an object which is why the object 
 * attributes are 2d vectors containing the data. 
 */
class Loader 
{   
    public:

        Object parseFile(string, string);
        void normalizeVertexCoords(vector<Vertex>&, float l);
        string outputString = "";
        string getOutputString() const { return outputString; }

};