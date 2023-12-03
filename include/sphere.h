/*
 *  Approximate a sphere using subdivision of a tetrahedron.
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 *
 *  Code inspired by Example 1, Ch 5, Interactive Computer Graphics
 *  by E. Angel and D. Shreiner, 2012.
 */
#pragma once

#include "3dstudio.h"

class Sphere
{
public:
    Sphere();
    void make_sphere(const int);

    inline const Vec3 *verticesData() const
        {return vsphere.data(); }
    inline const Vec3 *normalsData() const
        {return nsphere.data(); }
    inline const unsigned int *indicesData() const
        {return isphere.data(); }
    inline const std::vector<Vec3> vertices() const
        {return vsphere; }
    inline const std::vector<Vec3> normals() const
        {return nsphere; }
    inline const std::vector<unsigned int> indices() const
        {return isphere; }
    inline size_t sizeVertices() const
        {return vsphere.size()*sizeof(Vec3);}
    inline size_t sizeNormals() const
        {return nsphere.size()*sizeof(Vec3);}
    inline size_t sizeIndices() const
        {return isphere.size()*sizeof(unsigned int);}

private:
    // Four equally spaced points on the unit circle
    // Last value holds the index of the vertex
    std::vector<Vec4> v = {
        {0.0f, 0.0f, 1.0f, 0},
        {0.0f, 0.942809f, -0.333333f, 1},
        {-0.816497f, -0.471405f, -0.333333f, 2},
        {0.816497f, -0.471405f, -0.333333f, 3}};
    int lastindex = 3; // Current vertex index

    std::vector<Vec3> vsphere;
    std::vector<unsigned int> isphere;
    std::vector<Vec3> nsphere;

    Vec4 unit(const Vec4);
    void triangle(Vec4, Vec4, Vec4);
    void divide_triangle(Vec4, Vec4, Vec4, int);
};

