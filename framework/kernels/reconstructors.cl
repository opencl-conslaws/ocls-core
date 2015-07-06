
void linear(__global float* data, uint cell, integration_points* out){
    // NOT IMPLEMENTED!
}

void linear2(__global float** data, uint cell, integration_points2* out){
    // NOT IMPLEMENTED!
}
void linear3(__global float** data, uint cell, integration_points3* out){
    // NOT IMPLEMENTED!
}
void linear4(__global float** data, uint cell, integration_points4* out){
    // NOT IMPLEMENTED!
}

void planar(__global float* data, uint2 cell, integration_points* out){
#if (_DIMENSIONS_ == 2)
    const float k = 0.2886751346f;

    float q    = fetch(data,cell);
    float qe   = fetch_offset(data,cell,(int2)(1,0));
    float qw   = fetch_offset(data,cell,(int2)(-1,0));
    float qn   = fetch_offset(data,cell,(int2)(0,-1));
    float qs   = fetch_offset(data,cell,(int2)(0,1));

    float Sx 	= minmodAB(q-qw,qe-q);
    float Sy 	= minmodAB(q-qs,qn-q);

    out->west   = q + Sx*0.5f;
    out->west_plus  = q + Sx*0.5f + Sy*k;
    out->west_minus  = q + Sx*0.5f - Sy*k;

    out->south   = q + Sy*0.5f;
    out->south_plus  = q + Sy*0.5f + Sx*k;
    out->south_minus  = q + Sy*0.5f - Sx*k;

    float qnw = fetch_offset(data,cell,(int2)(-1,-1));
    float qnn = fetch_offset(data,cell,(int2)(0,-2));
    float qne = fetch_offset(data,cell,(int2)(1,-1));
    float qee = fetch_offset(data,cell,(int2)(2,0));
    float qse = fetch_offset(data,cell,(int2)(1,1));

    Sx 	= minmodAB(qe-q,qee-qe);
    Sy 	= minmodAB(qe-qse,qne-qe);

    out->east   = qe - Sx*0.5f;
    out->east_plus  = qe - Sx*0.5f + Sy*k;
    out->east_minus  = qe - Sx*0.5f - Sy*k;

    Sx 	= minmodAB(qn-qnw,qne-qn);
    Sy 	= minmodAB(qn-q,qnn-qn);

    out->north   = qn - Sy*0.5f;
    out->north_plus  = qn - Sy*0.5f + Sx*k;
    out->north_minus  = qn - Sy*0.5f - Sx*k;
#endif
}

