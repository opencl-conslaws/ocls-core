#version 150

in vec3 position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out vec3 uvw;

void main() {
    uvw = (position+vec3(1.0))*0.5;
    gl_Position = projection_matrix*view_matrix*model_matrix*vec4(position, 1.0);
}
