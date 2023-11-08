#include <cmath>

#define TINYOBJLOADER_IMPLEMENTATION // Defined exactly once!
#include "tiny_obj_loader.h"

//          Copyright Joe Coder 2004 - 2006.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)
#include <glm/glm>

using namespace boost::numeric::ublas;

matrix<float> translate(float dx, float dy, float dz);
matrix<float> scale(float sx, float sy, float sz);
matrix<float> rotatez(float a);
matrix<float> rotatey(float a);
matrix<float> rotatex(float a);
matrix<float> shear(float a, char on, char from);

int main () {
    std::cout << "Translation: " << translate(1,2,3) << std::endl;
    std::cout << "Scale: " << scale(1,2,3) << std::endl;
    std::cout << "Rotate-Z: " << rotatez(45 *(M_PI/180)) << std::endl;
    std::cout << "Rotate-Y: " << rotatey(45 *(M_PI/180)) << std::endl;
    std::cout << "Rotate-X: " << rotatex(45 *(M_PI/180)) << std::endl;
    std::cout << "Sheer-XZ: " << shear(45 *(M_PI/180), 'x', 'z') << std::endl;
    std::cout << "Prod of translation * scale: " << prod(translate(1,2,3), scale(1,2,3)) << std::endl;
    std::cout << "Prod of scale * translation: " << prod(scale(1,2,3), translate(1,2,3)) << std::endl;    
    return 0;
}

matrix<float> translate(float dx, float dy, float dz) {
    matrix<float> t_matrix (4,4);
    t_matrix <<=
        1, 0, 0, dx,
        0, 1, 0, dy,
        0, 0, 1, dz,
        0, 0, 0, 1;
    return t_matrix;
}

/* By using the assigning operator (<<=) the time to create the matrix went from 65 ms to 9 ms. */
matrix<float> scale(float sx, float sy, float sz) {
    matrix<float> s_matrix (4,4); 
    s_matrix <<=
        sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, sz, 0,
        0, 0, 0, 1;
    return s_matrix;
}

matrix<float> rotatez(float a) {
    matrix<float> r_matrix (4,4);
    r_matrix <<=
        cos(a), -sin(a), 0, 0,
        sin(a), cos(a), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
    return r_matrix;
}
matrix<float> rotatey(float a) {
    matrix<float> r_matrix (4,4);
    r_matrix <<=
        cos(a), 0, -sin(a), 0,
        0, 1, 0, 0,
        sin(a), 0, cos(a), 0,
        0, 0, 0, 1;
    return r_matrix;
}
matrix<float> rotatex(float a) {
    matrix<float> r_matrix (4,4);
    r_matrix <<=
        1, 0, 0, 0,
        0, cos(a), -sin(a), 0,
        0, sin(a), cos(a), 0,
        0, 0, 0, 1;
    return r_matrix;
}

matrix<float> shear(float a, char on, char from) {
    matrix<float> sh_matrix (4,4);
    sh_matrix <<=
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
    switch (on) {
        case 'x':
            if(from == 'y') {
                sh_matrix.insert_element(0, 1, tan(a));
            } else if(from =='z') {
                sh_matrix.insert_element(0, 2, tan(a));
            }
        break;
        case 'y':
            if(from == 'x') {
                sh_matrix.insert_element(1, 0, tan(a));
            } else if(from =='z') {
                sh_matrix.insert_element(1, 2, tan(a));
            }
        break;
        case 'z':
            if(from == 'x') {
                sh_matrix.insert_element(2, 0, tan(a));
            } else if(from =='y') {
                sh_matrix.insert_element(2, 1, tan(a));
            }
        break;
    }
    return sh_matrix;
}