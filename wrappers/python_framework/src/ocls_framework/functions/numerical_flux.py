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

def lax_friedrich_1D(framework, domain, flux, num_eq=1):
    src = """
    float${num_eq} lax_friedrich(float* Q${ar_len}, float dt, float dx){
        float${num_eq} q = fetch${num_eq}(Q, cell.x);
        float${num_eq} q_p = fetch${num_eq}_offset(Q, cell.x, 1);

        return 0.5f*(${func}(q)+${func}(q_p)) - (dx/(2.0f*dt))*(q_p-q);
    }
    """
    src = Template(src)
    if num_eq is 1:
        src = src.substitute(ar_len='',num_eq='',func=flux[0])
    else:
        src = src.substitute(ar_len='[{0}]'.format(num_eq), num_eq=num_eq, func=flux[0])

    src += flux[1]
    ocls_src = framework.loadFromString(src)
    return framework.createFluxFunction(domain, ocls_src, 'lax_friedrich')

def lax_wendroff_1D(framework, domain, flux, num_eq=1):
    src = """
    float${num_eq} lax_wendroff(float* Q${ar_len}, float dt, float dx){
        float${num_eq} q = fetch${num_eq}(Q, cell.x);
        float${num_eq} q_p = fetch${num_eq}_offset(Q, cell.x, 1);

        return ${func}(0.5f*(q+q_p) - (0.5f*dt/dx)*(${func}(q_p)-${func}(q)));
    }
    """
    src = Template(src)
    if num_eq is 1:
        src = src.substitute(ar_len='',num_eq='',func=flux[0])
    else:
        src = src.substitute(ar_len='[{0}]'.format(num_eq), num_eq=num_eq,func=flux[0])

    src += flux[1]
    ocls_src = framework.loadFromString(src)
    return framework.createFluxFunction(domain, ocls_src, 'lax_wendroff')
