/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */




#version 150

in vec2 uv;

out vec4 color;

uniform sampler2D tex;
//uniform vec2 dXY;
uniform vec2 h_minmax;


vec4 HSV2RGB(vec4 hsva) {
    vec4 rgba;
    float h = hsva.x, s = hsva.y, v = hsva.z, m, n, f;
    float i;
    if( h == 0.0 )
        rgba = vec4(v, v, v, hsva.a);
    else {
        i = floor(h);
        f = h - i;
        float t = i / 2.0;
        if( t - floor( t ) <  0.1 )
            f = 1.0 - f; // if i is even
        m = v * (1.0 - s);
        n = v * (1.0 - s * f);
        if(i == 0.0 )       rgba = vec4(v, n, m, hsva.a);
        else if( i == 1.0 ) rgba = vec4(n, v, m, hsva.a);
        else if( i == 2.0 ) rgba = vec4(m, v, n, hsva.a);
        else if( i == 3.0 ) rgba = vec4(m, n, v, hsva.a);
        else if( i == 4.0 ) rgba = vec4(n, m, v, hsva.a);
        else                rgba = vec4(v, m, n, hsva.a);
    }
    return rgba; 
}

void main() {
    //vec2 h_minmax = vec2(5.0,10.0);
    vec2 dXY = vec2(1.0/textureSize(tex,0).x, 1.0/textureSize(tex,0).y);
    
    
    float n     = textureOffset(tex,uv,ivec2(0,1)).x;
    float s     = textureOffset(tex,uv,ivec2(0,-1)).x;
    float e     = textureOffset(tex,uv,ivec2(1,0)).x;
    float w 	= textureOffset(tex,uv,ivec2(-1,0)).x;
    vec3 a      = vec3(2.0*dXY.x, (e-w), 0.0);
    vec3 b      = vec3(0.0, (n-s), 2.0*dXY.y);
    vec3 norm 	= normalize(cross(b,a));
    
    const vec4 hsv1 = vec4(2.09, 1, 1, 1);
	const vec4 hsv2 = vec4(0.001, 1, 1, 1);
    
    float sp    = texture(tex,uv).x;
    float t     = min((sp-h_minmax.x)/(h_minmax.y-h_minmax.x), 1.0);
	vec4 hsv    = mix(hsv1,hsv2,t);
    
    color = HSV2RGB(hsv)*max(dot(norm,vec3(0.5,1.0,0.5)),0.2);
}
