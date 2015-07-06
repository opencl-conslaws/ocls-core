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


#include "Collection.h"

namespace ocls {

Collection::Collection() :
   m_size(0), m_values(NULL) {
    //std::cout << "Size set in default construtor " << m_size << std::endl;
}

Collection::Collection(const Collection &obj) :
    m_size(obj.m_size), m_values(NULL)  {
    copyValues(obj.m_values);
    //std::cout << "Size set in copy constructor const " << m_size << std::endl;
}
Collection::Collection(Collection &obj) :
    m_size(obj.m_size), m_values(NULL)  {
    copyValues(obj.m_values);
    //std::cout << "Size set in copy constructor " << m_size << std::endl;
}

Collection& Collection::operator=(const Collection &obj) {
    m_size = obj.m_size;
    //std::cout << "Size set in = operator const" << m_size << std::endl;
    copyValues(obj.m_values);
    return *this;
}

Collection& Collection::operator=(Collection &obj) {
    m_size = obj.m_size;
    //std::cout << "Size set in = operator " << m_size << std::endl;
    copyValues(obj.m_values);
    return *this;
}

Collection::~Collection() {
    if(m_values != NULL)
        delete [] m_values;
    m_values = NULL;
}

Data *Collection::operator[](int i) {
    return at(i);
}

Data* Collection::at(int i){
    if (m_size == 0){
        return NULL;
    }
    return m_values[i];
}

size_t Collection::size() {
    return m_size;
}

/*
Collection Collection::create(Data* val, ...) {
    Collection collection;

    va_list args;
    va_start(args, val);

    const size_t max_args = 10;
    collection.m_values = new Data*[max_args];
    collection.m_values[collection.m_size++] = val;

    Data* v = va_arg(args, Data*);
    while (v != NULL){
        collection.m_values[collection.m_size++] = v;
        v = va_arg(args, Data*);
    }

    va_end(args);

    return collection;
}
*/

void Collection::copy(Collection &src, Collection &dest) {
    for(size_t i = 0; i < src.m_size; i++){
        if(dest[i] == NULL){
            // TODO: add ability to clone new copy of existiting data
        }

        // Copy data
        dest[i]->copy(src[i]);
    }
    dest.m_size = src.m_size;
}

void Collection::copyValues(Data **values) {
    if(m_values != NULL){
        delete [] m_values;
        m_values = NULL;
    }
    m_values = new Data*[m_size];
    memcpy(m_values, values, sizeof(Data*)*m_size);
}

void Collection::copy(Collection &src) {
    for(size_t i = 0; i < src.m_size; i++){
        if(this->at(i) == NULL){
            // TODO: add ability to clone new copy of existiting data
        }

        // Copy data
        this->at(i)->copy(src[i]);
    }
    m_size = src.m_size;
    //std::cout << "Size changed in copy function " << m_size << std::endl;
}

Collection Collection::glob(std::vector<Data *>& data) {
    Collection collection;
    collection.m_size = data.size();
    collection.m_values = new Data*[collection.m_size];
    memcpy(collection.m_values, &data[0], collection.m_size*sizeof(Data*));

    return collection;
}
}
