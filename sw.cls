/**
 * Variables
 */
 float g = 9.81f; //< gravity

/**
 * Initial functions
 */
float dam_break(void){
	if (pos.x > 0.0f)
	{
		return 5.0f;
	}
	else
	{
		return 2.0f;
	}
}

float test(void){
    //if(pos.x <= 1.0f && pos.x >= -1.0f &&
    //        pos.y <= 1.0f && pos.y >= -1.0f){
	    return (pos.y+pos.x+2.0f)*5.0f;
    //}
}

float test2(void){
    if(cell.y == 131 && cell.x == 131){
	    return 5.0f;
    }
    return 0.0f;
}

float gauss(float x, float b, float c){
    return (((x-b)*(x-b))/2.0f*c*c);
}

float gaussian1d(void){
    float val = 0.0f;

    val += gauss(pos.x,0.0f,2.5f);

    return 10.0f*exp(-val)+5.0f;
}

float gaussian2d(void){
    float val = 0.0f;

    val += gauss(pos.x,0.0f,2.5f);
    val += gauss(pos.y,0.0f,2.5f);

    return 10.0f*exp(-val)+5.0f;
}

float gaussian3d(void){
    float val = 0.0f;
    float val2 = 0.0f;

    val += gauss(pos.x,-1.0f,2.5f);
    val += gauss(pos.y,-1.0f,2.5f);
    val += gauss(pos.z,-1.0f,2.5f);

    val2 += gauss(pos.x,1.0f,2.5f);
    val2 += gauss(pos.y,1.0f,2.5f);
    val2 += gauss(pos.z,1.0f,2.5f);

    return exp(-val)+exp(-val2);
}

/**
 * Boundary functions
 */
struct boundary_in
{
	float* h;
	float* hu;
	float* hv;
};

void on_bounds_pos_x(boundary_in in){
    //store(in.h, 0.0f, cell.x, cell.y, 0, 0);
    //store(in.h, 0.0f, cell.x, cell.y, -1, 0);

    // Ghost cells h
	store(in.h, fetch(in.h, cell.x, cell.y, 1, 0), cell.x, cell.y, 0, 0);
	store(in.h, fetch(in.h, cell.x, cell.y, 2, 0), cell.x, cell.y, -1, 0);

	// Ghost cells hu (flip vectors)
	store(in.hu, -fetch(in.hu, cell.x, cell.y, 1, 0), cell.x, cell.y, 0, 0);
    store(in.hu, -fetch(in.hu, cell.x, cell.y, 2, 0), cell.x, cell.y, -1, 0);

    // Ghost cells hv
    store(in.hv, fetch(in.hv, cell.x, cell.y, 1, 0), cell.x, cell.y, 0, 0);
    store(in.hv, fetch(in.hv, cell.x, cell.y, 2, 0), cell.x, cell.y, -1, 0);
}
void on_bounds_pos_y(boundary_in in){
    //store(in.h, 0.0f, cell.x, cell.y, 0, 0);
    //store(in.h, 0.0f, cell.x, cell.y, 0, -1);

    // Ghost cells h
	store(in.h, fetch(in.h, cell.x, cell.y, 0, 1), cell.x, cell.y, 0, 0);
	store(in.h, fetch(in.h, cell.x, cell.y, 0, 2), cell.x, cell.y, 0, -1);

	// Ghost cells hu (flip vectors)
	store(in.hu, fetch(in.hu, cell.x, cell.y, 0, 1), cell.x, cell.y, 0, 0);
    store(in.hu, fetch(in.hu, cell.x, cell.y, 0, 2), cell.x, cell.y, 0, -1);

    // Ghost cells hv
    store(in.hv, -fetch(in.hv, cell.x, cell.y, 0, 1), cell.x, cell.y, 0, 0);
    store(in.hv, -fetch(in.hv, cell.x, cell.y, 0, 2), cell.x, cell.y, 0, -1);
}
void on_bounds_neg_x(boundary_in in){
    //store(in.h, -20.0f, cell.x, cell.y, 0, 0);
    //store(in.h, 20.0f, cell.x, cell.y, 1, 0);

    // Ghost cells h
	store(in.h, fetch(in.h, cell.x, cell.y, -1, 0), cell.x, cell.y, 0, 0);
	store(in.h, fetch(in.h, cell.x, cell.y, -2, 0), cell.x, cell.y, 1, 0);

	// Ghost cells hu (flip vectors)
	store(in.hu, -fetch(in.hu, cell.x, cell.y, -1, 0), cell.x, cell.y, 0, 0);
    store(in.hu, -fetch(in.hu, cell.x, cell.y, -2, 0), cell.x, cell.y, 1, 0);

    // Ghost cells hv
    store(in.hv, fetch(in.hv, cell.x, cell.y, -1, 0), cell.x, cell.y, 0, 0);
    store(in.hv, fetch(in.hv, cell.x, cell.y, -2, 0), cell.x, cell.y, 1, 0);
}
void on_bounds_neg_y(boundary_in in){
    //store(in.h, 20.0f, cell.x, cell.y, 0, 0);
    //store(in.h, 20.0f, cell.x, cell.y, 0, 1);

	// Ghost cells h
    store(in.h, fetch(in.h, cell.x, cell.y, 0, -1), cell.x, cell.y, 0, 0);
    store(in.h, fetch(in.h, cell.x, cell.y, 0, -2), cell.x, cell.y, 0, 1);

    // Ghost cells hu (flip vectors)
    store(in.hu, fetch(in.hu, cell.x, cell.y, 0, -1), cell.x, cell.y, 0, 0);
    store(in.hu, fetch(in.hu, cell.x, cell.y, 0, -2), cell.x, cell.y, 0, 1);

    // Ghost cells hv
    store(in.hv, -fetch(in.hv, cell.x, cell.y, 0, -1), cell.x, cell.y, 0, 0);
    store(in.hv, -fetch(in.hv, cell.x, cell.y, 0, -2), cell.x, cell.y, 0, 1);
}


