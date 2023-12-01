/*
 *  Approximate a sphere using subdivision of a tetrahedron.
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 *
 *  Code inspired by Example 1, Ch 5, Interactive Computer Graphics
 *  by E. Angel and D. Shreiner.
 */
#include "sphere.h"

Sphere::Sphere()
{
    // Reserve memory for n=4
    vsphere.reserve(1024);
    isphere.reserve(3100);
}

Vec4 Sphere::unit(const Vec4 v)
{
    Vec4 vnew;
    float d = v.toVector3D().length();

    if(d > 0.0f) {
        vnew = Vec4(v.x()/d,v.y()/d,v.z()/d, ++lastindex);
    } else {
        vnew = Vec4(0.0f,0.0f,0.0f, ++lastindex);
    }

    vsphere.push_back(vnew.toVector3D());

    // Since the sphere is centered around origio the normals
    // are pointing in the direction of the vertex
    nsphere.push_back(vnew.toVector3D().normalized());

    return vnew;
}

void Sphere::triangle(Vec4  a, Vec4 b, Vec4 c)
{
    isphere.push_back(static_cast<unsigned int>(a.w()));
    isphere.push_back(static_cast<unsigned int>(b.w()));
    isphere.push_back(static_cast<unsigned int>(c.w()));
}


void Sphere::divide_triangle(Vec4 a, Vec4 b, Vec4 c, int n)
{
    Vec4 v1, v2, v3;

    if(n>0) {
        v1 = unit(a + b);
        v2 = unit(a + c);
        v3 = unit(b + c);
        divide_triangle(a ,v2, v1, n-1);
        divide_triangle(c ,v3, v2, n-1);
        divide_triangle(b ,v1, v3, n-1);
        divide_triangle(v1 ,v2, v3, n-1);
    } else {
        triangle(a, b, c);
    }
}

/*
 * Create a sphere by subdividing the faces of a tetrahedron n times.
 */
void Sphere::make_sphere(const int n)
{
    vsphere.push_back(v[0].toVector3D());
    vsphere.push_back(v[1].toVector3D());
    vsphere.push_back(v[2].toVector3D());
    vsphere.push_back(v[3].toVector3D());
    nsphere.push_back(v[0].toVector3D().normalized());
    nsphere.push_back(v[1].toVector3D().normalized());
    nsphere.push_back(v[2].toVector3D().normalized());
    nsphere.push_back(v[3].toVector3D().normalized());

    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[3], v[2], n);
}

