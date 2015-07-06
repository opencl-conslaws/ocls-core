#ifndef GRAPH_PLOT_H
#define GRAPH_PLOT_H

#include "RenderContext.h"
#include "Data.h"
#include "GLUtils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <grp.h>

#include "Plot.h"

class GraphPlot : public Plot{
    struct LineMesh{
        size_t count;
        GLUtils::BO<GL_ARRAY_BUFFER>* vertices;
        GLUtils::BO<GL_ELEMENT_ARRAY_BUFFER>* indices;
        GLuint vao;
    };

public:

    GraphPlot(ocls::Module* module, ocls::RenderContext* context, std::string name, ocls::Data* data, ocls::Domain* domain);
    ~GraphPlot();

    void update(ocls::Data* data);

private:
    void createLineMesh(size_t nx);

    void repaint();
    void init();
    void clean();

private:
    GLUtils::Program* m_shader;
    LineMesh* m_mesh;

};

#endif

