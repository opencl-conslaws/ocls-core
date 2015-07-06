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


#include "Domain.h"
#include <iostream>
#include <cmath>
#include "Logger.h"

namespace ocls {
    
Domain::Domain(const unsigned short& dimensions_) : m_dimensions(dimensions_){
    if (m_dimensions > 3){
        logger->log(Logger::WARNING, "Invalid number of dimensions, cropped to 3");
        m_dimensions = 3;
    }
    else if(m_dimensions <= 0) {
        logger->log(Logger::WARNING, "Invalid number of dimensions, cropped to 1");
        m_dimensions = 1;
    }
    
    //m_bounds.resize(m_dimensions);
    //m_mesh.resize(m_dimensions);
    
    m_ghost_cells = 0;
    
    switch (m_dimensions) {
        case 1:
            setBounds(-1.0f,1.0f);
            setMesh(512);
            break;
        case 2:
            setBounds(-1.0f,1.0f,-1.0f,1.0f);
            setMesh(512,512);
            break;
        case 3:
            setBounds(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
            setMesh(512,512,512);
            break;
        default:
            break;
    }
}

/*
Domain::Domain(const Domain &other) {
    std::cout << "Domain Copy constructor invoked" << std::endl;
    memcpy(&this->m_bounds[0], &other.m_bounds[0], sizeof(Boundary)*3);
    memcpy(&this->m_mesh[0], &other.m_mesh[0], sizeof(size_t)*3);
    this->m_dimensions = other.m_dimensions;
    this->m_ghost_cells = other.m_ghost_cells;
}

Domain::Domain(Domain &other) {
    std::cout << "Domain Copy constructor invoked" << std::endl;
    memcpy(&this->m_bounds[0], &other.m_bounds[0], sizeof(Boundary)*3);
    memcpy(&this->m_mesh[0], &other.m_mesh[0], sizeof(size_t)*3);
    this->m_dimensions = other.m_dimensions;
    this->m_ghost_cells = other.m_ghost_cells;
}
*/

Domain::~Domain(){
    //..
}
    
void Domain::setBounds(const float& left_, const float& right_){
    m_bounds[0].min = left_;
    m_bounds[0].max = right_;
}
void Domain::setBounds(const float& left_, const float& right_, const float& bottom_, const float& top_){
    setBounds(left_,right_);
    
    if (m_dimensions < 2) {
        logger->log(Logger::WARNING, "Invalid number of dimensions");
        return;
    }
    
    m_bounds[1].min = bottom_;
    m_bounds[1].max = top_;
}
void Domain::setBounds(const float& left_, const float& right_, const float& bottom_, const float& top_, const float& front_, const float& back_){
    setBounds(left_,right_,bottom_,top_);
    
    if (m_dimensions < 3) {
        logger->log(Logger::WARNING, "Invalid number of dimensions");
        return;
    }
    
    m_bounds[2].min = front_;
    m_bounds[2].max = back_;
}
void Domain::setMesh(const size_t& x_){
    m_mesh[0] = x_;
}
void Domain::setMesh(const size_t& x_, const size_t& y_){
    setMesh(x_);
    if (m_dimensions < 2) {
        logger->log(Logger::WARNING, "Invalid number of dimensions");
        return;
    }
    m_mesh[1] = y_;
}
void Domain::setMesh(const size_t& x_, const size_t& y_, const size_t& z_){
    setMesh(x_,y_);
    if (m_dimensions < 3) {
        logger->log(Logger::WARNING, "Invalid number of dimensions");
        return;
    }
    m_mesh[2] = z_;
}

void Domain::setGhostCells(const unsigned short& ghost_cells_){
    m_ghost_cells = ghost_cells_;
}
    
Domain::Boundary* Domain::getBounds(){
    return &m_bounds[0];
}
    
Domain::Boundary Domain::getBounds(Dim dimension) const{
    if (dimension > m_dimensions) {
        return Boundary();
    }
    return m_bounds[dimension-1];
}
    
size_t* Domain::getMesh(){
    return &m_mesh[0];
}
    
size_t Domain::getMesh(Dim dimension) const{
    if (dimension > m_dimensions) {
        return 1;
    }
    return m_mesh[dimension-1];
}
    
size_t Domain::getMeshWithGC(Dim dimension) const{
    if (dimension > m_dimensions) {
        return 1;
    }
    return m_mesh[dimension-1] + m_ghost_cells*2;
}
    
unsigned short Domain::getDimensions() const{
    return m_dimensions;
}
    
unsigned short Domain::getGhostCells() const{
    return m_ghost_cells;
}

float Domain::getDelta(Domain::Dim dimension) const{
    return (std::fabs(getBounds(dimension).min)+
            std::fabs(getBounds(dimension).max))/
            (float)getMesh(dimension);
}
}