void planar2(__global float** data, uint2 cell, integration_points2* out){
#if (_DIMENSIONS_ == 2)
    const float k = 0.2886751346f;
    
    float2 q    = fetch2(data,cell);
    float2 qe   = fetch2_offset(data,cell,(int2)(1,0));
    float2 qw   = fetch2_offset(data,cell,(int2)(-1,0));
    float2 qn   = fetch2_offset(data,cell,(int2)(0,-1));
    float2 qs   = fetch2_offset(data,cell,(int2)(0,1));
    
    float2 Sx 	= minmodAB2(q-qw,qe-q);
    float2 Sy 	= minmodAB2(q-qs,qn-q);
    
    out->west   = q + Sx*0.5f;
    out->west_plus  = q + Sx*0.5f + Sy*k;
    out->west_minus  = q + Sx*0.5f - Sy*k;
    
    out->south   = q + Sy*0.5f;
    out->south_plus  = q + Sy*0.5f + Sx*k;
    out->south_minus  = q + Sy*0.5f - Sx*k;
    
    float2 qnw = fetch2_offset(data,cell,(int2)(-1,-1));
    float2 qnn = fetch2_offset(data,cell,(int2)(0,-2));
    float2 qne = fetch2_offset(data,cell,(int2)(1,-1));
    float2 qee = fetch2_offset(data,cell,(int2)(2,0));
    float2 qse = fetch2_offset(data,cell,(int2)(1,1));
    
    Sx 	= minmodAB2(qe-q,qee-qe);
    Sy 	= minmodAB2(qe-qse,qne-qe);
    
    out->east   = qe - Sx*0.5f;
    out->east_plus  = qe - Sx*0.5f + Sy*k;
    out->east_minus  = qe - Sx*0.5f - Sy*k;
    
    Sx 	= minmodAB2(qn-qnw,qne-qn);
    Sy 	= minmodAB2(qn-q,qnn-qn);
    
    out->north   = qn - Sy*0.5f;
    out->north_plus  = qn - Sy*0.5f + Sx*k;
    out->north_minus  = qn - Sy*0.5f - Sx*k;
#endif
}
void planar3(__global float** data, uint2 cell, integration_points3* out){
#if (_DIMENSIONS_ == 2)
    const float k = 0.2886751346f;
    
    float3 q    = fetch3(data,cell);
    float3 qe   = fetch3_offset(data,cell,(int2)(1,0));
    float3 qw   = fetch3_offset(data,cell,(int2)(-1,0));
    float3 qn   = fetch3_offset(data,cell,(int2)(0,-1));
    float3 qs   = fetch3_offset(data,cell,(int2)(0,1));
    
    float3 Sx 	= minmodAB3(q-qw,qe-q);
    float3 Sy 	= minmodAB3(q-qs,qn-q);
    
    out->west   = q + Sx*0.5f;
    out->west_plus  = q + Sx*0.5f + Sy*k;
    out->west_minus  = q + Sx*0.5f - Sy*k;
    
    out->south   = q + Sy*0.5f;
    out->south_plus  = q + Sy*0.5f + Sx*k;
    out->south_minus  = q + Sy*0.5f - Sx*k;
    
    float3 qnw = fetch3_offset(data,cell,(int2)(-1,-1));
    float3 qnn = fetch3_offset(data,cell,(int2)(0,-2));
    float3 qne = fetch3_offset(data,cell,(int2)(1,-1));
    float3 qee = fetch3_offset(data,cell,(int2)(2,0));
    float3 qse = fetch3_offset(data,cell,(int2)(1,1));
    
    Sx 	= minmodAB3(qe-q,qee-qe);
    Sy 	= minmodAB3(qe-qse,qne-qe);
    
    out->east   = qe - Sx*0.5f;
    out->east_plus  = qe - Sx*0.5f + Sy*k;
    out->east_minus  = qe - Sx*0.5f - Sy*k;
    
    Sx 	= minmodAB3(qn-qnw,qne-qn);
    Sy 	= minmodAB3(qn-q,qnn-qn);
    
    out->north   = qn - Sy*0.5f;
    out->north_plus  = qn - Sy*0.5f + Sx*k;
    out->north_minus  = qn - Sy*0.5f - Sx*k;
#endif
}
void planar4(__global float** data, uint2 cell, integration_points4* out){
#if (_DIMENSIONS_ == 2)
    const float k = 0.2886751346f;
    
    float4 q    = fetch4(data,cell);
    float4 qe   = fetch4_offset(data,cell,(int2)(1,0));
    float4 qw   = fetch4_offset(data,cell,(int2)(-1,0));
    float4 qn   = fetch4_offset(data,cell,(int2)(0,-1));
    float4 qs   = fetch4_offset(data,cell,(int2)(0,1));
    
    float4 Sx 	= minmodAB4(q-qw,qe-q);
    float4 Sy 	= minmodAB4(q-qs,qn-q);
    
    out->west   = q + Sx*0.5f;
    out->west_plus  = q + Sx*0.5f + Sy*k;
    out->west_minus  = q + Sx*0.5f - Sy*k;
    
    out->south   = q + Sy*0.5f;
    out->south_plus  = q + Sy*0.5f + Sx*k;
    out->south_minus  = q + Sy*0.5f - Sx*k;
    
    float4 qnw = fetch4_offset(data,cell,(int2)(-1,-1));
    float4 qnn = fetch4_offset(data,cell,(int2)(0,-2));
    float4 qne = fetch4_offset(data,cell,(int2)(1,-1));
    float4 qee = fetch4_offset(data,cell,(int2)(2,0));
    float4 qse = fetch4_offset(data,cell,(int2)(1,1));
    
    Sx 	= minmodAB4(qe-q,qee-qe);
    Sy 	= minmodAB4(qe-qse,qne-qe);
    
    out->east   = qe - Sx*0.5f;
    out->east_plus  = qe - Sx*0.5f + Sy*k;
    out->east_minus  = qe - Sx*0.5f - Sy*k;
    
    Sx 	= minmodAB4(qn-qnw,qne-qn);
    Sy 	= minmodAB4(qn-q,qnn-qn);
    
    out->north   = qn - Sy*0.5f;
    out->north_plus  = qn - Sy*0.5f + Sx*k;
    out->north_minus  = qn - Sy*0.5f - Sx*k;
#endif
}

void cubic(__global float* data, uint3 cell, integration_points* out){
    // NOT IMPLEMENTED!
}
void cubic2(__global float** data, uint3 cell, integration_points2* out){
    // NOT IMPLEMENTED!
}
void cubic3(__global float** data, uint3 cell, integration_points3* out){
    // NOT IMPLEMENTED!
}
void cubic4(__global float** data, uint3 cell, integration_points4* out){
    // NOT IMPLEMENTED!
}