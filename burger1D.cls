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
    val += gauss(pos.x,-0.8f,10.5f);
    return 10.0f*exp(-val)+2.0f;
}

void on_bounds(float* Q){
    store(Q, fetch_mirror(Q, cell.x), cell.x);
}

float flux(float q){
    return 0.5f*q*q;
}

float lax_friedrichs(float* Q, float dt, float dx){
    float q_m = fetch_offset(Q, cell.x,-1);
    float q_p = fetch_offset(Q, cell.x, 1);

    return 0.5f*(q_m+q_p) - (dt/dx)*(flux(q_p)-flux(q_m));
}