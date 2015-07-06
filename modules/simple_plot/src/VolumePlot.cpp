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





#include "VolumePlot.h"
#include "Shaders.h"
#include "SimplePlotModule.h"

VolumePlot::VolumePlot(ocls::Module* module, ocls::RenderContext *context, std::string name, ocls::Data *data, ocls::Domain *domain) :
        Plot(module, context, name, data, domain){
    if(domain->getDimensions() != 3){
        throw std::runtime_error("Volume plot only supports 3D data");
    }

    m_context = context;

    m_pone_shader = NULL;
    m_ptwo_shader = NULL;
    m_back_face_fbo = NULL;
    m_front_face_fbo = NULL;

    m_window = m_context->createWindow(name, 800, 600);
    if(m_window == NULL){
        return;
    }

    m_trackball.setWindowSize(800, 600);

    m_window->setWindowSizeUpdateCallback(&VolumePlot::windowResized, this);
    m_window->setCursorMotionCallback(&VolumePlot::cursorMotion, this);
    m_window->setMousebuttonCallback(&VolumePlot::mouseButton, this);
    m_window->setRepaintCallback(&VolumePlot::repaint, this);
    m_window->setCreateCallback(&VolumePlot::init, this);
    m_window->setDestoryCallback(&VolumePlot::clean, this);
    m_window->setKeyPressCallback(&VolumePlot::keypress, this);

    m_camera.projection = (glm::mat4)glm::perspective(45.0f,
            m_window->getFramebufferWidth() / (float) m_window->getFramebufferHeight(), 1.0f, 50.0f);
    m_camera.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.8f));
    m_model = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f));

    m_domain = domain;
    update(data);
}

VolumePlot::~VolumePlot() {

}

void VolumePlot::update(ocls::Data *data) {
    if(m_closed){
        return;
    }

    if(data != NULL) {
        m_data = data;
    }
    m_context->requestRepaint(m_window);
}

void VolumePlot::windowResized(int width, int height) {
    m_trackball.setWindowSize(width, height);
    m_camera.projection = (glm::mat4)glm::perspective(45.0f,
            width / (float) height, 1.0f, 50.0f);


    if(m_back_face_fbo->getWidth() != width ||
            m_back_face_fbo->getHeight() != height){
        delete m_back_face_fbo;
        m_back_face_fbo = new TextureFBO(width, height);
    }

    if(m_front_face_fbo->getWidth() != width ||
            m_front_face_fbo->getHeight() != height){
        delete m_front_face_fbo;
        m_front_face_fbo = new TextureFBO(width, height);
    }


    m_context->requestRepaint(m_window);
}

void VolumePlot::cursorMotion(double x, double y) {
    m_pos[0] = (int)x;
    m_pos[1] = (int)y;
    m_trackball.rotate(m_pos[0], m_pos[1]);
    if(m_trackball.isRotating())
        m_context->requestRepaint(m_window);
}

void VolumePlot::mouseButton(int button, int action, int mods) {
    if(action > 0){
        m_trackball.rotateBegin(m_pos[0], m_pos[1]);
    }else{
        m_trackball.rotateEnd(m_pos[0], m_pos[1]);
    }
    m_context->requestRepaint(m_window);
}

