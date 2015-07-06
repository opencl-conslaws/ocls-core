#version 150
in vec3 uvw;
out vec4 color;
void main() {
    color = vec4(uvw, 1.0);
}
