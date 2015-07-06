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
  
  
float gauss(float x, float b, float c){
    return (((x-b)*(x-b))/2.0f*c*c);
}

float gaussian(void){
    float val = 0.0f;
    val += gauss(pos.x,-0.8f,10.0f);
    return 10.0f*exp(-val)+2.0f;
}

void on_bounds(float* Q){
    store(Q, fetch_mirror(Q, cell.xy), cell.xy);
}

float flux(float q){
    return 0.5f*q*q;
}

float lax_friedrichs(float* Q, float dt, float dx, float dy){
    float q_im = fetch_offset(Q, cell.xy,(int2)(-1,0));
    float q_ip = fetch_offset(Q, cell.xy,(int2)(1,0));
    float q_jm = fetch_offset(Q, cell.xy,(int2)(0, -1));
    float q_jp = fetch_offset(Q, cell.xy,(int2)(0, 1));

    return 0.25f*(q_im+q_ip+q_jm+q_jp) -
        0.5f*(dt/dx)*(flux(q_ip)-flux(q_im)) -
        0.5f*(dt/dy)*(flux(q_jp)-flux(q_jm));
}