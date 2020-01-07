#version 330
uniform mat4 M;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 NormalMatrix;
uniform vec4 a_parameter;
uniform vec4 Global_U;

in  vec4 in_position;
in  vec3 in_color;
in  vec3 in_normal;
in  vec2 in_UVs;

// We output the ex_color variable to the next shader in the chain
out vec3 ex_color;
out vec3 out_normal;
out vec4 position;
out vec4 a_Param_Transform;
out vec4 Global_U_out;
out vec2 UV;

void main(void) {

    gl_Position = projectionMatrix * viewMatrix * M * in_position;
    vec3 in_normal = normalize(mat3(NormalMatrix) * in_normal);

    // passing normalized vectors and their position transformations.
    ex_color = in_color;
    out_normal = in_normal;
    a_Param_Transform = normalize(NormalMatrix * a_parameter);
    Global_U_out = NormalMatrix * Global_U; 
    UV = in_UVs;
}