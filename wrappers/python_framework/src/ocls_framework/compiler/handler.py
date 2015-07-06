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


from ocl import C99Handler
from ocl import get_arg

import ast
import re

class oclsHandler(C99Handler):

    def __init__(self):
        C99Handler.__init__(self)
        self.vec_types = None

    def make_types(self, types):
        self.vec_types = types.copy()
        for key, value in types.items():
            self.vec_types[key] = self.make_arr_len(value)
            types[key] = self.make_type(value)
        return types

    def is_FunctionDef(self, item, pad, types):
        args = ', '.join('%s %s%s' % (
            self.make_type(types[get_arg(a)]), get_arg(a), \
                '[%d]' % self.vec_types[get_arg(a)] if self.vec_types[get_arg(a)] else '')
            for a in item.args.args)
        return 'void %s(%s) {\n/*@VARS*/\n%s\n}' % (
            item.name, args, self.t(item.body, pad + self.sep))

    @staticmethod
    def make_arr_len(name):
        if name.startswith('data:'):
            return int(name[5:])
        return None

    @staticmethod
    def make_type(name):
        newname = stars = ''
        if name.startswith('data'):
            newname, name = newname + 'float', '*'
        return newname + name + stars

    def is_Assign(self, item, pad):
        if len(item.targets) != 1:
            raise NotImplementedError
        left, right = item.targets[0], item.value
        if isinstance(left, ast.Name) and not left.id in self.symbols:
            if isinstance(right, ast.Call):
                if right.func.id.startswith('new_'):
                    self.symbols[left.id] = right.func.id[4:]
                    right = right.args[0] if right.args else None
                elif right.func.id.startswith('fetch'):
                    self.symbols[left.id] = 'float'
                    self.symbols[left.id] += '%d' % self.vec_types[self.t(right.value)] if self.vec_types[self.t(right.value)] else ''
            elif isinstance(right.ctx, ast.Load):
                self.symbols[left.id] = 'float'
                self.symbols[left.id] += '%d' % self.vec_types[self.t(right.value)] if self.vec_types[self.t(right.value)] else ''
            else:
                # guess type
                if isinstance(right, ast.Num) and isinstance(right.n, float):
                    self.symbols[left.id] = 'float'
                elif isinstance(right, ast.Num) and isinstance(right.n, int):
                    self.symbols[left.id] = 'int'
                elif isinstance(right, ast.Str):
                    self.symbols[left.id] = 'ptr_char'
                else:
                    raise RuntimeError('unkown C-type %s' % left.id)
            return '%s = (%s)%s;' % (
                self.t(item.targets[0]), self.make_type(self.symbols[left.id]),
                self.t(right)) if right else ''
        elif isinstance(left.ctx, ast.Store):
            return 'store%s(%s, %s, %s);' % ('%d' % self.vec_types[self.t(left.value)] if self.vec_types[self.t(left.value)] else '', \
            self.t(left.value), self.t(right), self.t(left.slice.value))
        else:
            return '%s = %s;' % (self.t(item.targets[0]),self.t(right)) if right else ''

    def is_Subscript(self, item, pad):
        if isinstance(item.ctx, ast.Load):
            return 'fetch%s(%s,%s)' % ('%d' % self.vec_types[self.t(item.value)] if self.vec_types[self.t(item.value)] else '', \
             self.t(item.value), self.t(item.slice.value))
