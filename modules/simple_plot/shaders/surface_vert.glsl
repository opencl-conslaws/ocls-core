#version 150

in vec2 position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

uniform sampler2D tex;
uniform float scale;

out vec2 uv;

void main() {
    uv = position;
    float h = texture(tex, uv).x*scale;
    gl_Position = projection_matrix*view_matrix*model_matrix*vec4(position.x, h, position.y, 1.0);
}
