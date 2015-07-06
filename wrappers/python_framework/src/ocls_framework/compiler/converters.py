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

from ocl import Compiler as cCompiler
from meta.decompiler import decompile_func
import handler

class Compiler:
    def __init__(self, framework, domain, verbose=False):
        self.framework = framework
        self.domain = domain
        self.deps = []
        self.verbose = verbose

    def get_source(self, func, name, types):
        c99 = cCompiler(handler=handler.oclsHandler())
        types = c99.handler.make_types(types)
        decompiled = decompile_func(func)
        c99.functions[name] = dict(func=func,
                                    prefix='',
                                    ast=decompiled,
                                    types=types,
                                    code=None)

        return c99.convert(headers=False)

    def device(self, **types):
        def wrap(func, types=types):

            src = self.get_source(func, func.__name__, types)

            self.deps.append(src)
            return src
        return wrap

    def flux(self, **types):
        def wrap(func, types=types):
            name = func.__name__
            src = ""
            for dep in self.deps:
                src += (dep+'\n')

            src += self.get_source(func, name, types)
            if self.verbose:
                print "Compiled flux function:\n", src

            ocls_src = self.framework.loadFromString(src)
            return self.framework.createFluxFunction(self.domain, ocls_src, name)
        return wrap

    def integrator(self, **types):
        def wrap(func, types=types):
            name = func.__name__
            src = ""
            for dep in self.deps:
                src += (dep+'\n')

            src += self.get_source(func, name, types)
            if self.verbose:
                print "Compiled integrator function:\n", src

            ocls_src = self.framework.loadFromString(src)
            return self.framework.createIntegratorFunction(self.domain, ocls_src, name)
        return wrap

    def boundary(self, **types):
        def wrap(func, types=types):
            name = func.__name__
            src = ""
            for dep in self.deps:
                src += (dep+'\n')

            src += self.get_source(func, name, types)
            if self.verbose:
                print "Compiled boundary function:\n", src

            ocls_src = self.framework.loadFromString(src)
            return self.framework.createBoundaryFunction(self.domain, ocls_src, name)
        return wrap
