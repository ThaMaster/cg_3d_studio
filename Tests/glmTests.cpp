#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace glm;

int main() {

    mat4 id_matrix(1.0f);

    mat4x4 matModel(1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0);

    model = rotate(matModel, radians(20.0f), vec3(1,0,0));
    std::cout << to_string(model) << std::endl;

    return 0;
}