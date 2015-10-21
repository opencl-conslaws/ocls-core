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



//float __OVERLOADABLE__ minmod(float a, float b){
float minmodAB(float a, float b){
    float res = min(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}
float2 minmodAB2(float2 a, float2 b){
    float2 res = min(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}
float3 minmodAB3(float3 a, float3 b){
    float3 res = min(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}
float4 minmodAB4(float4 a, float4 b){
    float4 res = min(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}

//float __OVERLOADABLE__ minmod(float a, float b, float c){
float minmodABC(float a, float b, float c){
    return 0.25f
            *sign(a)
            *(sign(a) + sign(b))
            *(sign(b) + sign(c))
            *min( min(fabs(a),fabs(b)), fabs(c) );
}

float2 minmodABC2(float2 a, float2 b, float2 c){
    return 0.25f
            *sign(a)
            *(sign(a) + sign(b))
            *(sign(b) + sign(c))
            *min( min(fabs(a),fabs(b)), fabs(c) );
}
float3 minmodABC3(float3 a, float3 b, float3 c){
    return 0.25f
            *sign(a)
            *(sign(a) + sign(b))
            *(sign(b) + sign(c))
            *min( min(fabs(a),fabs(b)), fabs(c) );
}
float4 minmodABC4(float4 a, float4 b, float4 c){
    return 0.25f
            *sign(a)
            *(sign(a) + sign(b))
            *(sign(b) + sign(c))
            *min( min(fabs(a),fabs(b)), fabs(c) );
}
float6 minmodABC6(float6 a, float6 b, float6 c){
    return 0.25f
            *sign(a)
            *(sign(a) + sign(b))
            *(sign(b) + sign(c))
            *min( min(fabs(a),fabs(b)), fabs(c) );
}

float maxmod(float a, float b){
    float res = max(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}

float2 maxmod2(float2 a, float2 b){
    float2 res = max(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}
float3 maxmod3(float3 a, float3 b){
    float3 res = max(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}
float4 maxmod4(float4 a, float4 b){
    float4 res = max(fabs(a),fabs(b));
    return res*(sign(a)+sign(b))*0.5f;
}

float minmod_slope(float Q, float Qneg, float Qpos){
    return minmodAB(Q-Qneg, Qpos-Q);
}

float superbee_slope(float Q, float Qneg, float Qpos){
    float a = minmodAB(Qpos-Q, 2.0f*(Q-Qneg));
    float b = minmodAB(2.0f*(Qpos-Q), Q-Qneg);
    return maxmod(a, b);
}

float mclimiter_slope(float Q, float Qneg, float Qpos){
    return minmodABC(Qpos-Q, 2.0f*(Q-Qneg), 2.0f*(Qpos-Q));
}
