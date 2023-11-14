#pragma once

#include <glm/glm.hpp>
#include "tiny_obj_loader.h"

using namespace std;

class Loader 
{   
    public:
        bool parseFile(string);
        void normalizeVertexCoords();
        void clearLoader();

        string loadedFileName;

        vector<vector<glm::vec3>> vertexCoords;
        vector<vector<unsigned int>> indices;
        vector<vector<glm::vec3>> vertexNormals;
        vector<vector<glm::vec3>> textureCoords;
        vector<vector<glm::vec3>> colorVals;

    protected:

    private:
        float getLargestVertexLength(size_t);
};