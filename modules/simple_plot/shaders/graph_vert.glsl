#version 150

in float position;

uniform sampler2D tex;
//uniform sampler1D tex;

uniform float scale;
uniform float center;

void main() {
    float u = (position+1.0)*0.5;
    float h = texture(tex, vec2(u,0.0)).x*scale;
    //float h = texture(tex, u).x*scale;

    gl_Position = vec4(position, h-center, 0.0, 1.0);
}
