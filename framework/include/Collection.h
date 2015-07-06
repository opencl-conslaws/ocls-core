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


#ifndef COLLECTION_H
#define COLLECTION_H

#include "Data.h"

namespace ocls{

/** \ingroup main_api
*  @{
*/
class Collection {
public:
    Collection();
    Collection(const Collection &obj);
    Collection( Collection& obj );
    Collection& operator=(const Collection& obj);
    Collection& operator=(Collection& obj);

    ~Collection();

    Data* operator[](int i);
    Data* at(int i);

    size_t size();

    static Collection glob(std::vector<Data*>& data);

    template <typename ... Values>
    static Collection glob(Values& ... args){
        // Unpack values, compile error should be raised if not
        // of type Data*
        Data* tmp[sizeof...(Values)] = {args...};

        Collection collection;
        collection.m_size = (size_t)sizeof...(args);
        collection.m_values = new Data*[collection.m_size];
        memcpy(collection.m_values, &tmp[0], collection.m_size*sizeof(Data*));

        /*
        std::cout << "Globbing " << collection.m_size << " Data ";
        for (int i = 0; i < collection.m_size; ++i) {
            std::cout << "[" << collection.m_values[i] << "]";
        }
        std::cout << std::endl;
        */

        return collection;
    }

    //static Collection create(Data* val, ...);
    static void copy(Collection& src, Collection& dest);
    void copy(Collection& src);
private:
    void copyValues(Data** values);

private:
    size_t m_size;
    Data** m_values;
};
/** @} */

}
#endif