/**
 * Reconstruction step
 */
struct recon_in
{
	float* h;
    float* hu;
    float* hv;
};

struct flux_in
{
	float3 QW;
    float3 QWp;
    float3 QWm;
   
    float3 QE;
    float3 QEp;
    float3 QEm;

    float3 QS;
    float3 QSp;
    float3 QSm;
    
    float3 QN;
    float3 QNp;
    float3 QNm;
};

struct evaluator_in
{
	float* h[2];
    float* hu[2];
    float* hv[2];

	float* F_h;
    float* F_hu;
    float* F_hv;

    float* F_h;
    float* G_hu;
    float* G_hv;

	float dt;
};

flux_in bilinear_reconstruction(recon_in in){
	// Find integration points
	const float k = 0.2886751346f;

	float3 Q    = fetch(in.Q,x,y,1,1);
    float3 QE   = fetch(in.Q,x+1,y,1,1);
    float3 QW   = fetch(in.Q,x-1,y,1,1);
    float3 QN   = fetch(in.Q,x,y+1,1,1);
    float3 QS   = fetch(in.Q,x,y-1,1,1);

    float3 Sx 	= minmod(Q-QW,QE-Q);
    float3 Sy 	= minmod(Q-QS,QN-Q);

    flux_in out;
    out.Q = in.Q;

    out.QW   = Q + Sx*0.5f;
    out.QWp  = Q + Sx*0.5f + Sy*k;
    out.QWm  = Q + Sx*0.5f - Sy*k;

    out.QS   = Q + Sy*0.5f;
    out.QSp  = Q + Sy*0.5f + Sx*k;
    out.QSm  = Q + Sy*0.5f - Sx*k;

    float3 QNW = fetch(in.Q,x-1,y+1,1,1);
    float3 QNN = fetch(in.Q,x,y+2,1,1);
    float3 QNE = fetch(in.Q,x+1,y+1,1,1);
    float3 QEE = fetch(in.Q,x+2,y,1,1);
    float3 QSE = fetch(in.Q,x+1,y-1,1,1);

    Sx 	= minmod(QE-Q,QEE-QE);
    Sy 	= minmod(QE-QSE,QNE-QE);

    out.QE   = QE - Sx*0.5f;
    out.QEp  = QE - Sx*0.5f + Sy*k;
    out.QEm  = QE - Sx*0.5f - Sy*k;

    Sx 	= minmod(QN-QNW,QNE-QN);
    Sy 	= minmod(QN-Q,QNN-QN);

    out.QN   = QN - Sy*0.5f;
    out.QNp  = QN - Sy*0.5f + Sx*k;
    out.QNm  = QN - Sy*0.5f - Sx*k;
}

/*
 * Evaluate flux
 */
