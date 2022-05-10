//
// Created by mariusjenin on 01/03/2022.
//

#include "meshloader.hpp"

#include <utility>

MeshData::MeshData(std::vector<glm::vec3> vp,std::vector<unsigned short int> ti,std::vector<glm::vec2> vtc,std::vector<glm::vec3> vn){
    vertex_positions = std::move(vp);
    triangle_indices = std::move(ti);
    vertex_tex_coords = std::move(vtc);
    vertex_normals = std::move(vn);
}

MeshData create_plane(
        int nb_vertex_1,
        int nb_vertex_2,
        vec3 pos_vertex_start,
        vec3 pos_vertex_end,
        NormalDirection normal_dir
) {

    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<unsigned short int> indices;
    std::vector<vec2> uv;

    srand(time(nullptr));
    vec3 normal;
    bool is_inv_normal = normal_dir == X_INV_NORMAL_DIRECTION || normal_dir == Y_INV_NORMAL_DIRECTION|| normal_dir == Z_INV_NORMAL_DIRECTION;
    switch (normal_dir) {
        case X_NORMAL_DIRECTION:
            normal = {1,0,0};
            break;
        case Y_NORMAL_DIRECTION:
            normal = {0,1,0};
            break;
        case Z_NORMAL_DIRECTION:
            normal = {0,0,1};
            break;
        case X_INV_NORMAL_DIRECTION:
            normal = {-1,0,0};
            break;
        case Y_INV_NORMAL_DIRECTION:
            normal = {0,-1,0};
            break;
        case Z_INV_NORMAL_DIRECTION:
            normal = {0,0,-1};
            break;
    }
    float step_1, step_2;
    if (normal_dir == X_NORMAL_DIRECTION || normal_dir == X_INV_NORMAL_DIRECTION) {
        step_1 = (pos_vertex_end[1] - pos_vertex_start[1]) / (float)nb_vertex_1;
        step_2 = (pos_vertex_end[2] - pos_vertex_start[2]) / (float)nb_vertex_2;
    } else if (normal_dir == Y_NORMAL_DIRECTION || normal_dir == Y_INV_NORMAL_DIRECTION) {
        step_1 = (pos_vertex_end[0] - pos_vertex_start[0]) /(float) nb_vertex_1;
        step_2 = (pos_vertex_end[2] - pos_vertex_start[2]) / (float)nb_vertex_2;
    } else {
        step_1 = (pos_vertex_end[0] - pos_vertex_start[0]) / (float)nb_vertex_1;
        step_2 = (pos_vertex_end[1] - pos_vertex_start[1]) / (float)nb_vertex_2;
    }

    for (int i = 0; i < nb_vertex_1; i++) {
        for (int j = 0; j < nb_vertex_2; j++) {

            uv.emplace_back((float) i / (float) nb_vertex_1, (float) j / (float) nb_vertex_2);

            normals.push_back(normal);
            if (normal_dir == X_NORMAL_DIRECTION || normal_dir == X_INV_NORMAL_DIRECTION) {
                vertices.push_back(pos_vertex_start + vec3(0.f, (float)i * step_1, (float)j * step_2));
            } else if (normal_dir == Y_NORMAL_DIRECTION || normal_dir == Y_INV_NORMAL_DIRECTION) {
                vertices.push_back(pos_vertex_start + vec3((float)i * step_1, 0.f, (float)j * step_2));
            } else {
                vertices.push_back(pos_vertex_start + vec3((float)i * step_1, (float)j * step_2, 0.f));
            }
            if (i < nb_vertex_1 - 1 && j < nb_vertex_2 - 1) {
                indices.push_back(i * nb_vertex_2 + j);
                if(!is_inv_normal)indices.push_back((i + 1) * nb_vertex_2 + (j + 1));
                indices.push_back((i + 1) * nb_vertex_2 + j);
                if(is_inv_normal)indices.push_back((i + 1) * nb_vertex_2 + (j + 1));

                indices.push_back(i * nb_vertex_2 + j);
                if(!is_inv_normal)indices.push_back(i * nb_vertex_2 + j + 1);
                indices.push_back((i + 1) * nb_vertex_2 + (j + 1));
                if(is_inv_normal)indices.push_back(i * nb_vertex_2 + j + 1);
            }
        }
    }
    return {vertices, indices, uv, normals};
}


