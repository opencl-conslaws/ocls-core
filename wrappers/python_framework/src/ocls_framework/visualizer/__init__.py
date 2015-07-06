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
