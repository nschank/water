#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;
uniform mat3 normal_matrix;

out vec3 pos;
out vec3 norm;
out mat4 view;
out vec3 vertexToEye;

out vec3 out_normal;

void main()
{
    gl_Position = p*v*m * vec4(position, 1.0f);
    pos = vec3(m * vec4(position, 1.0f));
    norm = normalize(normal_matrix * normalize(normal));
    view = v;
    vertexToEye = -normalize(pos);
}
