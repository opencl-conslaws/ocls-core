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
