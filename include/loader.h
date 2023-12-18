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
        bool parseFile(string, string);
        void normalizeVertexCoords();
        void clearLoader();

        vector<Object> objects; //STORE ALL LOADED OBJECTS.

        // Object information.
        struct objectInfo {
            size_t nShapes = 0;
            int nVertices = 0;
            int nFaces = 0;
            int nIndices = 0;
            int nNormals = 0;
            int nTexCoords = 0;
            int nColors = 0;
            bool objectLoaded = false;
        } oInfo;

        char* outputString = new char[255];

        void appendString(std::string str) {
            const char* inputStr = str.c_str();
            if(inputStr == nullptr)
                return;
            size_t destLen = strlen(outputString);
            size_t strLen = strlen(inputStr);
            size_t newLen = destLen + strLen;
            char* newStr = new char[newLen + 1];

            strcpy(newStr, outputString);
            strcpy(newStr, inputStr);
            strcpy(outputString, newStr);
        }

        char* getOutputString() const {
            return outputString;
        }

    protected:

    private:        
        // Searches a shapes vertices and finds the longest vector lenght
        float getLargestVertexLength(size_t);
        float calcVectorLength(glm::vec3);
};