MeshData create_sphere(
        float radius, int slices, int stacks) {
    std::vector<vec3> vertices = std::vector<vec3>();
    std::vector<vec3> normals = std::vector<vec3>();
    std::vector<unsigned short int> indices = std::vector<unsigned short int>();
    std::vector<vec2> uv = std::vector<vec2>();

    float du = 2 * M_PI / slices;
    float dv = M_PI / stacks;
    int i, j;
    float u, v, x, y, z;
    for (i = 0; i <= stacks; i++) {
        v = (float) -M_PI / 2 + (float) i * dv;
        for (j = 0; j <= slices; j++) {
            u = j * du;
            x = cos(u) * cos(v);
            y = sin(u) * cos(v);
            z = sin(v);
            vertices.emplace_back(radius * x, radius * y, radius * z);
            normals.emplace_back(radius * x, radius * y, radius * z);
            uv.emplace_back((float) j / (float) slices, (float) i / (float) stacks);
        }
    }
    for (j = 0; j < stacks; j++) {
        int row1 = j * (slices + 1);
        int row2 = (j + 1) * (slices + 1);
        for (i = 0; i < slices; i++) {
            indices.push_back(row1 + i);
            indices.push_back(row2 + i + 1);
            indices.push_back(row2 + i);
            indices.push_back(row1 + i);
            indices.push_back(row1 + i + 1);
            indices.push_back(row2 + i + 1);
        }
    }
    return {vertices, indices, uv, normals};
}

MeshData create_rectangle_cuboid(glm::vec3 size){
    size/=2.0f;
    std::vector<glm::vec3> verticies  = {
            {size.x, size.y,size.z},
            {size.x, size.y,size.z},
            {size.x, size.y,size.z},
            {size.x,size.y,-size.z},
            {size.x,size.y,-size.z},
            {size.x,size.y,-size.z},
            {size.x,-size.y,size.z},
            {size.x,-size.y,size.z},
            {size.x,-size.y,size.z},
            {size.x,-size.y,-size.z},
            {size.x,-size.y,-size.z},
            {size.x,-size.y,-size.z},
            {-size.x,size.y,size.z},
            {-size.x,size.y,size.z},
            {-size.x,size.y,size.z},
            {-size.x,size.y,-size.z},
            {-size.x,size.y,-size.z},
            {-size.x,size.y,-size.z},
            {-size.x,-size.y,size.z},
            {-size.x,-size.y,size.z},
            {-size.x,-size.y,size.z},
            {-size.x,-size.y,-size.z},
            {-size.x,-size.y,-size.z},
            {-size.x,-size.y,-size.z},
    };
    std::vector<vec3> normals = {
            {0,1,0},
            {0,0,1},
            {1,0,0},

            {0,1,0},
            {1,0,0},
            {0,0,-1},

            {0,0,1},
            {1,0,0},
            {0,-1,0},

            {1,0,0},
            {0,-1,0},
            {0,0,-1},

            {0,1,0},
            {0,0,1},
            {-1,0,0},

            {0,1,0},
            {-1,0,0},
            {0,0,-1},

            {0,0,1},
            {-1,0,0},
            {0,-1,0},

            {-1,0,0},
            {0,-1,0},
            {0,0,-1},
    };
    for(int i = 0 ; i < 8;i++){
        verticies.emplace_back(normals[i][0] * size.x,normals[i][1] * size.y,normals[i][2] * size.z);
    }
    std::vector<vec2> uv = {
            {1.f/2.f,1.f/3.f},
            {1.f/2.f,1.f/3.f},
            {1.f/2.f,1.f/3.f},

            {1.f/2.f,2.f/3.f},
            {1.f/2.f,2.f/3.f},
            {1.f/2.f,2.f/3.f},

            {1.f/2.f,0},
            {3.f/4.f,1.f/3.f},
            {3.f/4.f,1.f/3.f},

            {3.f/4.f,2.f/3.f},
            {1.f/2.f,1.f},
            {3.f/4.f,2.f/3.f},

            {1.f/4.f,1.f/3.f},
            {1.f/4.f,1.f/3.f},
            {1.f/4.f,1.f/3.f},


            {1.f/4.f,2.f/3.f},
            {1.f/4.f,2.f/3.f},
            {1.f/4.f,2.f/3.f},


            {1.f/4.f,0.f},
            {0.f,1.f/3.f},
            {1.f,1.f/3.f},

            {0,2.f/3.f},
            {1.f,2.f/3.f},
            {1.f/4.f,1.f},
    };
    std::vector<unsigned short int> indices = {
            0,3,12,
            12,3,15,
            6,1,18,
            18,1,13,
            9,4,7,
            7,4,2,
            19,14,21,
            21,14,16,
            10,8,22,
            22,8,20,
            5,11,17,
            17,11,23,
    };
    return {verticies, indices, uv, normals};
}