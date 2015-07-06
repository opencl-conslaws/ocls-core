from ocls_framework import *
from string import Template

def periodic_1D(framework, domain, num_eq=1):
    src = """
    void periodic(float* Q${ar_len}){
        if(LEFT)
            store${num_eq}(Q, fetch${num_eq}_offset(Q, cell.x, Nx), cell.x);
        else
            store${num_eq}(Q, fetch${num_eq}_offset(Q, cell.x, -Nx), cell.x);
    }
    """
    src = Template(src)
    if num_eq is 1:
        src = src.substitute(ar_len='',num_eq='')
    else:
        src = src.substitute(ar_len='[{0}]'.format(num_eq), num_eq=num_eq)

    ocls_src = framework.loadFromString(src)
    return framework.createBoundaryFunction(domain, ocls_src, 'periodic')

def absorbing_1D(framework, domain, num_eq=1):
    src = """
    void absorbing(float* Q${ar_len}){
        store${num_eq}(Q, fetch${num_eq}_mirror(Q, cell.x), cell.x);
    }
    """
    src = Template(src)
    if num_eq is 1:
        src = src.substitute(ar_len='',num_eq='')
    else:
        src = src.substitute(ar_len='[{0}]'.format(num_eq), num_eq=num_eq)

    ocls_src = framework.loadFromString(src)
    return framework.createBoundaryFunction(domain, ocls_src, 'absorbing')
