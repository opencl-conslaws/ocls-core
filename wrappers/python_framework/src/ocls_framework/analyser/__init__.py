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



import ocls_framework as ocls

start                   = ocls.c_api.oclsAnalyserStart
end                     = ocls.c_api.oclsAnalyserEnd
timeline                = ocls.c_api.oclsAnalyserRead
kernel_to_string        = ocls.c_api.oclsAnalyserKernelRead
memory_to_string        = ocls.c_api.oclsAnalyserMemoryRead
kernel                  = ocls.c_api.oclsAnalyserKernel
memory                  = ocls.c_api.oclsAnalyserMemory
