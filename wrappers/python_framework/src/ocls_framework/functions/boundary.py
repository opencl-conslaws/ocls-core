#
#  This file is part of the OpenCL-ConsLaws framework
#  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
