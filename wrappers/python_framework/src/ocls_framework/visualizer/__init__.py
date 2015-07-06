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

import ocls_visualizer_swig as c_api_viz
import ocls_framework

__visualizer__ = None
__plot_stack__ = {}

def init(framework=None):
    global __visualizer__, __plot_stack__

    __plot_stack__ = {}
    if __visualizer__ is not None:
        del __visualizer__
    __visualizer__ = None

    if framework is not None:
        __visualizer__ = c_api_viz.oclsVizInit(framework._Framework__ctx)
    else:
        if ocls_framework.ocls_framework.__framework__ is None:
            raise EnvironmentError("ocls needs to be import before this module")
        __visualizer__ = c_api_viz.oclsVizInit(ocls_framework.ocls_framework.__framework__._Framework__ctx)
    return __visualizer__

# Init on current context
init()

def createPlot(data, visualiser):
    return c_api_viz.oclsVisPlot(visualiser, data._Data__handle, data._Data__domain._Domain__d)

def updatePlot(figure, data):
    c_api_viz.oclsVisUpdate(figure, data._Data__handle);

def plot(data, visualizer=None):
    global __visualizer__, __plot_stack__

    if data not in __plot_stack__:
        if visualizer is None:
            __plot_stack__[data] = createPlot(data, __visualizer__)
        else:
            __plot_stack__[data] = createPlot(data, visualizer)
    else:
        updatePlot(__plot_stack__[data], data)
