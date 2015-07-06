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

uniform sampler2D back_face;
uniform sampler2D front_face;
uniform sampler3D tex;
uniform vec2 thresholds = vec2(0.5,1.5);

in vec2 uv;
out vec4 color;

const float samples = 250.0;

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

vec4 rayCast(in vec3 front, in vec3 back){
    vec3 direction = back - front;
    int steps = int(floor(samples * length(direction)));
    if(steps == 0){
        return vec4(1.0);
    }

    vec3 diff = direction / float(steps);

    float value;
    vec4 result = vec4(0.0);

    float upperThreshold = thresholds.y;
    float threshold = thresholds.x+0.1;

    float scale = 1.0/(upperThreshold - threshold);

    const vec4 hsv1 = vec4(2.09, 1, 1, 1);
    const vec4 hsv2 = vec4(0.001, 1, 1, 1);

    vec3 uvw = front;
    for (int i=0; i<steps; i++) {
        value = texture(tex, uvw).x;
        uvw += diff;

        if (value >= threshold && value <=upperThreshold) {
            value = (value-threshold)*scale;
            vec4 texVal = mix(hsv1,hsv2,value);
            texVal.a = value;

            result.rgb += (1.0-result.a)*texVal.a*texVal.rgb;
            result.a += (1.0-result.a)*texVal.a;

            // Stop iterations if no longer transparent
            if (result.a >= 0.99) {
                i = steps;
                result.a = 1.0;
            }
        }
    }
    return HSV2RGB(result);
}

void main(){
    vec3 front = texture(front_face, uv).rgb;
    vec3 back = texture(back_face, uv).rgb;
    color = rayCast(front, back);
}