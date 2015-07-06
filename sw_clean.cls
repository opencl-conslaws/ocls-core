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


/**
 * Global Variables
 *
 * Variables defined outside any scope will be
 * constant and accessible from any function in this
 * script file.
 */
 float g = 9.81f;   //< gravity
 int top = 15;      //< top of gaussian func
 int bottom = 5;    //< bottom of gaussian func

/**
 * Initial function
 *
 */
float gauss(float x, float b, float c){
    return (((x-b)*(x-b))/2.0f*c*c);
}

float gaussian2d(void){
    float val = 0.0f;

    val += gauss(pos.x,0.0f,2.5f);
    val += gauss(pos.y,0.0f,2.5f);

    return ((float)(top-bottom))*exp(-val)+((float)bottom);
}

float dam_break(void){
    if(pos.x < 0.0f){
        return (float)top;
    }else{
        return (float)bottom;
    }
}

/**
 * Boundary function
 *
 * Function runs on every ghost cell.
 *
 * Boundary flag is true if we are processing ghost
 * cells on that boundary. For instance NORTH is TRUE if
 * we are processing boundary cells to the north (positive y)
 *
 */
void on_bounds(float* Q[3]){
    store(Q[0], fetch_mirror(Q[0], cell.xy), cell.xy);
    if(WEST && EAST)
        store2(&Q[1], -fetch2_mirror(&Q[1], cell.xy), cell.xy);
    else
        store2(&Q[1], fetch2_mirror(&Q[1], cell.xy), cell.xy);
}

float3  fflux(float3 Q){
    float u = Q.y/Q.x;
    return (float3)(Q.y, (Q.y*u)+(0.5f*g*Q.x*Q.x), Q.z*u);
}

float3  gflux(float3 Q){
    float v = Q.z/Q.x;
    return (float3)(Q.z, Q.y*v, (Q.z*v)+(0.5f*g*Q.x*Q.x));
}

float4 flux_y(integration_points3 R){
    float c, ap, am;
    c           = sqrt(g*R.south.s0);
    ap          = max((R.south.s2+c)/R.south.s0,0.0f);
    am          = min((R.south.s2-c)/R.south.s0,0.0f);
    c           = sqrt(g*R.north.s0);
    ap          = max((R.north.s2+c)/R.north.s0,ap);
    am          = min((R.north.s2-c)/R.north.s0,am);

    float3 Gp, Gm;
    float3 QSp = R.south_plus;
    float3 QNp = R.north_plus;
    float3 QSm = R.south_minus;
    float3 QNm = R.north_minus;

    Gp   = ((ap*gflux(QSp) - am*gflux(QNp)) + (ap*am)*(QNp-QSp))/(ap-am);
    Gm   = ((ap*gflux(QSm) - am*gflux(QNm)) + (ap*am)*(QNm-QSm))/(ap-am);

    return (float4)(mix(Gp, Gm, 0.5f),ap);
}

float4 flux_x(integration_points3 R){
    float c, ap, am;
    c           = sqrt(g*R.west.s0);
    ap          = max((R.west.s1+c)/R.west.s0,0.0f);
    am          = min((R.west.s1-c)/R.west.s0,0.0f);
    c           = sqrt(g*R.east.s0);
    ap          = max((R.east.s1+c)/R.east.s0,ap);
    am          = min((R.east.s1-c)/R.east.s0,am);

    float3 Fp, Fm;
    float3 QWp = R.east_plus;
    float3 QEp = R.west_plus;
    float3 QWm = R.east_minus;
    float3 QEm = R.west_minus;

    Fp   = ((ap*fflux(QWp) - am*fflux(QEp)) + (ap*am)*(QEp-QWp))/(ap-am);
    Fm   = ((ap*fflux(QWm) - am*fflux(QEm)) + (ap*am)*(QEm-QWm))/(ap-am);

    return (float4)(mix(Fp, Fm, 0.5f),ap);
}

float7 central_upwind_flux(float* Q[3]){
    integration_points3 R;
    //planar3(&Q[0], cell.xy, &R);
    R = Q;

    float4 F = flux_x(R);
    float4 G = flux_y(R);
    float E = max(F.w, G.w);

    return (float7)(F.s012, G.s012, E);
}

float6 dump_stencil_x(float* Q[3]){
    integration_points R;
    planar(Q[0], cell.xy, &R);

    float3 Q_0    = fetch3(&Q[0],cell.xy);

    return (float6)(Q_0.x,Q_0.x,Q_0.x,
                    Q_0.x,Q_0.x,Q_0.x);
    //return (float6)(R.west, R.west_plus, R.west_minus,
    //                R.east, R.east_plus, R.east_minus);
}

float6 dump_stencil_y(float* Q[3]){
    integration_points R;
    planar(Q[0], cell.xy, &R);

    float3 Q_0    = fetch3(&Q[0],cell.xy);

    return (float6)(Q_0.x,Q_0.x,Q_0.x,
                    Q_0.x,Q_0.x,Q_0.x);
    //return (float6)(R.north, R.north_plus, R.north_minus,
    //                R.south, R.south_plus, R.south_minus);
}

float3 forward_euler(float* Q[3], float* flux[7], float dt, float dx, float dy){
    float3 FE   = fetch3_offset(&flux[0],cell.xy,(int2)(0,0));
    float3 FW   = fetch3_offset(&flux[0],cell.xy,(int2)(-1,0));
    float3 GN   = fetch3_offset(&flux[3],cell.xy,(int2)(0,0));
    float3 GS   = fetch3_offset(&flux[3],cell.xy,(int2)(0,1));

    float3 L    = -((FE-FW)/dx+(GN-GS)/dy);
    float3 Q_0    = fetch3(&Q[0],cell.xy);

    return Q_0+dt*L;
}
