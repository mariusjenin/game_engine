#version 330 core
//VERTEX DATA
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

//MVP
uniform mat4 model_mat;
uniform mat4 normal_model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

//OUT
out vec2 uv;
out vec3 normal;
out vec3 fragment_pos;

/* ########################################### */

//HEIGHT MAP
uniform sampler2D hm_land;

//Object attribute
uniform bool has_hm;
uniform bool is_node_on_top;
uniform float on_top_height_adjustement;
uniform vec3 on_top_position;
uniform vec2 on_top_uv;
uniform vec3 on_top_normal;
uniform mat4 on_top_model;

void main(){
    mat4 mv = projection_mat * view_mat ;
    uv = vertex_uv;
    normal = normalize(vec3(normal_model_mat*vec4(vertex_normal,0)));

    vec4 vertex_position_locally;
    float mult_height = -5;

    if(is_node_on_top){
        vertex_position_locally = model_mat * vec4(vertex_position,1);
        float height = texture2D(hm_land, on_top_uv).x;
        float height_object = (on_top_model * vec4(on_top_position + (on_top_normal * height * mult_height),1)).y + vertex_position.y + on_top_height_adjustement;
        vec4 vertex_computed = vec4(vertex_position_locally.x,height_object,vertex_position_locally.z,1);
        fragment_pos = vec3(vertex_computed);
        gl_Position = projection_mat * view_mat * vertex_computed;
    } else {
        if(has_hm){
            float height = texture2D(hm_land, vertex_uv).x;
            vertex_position_locally = model_mat * vec4(vertex_position + (vertex_normal * height * mult_height),1);
        } else {
            vertex_position_locally = model_mat * vec4(vertex_position,1);
        }

        fragment_pos = vec3(vertex_position_locally);
        gl_Position = mv* vertex_position_locally;
    }

}

