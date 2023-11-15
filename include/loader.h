#pragma once

#include <glm/glm.hpp>


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
        bool parseFile(string);
        void normalizeVertexCoords();
        void clearLoader();

        // The name of the last successfully loaded object file.
        string loadedFileName;

        // Vertex coordinates.
        vector<vector<glm::vec3>> vertexCoords;

        // Vertex indexes.
        vector<vector<unsigned int>> indices;

        // Vertex normals.
        vector<vector<glm::vec3>> vertexNormals;

        // Texture coordinates.
        vector<vector<glm::vec3>> textureCoords;

        // Color values.
        vector<vector<glm::vec3>> colorVals;

        // Number of objects that are stored in the loader.
        size_t numberOfObjects = 0;

    protected:

    private:

        // Searches a shapes vertices and finds the longest vector lenght
        float getLargestVertexLength(size_t);
};