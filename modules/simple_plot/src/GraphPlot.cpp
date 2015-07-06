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





#include <glm/gtc/matrix_transform.hpp>
#include "GraphPlot.h"
#include "Shaders.h"

GraphPlot::GraphPlot(ocls::Module* module, ocls::RenderContext *context, std::string name, ocls::Data *data, ocls::Domain *domain) :
        Plot(module, context, name, data, domain){
    if(domain->getDimensions() != 1){
        throw std::runtime_error("Graph plot only supports 1D data");
    }

    m_context = context;
    m_shader = NULL;
    m_mesh = NULL;

    m_window = m_context->createWindow(name, 800, 600);
    if(m_window == NULL){
        return;
    }

    m_window->setRepaintCallback(&GraphPlot::repaint, this);
    m_window->setCreateCallback(&GraphPlot::init, this);
    m_window->setDestoryCallback(&GraphPlot::clean, this);

    m_domain = domain;
    update(data);
}

GraphPlot::~GraphPlot() {

}

void GraphPlot::createLineMesh(size_t nx) {
    m_mesh = new LineMesh();

    std::vector<GLfloat>    vertices;
    std::vector<GLuint>     indices;

    float dx = 1.0f/nx;

    //Vertices
    vertices.reserve(nx+1);
    for (unsigned int i=0; i<=nx; ++i) {
        vertices.push_back(glm::mix(1.0f,-1.0f,dx*i));	//x
        indices.push_back(i);
    }

    m_mesh->vertices = new GLUtils::BO<GL_ARRAY_BUFFER>(vertices.data(),sizeof(GLfloat)*(GLuint)vertices.size());
    m_mesh->count = (GLuint)indices.size();
    m_mesh->indices = new GLUtils::BO<GL_ELEMENT_ARRAY_BUFFER>(indices.data(),sizeof(GLfloat)*(GLuint)indices.size());

    glGenVertexArrays(1, &m_mesh->vao);
    glBindVertexArray(m_mesh->vao);
    m_shader->use();
    m_mesh->vertices->bind();
    m_shader->setAttributePointer("position", 1);
    m_mesh->indices->bind();
    glBindVertexArray(0);
}

void GraphPlot::update(ocls::Data *data) {
    if(m_closed){
        return;
    }

    if(data != NULL) {
        m_data = data;
    }
    m_context->requestRepaint(m_window);
}

void GraphPlot::repaint() {
    glClearColor(1.0f,1.0f,1.0f,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();

    glUniform1i(m_shader->getUniform("tex"),0);

    float minmax[2] = {5.0f,10.0f};
    m_data->minmax(&minmax[0]);

    float tmp = std::fabs(minmax[1])+std::fabs(minmax[0]);
    float scale = tmp != 0.0f ? 1.0f/tmp : 1.0f;
    float center = ((minmax[1]+minmax[0])*scale)*0.5f;
    glUniform1f(m_shader->getUniform("scale"), scale);
    glUniform1f(m_shader->getUniform("center"), center);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_data->getTextureID());
    //glBindTexture(GL_TEXTURE_1D, m_data->getTextureID());

    glBindVertexArray(m_mesh->vao);
    glDrawElements(GL_LINE_STRIP, m_mesh->count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

    m_shader->disuse();
}

void GraphPlot::init() {
    if(m_shader == NULL){
        m_shader = new GLUtils::Program(graph_vert::source, graph_frag::source);
    }

    if(m_mesh == NULL){
        createLineMesh(m_domain->getMesh(ocls::Domain::Dim::X));
    }

    m_closed = false;
}

void GraphPlot::clean() {
    delete m_mesh; // < TODO: mem leak, fix later
    m_mesh = NULL;

    delete m_shader;
    m_shader = NULL;

    m_closed = true;
}
