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


#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "Data.h"

#include <map>

namespace ocls {

class Framework;

/** \ingroup main_api
*  @{
*/
class DataManager{
public:
    /**
     *
     */
    DataManager(Framework* framework);
    /**
     *
     */
    ~DataManager();

    /**
     * Create a new data object
     */
    Data* create(const size_t* size_, const std::string& reference_){
        Data* data = new Data(m_compute_context, m_render_context,
                                    size_, Data::READ_WRITE, reference_);
        m_data.push_back(data);

        return data;
    }

    Data* get(const std::string& reference_){
        for (int i = 0; i < m_data.size(); ++i) {
            if(reference_ == m_data[i]->getName()){
               return m_data[i];
            }
        }
        return NULL;
    }
    
private:
    ComputeContext* m_compute_context;
    RenderContext* m_render_context;
    
    //std::map<std::string, Data*> m_data;
    std::vector<Data*> m_data;
};
/** @} */

} //cls

#endif //PIPELINE_H
