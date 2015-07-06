/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */



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

