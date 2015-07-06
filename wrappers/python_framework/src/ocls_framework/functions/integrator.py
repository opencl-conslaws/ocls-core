from ocls_framework import *
from string import Template

def forward_euler_1D(framework, domain, num_eq=1):
    src = """
    float${num_eq} forward_euler(float* Q${ar_len}, float* flux${ar_len}, float dt, float dx){
        float${num_eq} q = fetch${num_eq}(Q, cell.x);
        float${num_eq} F_m = fetch${num_eq}_offset(flux, cell.x,-1);
        float${num_eq} F_p = fetch${num_eq}(flux, cell.x);

        return q - (dt/dx)*(F_p-F_m);
    }
    """
    src = Template(src)
    if num_eq is 1:
        src = src.substitute(ar_len='',num_eq='')
    else:
        src = src.substitute(ar_len='[{0}]'.format(num_eq), num_eq=num_eq)

    ocls_src = framework.loadFromString(src)
    return framework.createIntegratorFunction(domain, ocls_src, 'forward_euler')
