#include <boost/numeric/ublas/assignment.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <cmath>
#include <chrono>

using namespace boost::numeric::ublas;

matrix<float> translate(float dx, float dy, float dz);
matrix<float> scale(float sx, float sy, float sz);
matrix<float> rotatez(float a);
matrix<float> rotatey(float a);
matrix<float> rotatex(float a);

int main () {
    matrix<float> m (4, 4);
    int i = 0;
    int j = 0;
    while(i < m.size1()) {
        m (i, j) = 1;
        i++;
        j++;
    }
    std::cout << translate(1,2,3) << std::endl;
    std::cout << scale(1,2,3) << std::endl;
    std::cout << rotatez(45 *(M_PI/180)) << std::endl;
    std::cout << rotatey(45 *(M_PI/180)) << std::endl;
    std::cout << rotatex(45 *(M_PI/180)) << std::endl;
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