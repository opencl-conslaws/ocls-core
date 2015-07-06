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



#ifndef VOLUME_PLOT_H
#define VOLUME_PLOT_H

#include "RenderContext.h"
#include "Data.h"
#include "GLUtils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "VirtualTrackball.h"
#include "TextureFBO.h"

#include "Plot.h"

class VolumePlot : public Plot{

public:
    VolumePlot(ocls::Module* module, ocls::RenderContext* context, std::string name, ocls::Data* data, ocls::Domain* domain);
    virtual ~VolumePlot();

    void update(ocls::Data* data);

private:
    void createUnitBox();
    void createUnitSquare();

    void renderUnitBox(GLUtils::Program* shader);

    void windowResized(int width,int height);
    void cursorMotion(double x, double y);
    void mouseButton(int button, int action, int mods);
    void keypress(int key, int scancode, int action, int mods);

    void repaint();
    void init();
    void clean();

private:
    ocls::RenderContext* m_context;
    ocls::RenderWindow* m_window;

    ocls::Data* m_data;
    ocls::Domain* m_domain;

    GLUtils::Program* m_pone_shader; // < Pass-1 shader
    GLUtils::Program* m_ptwo_shader; // < Pass-2 shader

    glm::mat4 m_model;
    struct {
        glm::mat4 projection;
        glm::mat4 view;
    } m_camera;

    VirtualTrackball m_trackball;
    int m_pos[2];

    TextureFBO* m_back_face_fbo;
    TextureFBO* m_front_face_fbo;

    struct {
        size_t count;
        GLUtils::BO<GL_ARRAY_BUFFER>* vertices;
        GLUtils::BO<GL_ELEMENT_ARRAY_BUFFER>* indices;
        GLuint vao;
    } m_unit_box, m_unit_square;

};

#endif

