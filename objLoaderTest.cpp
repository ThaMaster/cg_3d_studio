#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"
#include <iostream>

using namespace std;

int main() {
    string inputfile = "object_files/my_cube.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./object_files"; // Path to material files

    // Because using default config, the reader will triangulate the face vertices.
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        // If reader detects known error.
        if (!reader.Error().empty()) {
            cerr << "TinyObjReader: " << reader.Error();
        }
        // If reader is unable to parse the file.
        exit(1);
    }

    if (!reader.Warning().empty()) {
        cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        cout << "Faces and its vertices coordinates" << endl;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            cout << "--------| Vertex cooridnates for face " << f << " |--------" << endl;
            cout << endl << "Vertex coordinates: " << endl;
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];
                cout << "(" << vx << ", " << vy << ", " << vz << ")" << endl;
            }

            cout << endl << "Vertex normals: " << endl;
            // Check if `normal_index` is zero or positive. negative = no normal data
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    cout << "(" << nx << ", " << ny << ", " << nz << ")" << endl;
                } else {
                    cout << "(-,-,-)" << endl;
                }
            }
            
            cout << endl << "Vertex texture coordinates: " << endl;
            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    tinyobj::real_t tz = attrib.texcoords[2*size_t(idx.texcoord_index)+2];
                    cout << "(" << tx << ", " << ty << ", " << tz << ")" << endl;
                } else {
                    cout << "(-,-,-)" << endl;
                }
            }

            cout << endl << "Color values: " << endl;
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                cout << "(" << red << ", " << green << ", " << blue << ")" << endl;
                
            }
            // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
            // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
            // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
            cout << endl;
        }
    }
    cout << "--------| FILE INFO |--------" << endl;
    cout << "Num of face vertices: " << shapes[0].mesh.num_face_vertices.size() << endl;
    cout << "Num of vertices: " << attrib.vertices.size() << endl;
    cout << "Num of normals: " << attrib.normals.size() << endl;
    cout << "Num of texture coordinates: " << attrib.texcoords.size() << endl;
    cout << "Num of colors: " << attrib.colors.size() << endl;
}