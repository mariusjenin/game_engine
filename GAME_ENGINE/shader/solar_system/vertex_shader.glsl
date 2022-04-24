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

// STAR ID CONST
uniform int SUN_ID;
uniform int EARTH_ID;
uniform int MOON_ID;

//HEIGHT MAP
uniform sampler2D hm_sun;
uniform sampler2D hm_earth;
uniform sampler2D hm_moon;

//Object attribute
uniform int star_id;



void main(){
    mat4 mvp = projection_mat * view_mat * model_mat;
    vec4 vertex_position_locally;
    uv = vertex_uv;
    normal = normalize(vec3(normal_model_mat*vec4(vertex_normal,0)));

    float value_hm;
    if(star_id == SUN_ID){
        value_hm = texture2D(hm_sun, vertex_uv).x /2;
    } else if(star_id == EARTH_ID){
        value_hm = texture2D(hm_earth, vertex_uv).x;
    } else if(star_id == MOON_ID){
        value_hm = texture2D(hm_moon, vertex_uv).x;
    }
    if(star_id == EARTH_ID){
        float height_water = 0.3;
        float height = texture2D(hm_earth, vertex_uv).x - height_water;
        //display only the part above the water (height_water : treshold)
        if(height > 0){
            vertex_position_locally = vec4(vertex_position + (vertex_normal * (value_hm - height_water) * 0.25),1);
        } else {
            vertex_position_locally = vec4(vertex_position ,1);
        }
    } else if(star_id == SUN_ID || star_id == MOON_ID) {
        vertex_position_locally = vec4(vertex_position + (vertex_normal * value_hm * 0.25),1);
    } else {
        vertex_position_locally = vec4(vertex_position ,1);
    }
    fragment_pos = vec3(model_mat*vertex_position_locally);
    gl_Position = mvp* vertex_position_locally;
}

