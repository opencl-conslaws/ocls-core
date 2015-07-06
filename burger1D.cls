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