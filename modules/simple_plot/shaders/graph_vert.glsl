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
