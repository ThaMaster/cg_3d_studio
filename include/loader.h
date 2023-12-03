#pragma once

#include <glm/glm.hpp>
#include <cstring>

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
        bool parseFile(string, string);
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

        // Object information.
        size_t numberOfShapes = 0;
        int numberOfVertices = 0;
        int numberOfFaces = 0;
        int numberOfIndices = 0;
        int numberOfNormals = 0;
        int numberOfTexCoords = 0;
        int numberOfColors = 0;

        char * outputString = new char[1];

        void appendString(const char* str) {
            // Calculate the new size of the buffer
            size_t newSize = strlen(outputString) + strlen(str) + 1;

            // Allocate a new buffer with the new size
            char* newBuffer = new char[newSize];

            // Copy the existing content and the new string to the new buffer
            strcpy(newBuffer, outputString);
            strcat(newBuffer, str);

            // Deallocate the old buffer and update the pointer
            delete[] outputString;
            outputString = newBuffer;
            
        }

        const char* getOutputString() const {
            return outputString;
        }

    protected:

    private:        
        // Searches a shapes vertices and finds the longest vector lenght
        float getLargestVertexLength(size_t);
        float calcVectorLength(glm::vec3);
};