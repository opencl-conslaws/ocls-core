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