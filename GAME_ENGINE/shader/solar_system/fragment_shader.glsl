#version 430 core

struct Material{
    int type;
    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //With texture
    sampler2D diffuse_texture;
    sampler2D specular_texture;
};

struct Light {
    int type;
    vec3 ambient;
    float pad0;
    vec3 diffuse;
    float pad1;
    vec3 specular;
    float pad2;
    //Positionned Light
    vec3  position;
    float pad3;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
    //Directed Light
    vec3  direction;
    float pad4;
    //SpotLight
    float inner_cut_off;
    float outer_cut_off; // if inner == outer then no smooth edge
};

//CONSTANT
uniform int LIGHT_TYPE_DIRECTIONAL;
uniform int LIGHT_TYPE_POINT;
uniform int LIGHT_TYPE_SPOT;

uniform int MATERIAL_TYPE_COLOR;
uniform int MATERIAL_TYPE_TEXTURE;

//IN
in vec2 uv;
in vec3 normal;
in vec3 fragment_pos;

//Lights
layout(std430, binding = 3) buffer lights_buffer
        {
                Light lights_from_buffer[];
        };
uniform int nb_lights;

//CAMERA_POS
uniform vec3 view_pos;

//MATRIX
uniform mat4 model_mat;

//DATA Object
uniform Material material;
uniform bool always_enlightened;

out vec3 color;


vec3 compute_phong(Light light,Material material,vec3 view_pos){
    //AMBIENT
    vec3 ambient = vec3(0,0,0);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);

    if(material.type == MATERIAL_TYPE_COLOR) {
        ambient = light.ambient *  material.ambient;
    } else if(material.type == MATERIAL_TYPE_TEXTURE) {
        ambient = light.ambient * texture(material.diffuse_texture, uv).rgb;
    }


    //DIFFUSE
    vec3 light_dir;
    if(light.type == LIGHT_TYPE_DIRECTIONAL){
        light_dir = normalize(-light.direction);
    } else if(light.type == LIGHT_TYPE_POINT || light.type == LIGHT_TYPE_SPOT){
        light_dir = normalize(light.position - fragment_pos);
    }
    float diff = max(dot(normal, light_dir), 0.0);
    if(material.type == MATERIAL_TYPE_COLOR) {
        diffuse = light.diffuse * diff * material.diffuse;
    } else  if(material.type == MATERIAL_TYPE_TEXTURE) {
        diffuse = light.diffuse * diff * texture(material.diffuse_texture, uv).rgb;
    }

    //SPECULAR

    if(material.shininess > 0){
        vec3 view_dir = normalize(view_pos - fragment_pos);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        if(material.type == MATERIAL_TYPE_COLOR) {
            specular = light.specular * spec * material.specular;
        } else  if(material.type == MATERIAL_TYPE_TEXTURE) {
            specular = light.specular * spec * texture(material.specular_texture, uv).rgb;
        }
    }


    //CUTOFF (with soft edges)
    if(light.type == LIGHT_TYPE_SPOT){
        float theta = dot(light_dir, normalize(-light.direction));
        float epsilon = (light.inner_cut_off - light.outer_cut_off);
        float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    }

    //ATTENUATION
    if(( light.type == LIGHT_TYPE_POINT || light.type == LIGHT_TYPE_SPOT ) && (light.constant_attenuation != 0 || light.linear_attenuation != 0 || light.quadratic_attenuation != 0) ){
        float distance    = length(light.position - fragment_pos);
        float attenuation = 1.0 / (light.constant_attenuation + light.linear_attenuation * distance + light.quadratic_attenuation * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    return ambient + diffuse + specular;
}
void main() {


    if(always_enlightened){
        if(material.type == MATERIAL_TYPE_COLOR) {
            color = material.diffuse;
        } else  if(material.type == MATERIAL_TYPE_TEXTURE) {
            color = texture(material.diffuse_texture, uv).rgb;
        }
    } else {
        color = vec3(0,0,0);
        for(int i = 0 ; i < nb_lights ; i ++){
            color += compute_phong(lights_from_buffer[i],material,view_pos);
        }
    }

}