float3 fflux(float3 Q){
    if(Q.x <= 1.19e-07f){
        return (float3)(0.0f, 0.0f, 0.0f);
    }
    
    float k = 1e-1f*max(1.0f,min(1.0f/(float)(Nx),1.0f/(float)(Ny)));
    float u = 0.0f;
    if(Q.x < k){
        u = (sqrt(2.0f)*Q.x*Q.y)/(sqrt(pow(Q.x,4.0f)+max(pow(Q.x,4.0f),k)));
    }else{
        u = Q.y/Q.x;
    }
    return (float3)(Q.y, (Q.y*u)+(0.5f*g*Q.x*Q.x), Q.z*u);
}

float4 gflux(float4 Q){
    if(Q.x <= 1.19e-07f){
        return (float3)(0.0f, 0.0f, 0.0f);
    }
    
    float k = 1e-1f*max(1.0f,min(1.0f/(float)(Nx),1.0f/(float)(Ny)));
    float v = 0.0f;
    if(Q.x < k){
        v = (sqrt(2.0f)*Q.x*Q.z)/(sqrt(pow(Q.x,4.0f)+max(pow(Q.x,4.0f),k)));
    }else{
        v = Q.z/Q.x;
    }
    return (float3)(Q.z, Q.y*v, (Q.z*v)+(0.5f*g*Q.x*Q.x));
}

float xFlux(flux_in in, float3& F){
    float4 QW   = Q + Sx*0.5f;
    float4 QWp  = Q + Sx*0.5f + Sy*k;
    float4 QWm  = Q + Sx*0.5f - Sy*k;
    
    float4 QE   = Q1 - Sxp*0.5f;
    float4 QEp  = Q1 - Sxp*0.5f + Syp*k;
    float4 QEm  = Q1 - Sxp*0.5f - Syp*k;
    
    float c, ap, am;
    c           = sqrt(g*QW.x);
    ap          = max((QW.y+c)/QW.x,0.0f);
    am          = min((QW.y-c)/QW.x,0.0f);
    c           = sqrt(g*QE.x);
    ap          = max((QE.y+c)/QE.x,ap);
    am          = min((QE.y-c)/QE.x,am);
	
    float4 Fp   = ((ap*fflux(g, QWp) - am*fflux(g, QEp)) + (ap*am)*(QEp-QWp))/(ap-am);
    float4 Fm   = ((ap*fflux(g, QWm) - am*fflux(g, QEm)) + (ap*am)*(QEm-QWm))/(ap-am);
    
    F = mix(Fp, Fm, 0.5f);
    
    return ap;
}

float yFlux(flux_in in, float3& G){
    float4 QS   = Q + Sy*0.5f;
    float4 QSp  = Q + Sy*0.5f + Sx*k;
    float4 QSm  = Q + Sy*0.5f - Sx*k;
    
    float4 QN   = Q1 - Syp*0.5f;
    float4 QNp  = Q1 - Syp*0.5f + Sxp*k;
    float4 QNm  = Q1 - Syp*0.5f - Sxp*k;
    
    float c, ap, am;
    c           = sqrt(g*QS.x);
    ap          = max((QS.z+c)/QS.x,0.0f);
    am          = min((QS.z-c)/QS.x,0.0f);
    c           = sqrt(g*QN.x);
    ap          = max((QN.z+c)/QN.x,ap);
    am          = min((QN.z-c)/QN.x,am);
    
    float4 Gp   = ((ap*gflux(g, QSp) - am*gflux(g, QNp)) + (ap*am)*(QNp-QSp))/(ap-am);
    float4 Gm   = ((ap*gflux(g, QSm) - am*gflux(g, QNm)) + (ap*am)*(QNm-QSm))/(ap-am);
    G = mix(Gp, Gm, 0.5f);

  	return ap;
}

evolver_in central_upwind_flux(flux_in in){
	// Evaluate flux F, G and compute eigenvalues E
	evolver_in out;
	float maxE = max(xflux(in, out.F), yFlux(in, out.G));
	out.Q = in.Q;

	out.dt = compute_dt(maxE);
}

/**
 * Evolve solution
 */
float3* runge_kutta(evolver_in in){
	// Compute L and evolve solution
	float3 FE   = fetch(in.F,x,y,2,2);
    float3 FW   = fetch(in.F,x-1,y,2,2);
    float3 GN   = fetch(in.G,x,y,2,2);
    float3 GS   = fetch(in.G,x,y-1,2,2);
    
    float3 L    = -((FE-FW)/dx+(GN-GS)/dy);
    
    float3 Q  	= fetch(in.Q[substep],x,y,2,2);
    
    if (substep  == 0) {
    	return Q+dt*L;
    } else {
    	float3 Qk = fetch(in.Q[0],x,y,2,2);
    	return 0.5f*Q+0.5f*(Qk+dt*L)
    }
}