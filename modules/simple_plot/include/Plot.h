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



#ifndef PLOT_H
#define PLOT_H

#include "Framework.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Plot {
public:
    Plot(ocls::Module* module, ocls::RenderContext* context, std::string name, ocls::Data* data, ocls::Domain* domain);
    virtual ~Plot(){};

    virtual void update(ocls::Data* data = NULL) = 0;

    size_t getID();

    static void startSync();
    static void endSync();

protected:
    friend class SimplePlot;
    void requestRepaintWindow();

protected:
    size_t m_id;
    static size_t id_seed;

    ocls::RenderContext* m_context;
    ocls::RenderWindow* m_window;

    volatile bool m_closed;

    ocls::Data* m_data;
    ocls::Domain* m_domain;

    ocls::Module* m_module;

    static bool syncing;
    static glm::mat4 transform;
};

#endif
