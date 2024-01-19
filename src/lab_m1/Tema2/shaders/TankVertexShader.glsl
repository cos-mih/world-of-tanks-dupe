#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int hp;

uniform vec3 obj_color;

// Output
out vec3 color;
out vec3 damage;

vec3 computeDeform(int HP)
{
    return -(3 - HP) * 0.1 * v_position * normalize(v_normal);
}

void main()
{
    vec3 deform = vec3(0);
    damage = vec3(0);
    if (hp >= 0)
    {
        deform = computeDeform(hp);
       damage = vec3(3) - hp;

    }
    vec3 v_pos = v_position + deform;

    vec3 world_pos = vec3 (Model * vec4(v_pos,1));
    vec3 N = normalize( mat3(Model) * v_normal );
    vec3 L = normalize( light_position - world_pos );
    vec3 V = normalize( eye_position - world_pos );
    vec3 R = reflect (-L, N);

    float ambient_light = 0.25;
    float diffuse_light = material_kd * max (dot(N,L), 0);
    float specular_light = 0;

    if (dot(N, L) > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    float light = ambient_light + diffuse_light + specular_light; 
  
    color = light * obj_color;

    gl_Position = Projection * View * Model * vec4(v_pos, 1.0);

}