void VolumePlot::repaint() {
    glClearColor(1.0f,1.0f,1.0f,1.0);

    // Pass 1
    m_pone_shader->use();

    m_back_face_fbo->bind(GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    renderUnitBox(m_pone_shader);
    m_back_face_fbo->unbind(GL_FRAMEBUFFER);

    m_front_face_fbo->bind(GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    renderUnitBox(m_pone_shader);
    m_front_face_fbo->unbind(GL_FRAMEBUFFER);

    m_pone_shader->disuse();

    // Pass 2
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ptwo_shader->use();

    float minmax[2];
    //minmax[0] = 0.1f;
    //minmax[1] = 1.5f;
    m_data->minmax(&minmax[0]);
    glUniform2fv(m_ptwo_shader->getUniform("thresholds"), 1, &minmax[0]);

    glUniform1i(m_ptwo_shader->getUniform("tex"),0);
    glUniform1i(m_ptwo_shader->getUniform("back_face"),1);
    glUniform1i(m_ptwo_shader->getUniform("front_face"),2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, m_data->getTextureID());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_back_face_fbo->getTexture());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_front_face_fbo->getTexture());

    glBindVertexArray(m_unit_square.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    glBindVertexArray(m_unit_square.vao);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_ptwo_shader->disuse();
}

void VolumePlot::init() {
    if (m_back_face_fbo == NULL){
        m_back_face_fbo = new TextureFBO(800,600);
    }
    if (m_front_face_fbo == NULL){
        m_front_face_fbo = new TextureFBO(800,600);
    }

    if (m_pone_shader == NULL){
        m_pone_shader = new GLUtils::Program(volume_pone_vert::source, volume_pone_frag::source);
    }

    if (m_ptwo_shader == NULL){
        m_ptwo_shader = new GLUtils::Program(volume_ptwo_vert::source, volume_ptwo_frag::source);
    }

    createUnitBox();
    createUnitSquare();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_closed = false;
}

void VolumePlot::clean() {
    delete m_back_face_fbo;
    delete m_front_face_fbo;
    delete m_pone_shader;
    delete m_ptwo_shader;

    delete m_unit_box.vertices;
    delete m_unit_square.vertices;
    delete m_unit_square.indices;

    glDeleteVertexArrays(1, &m_unit_box.vao);
    glDeleteVertexArrays(1, &m_unit_square.vao);

    m_closed = true;
}

void VolumePlot::createUnitBox() {
    glGenVertexArrays(1, &m_unit_box.vao);
    glBindVertexArray(m_unit_box.vao);
    GLfloat cube_vertices[] = {
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,

            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,

            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f
    };
    m_unit_box.vertices = new GLUtils::BO<GL_ARRAY_BUFFER>(cube_vertices, sizeof(cube_vertices));

    m_unit_box.count = sizeof(cube_vertices)/(sizeof(float)*3);;

    m_unit_box.vertices->bind();
    m_pone_shader->setAttributePointer("position", 3);

    glBindVertexArray(0);
}

void VolumePlot::createUnitSquare() {
    glGenVertexArrays(1, &m_unit_square.vao);
    glBindVertexArray(m_unit_square.vao);
    GLfloat quad_vertices[] =  {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f,  1.0f,
            -1.0f,  1.0f,
    };
    m_unit_square.vertices = new GLUtils::BO<GL_ARRAY_BUFFER>(quad_vertices, sizeof(quad_vertices));

    GLubyte quad_indices[] = {
            0, 1, 2, //triangle 1
            2, 3, 0, //triangle 2
    };
    m_unit_square.indices = new GLUtils::BO<GL_ELEMENT_ARRAY_BUFFER>(quad_indices, sizeof(quad_indices));
    m_unit_square.count = 6;

    m_unit_square.vertices->bind();
    m_ptwo_shader->setAttributePointer("position", 2);
    m_unit_square.indices->bind();

    glBindVertexArray(0);
}

void VolumePlot::renderUnitBox(GLUtils::Program* shader) {
    glUniformMatrix4fv(shader->getUniform("projection_matrix"), 1, 0, glm::value_ptr(m_camera.projection));
    glUniformMatrix4fv(shader->getUniform("model_matrix"), 1, 0, glm::value_ptr(m_model));
    if (syncing) {
        if (m_window->hasFocus()) {
            transform = m_trackball.getTransform();
            // Request a repaint on everyone except me
            dynamic_cast<SimplePlot*>(m_module)->requestRepaintAllExcept(getID());
        }
        glUniformMatrix4fv(shader->getUniform("view_matrix"), 1, 0, glm::value_ptr(m_camera.view*transform));
    }else {
        glUniformMatrix4fv(shader->getUniform("view_matrix"), 1, 0, glm::value_ptr(m_camera.view * m_trackball.getTransform()));
    }
    //glUniformMatrix4fv(shader->getUniform("view_matrix"), 1, 0, glm::value_ptr(m_camera.view*m_trackball.getTransform()));

    glBindVertexArray(m_unit_box.vao);
    glDrawArrays(GL_TRIANGLES, 0, m_unit_box.count);
    glBindVertexArray(0);
}

void VolumePlot::keypress(int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT){
        startSync();
    }
    else if(action == GLFW_RELEASE && key == GLFW_KEY_LEFT_SHIFT){
        endSync();
    }
    m_context->requestRepaint(m_window);
}
