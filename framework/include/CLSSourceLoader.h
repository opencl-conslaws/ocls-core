#ifndef CLS_SOURCE_LOADER_H
#define CLS_SOURCE_LOADER_H

#include <map>
#include "Logger.h"

namespace ocls {

class CLSSource;
class Framework;

/** \ingroup main_api
*  @{
*/
class CLSSourceLoader{
private:
    friend class Framework;
    
    /**
     *
     */
    CLSSourceLoader(Framework* framwork);
    /**
     *
     */
    ~CLSSourceLoader();

public:
    /**
     * Load source from string
     */
    CLSSource* loadFromString(std::string str);
    
    /**
     * Load source from file
     */
    CLSSource* loadFromFile(std::string path);
    
private:
    void removeComments(std::string* str);
    
    /**
     * Reads a file, returns true if success, false otherwise
     */
    bool readFile(std::string file, std::string* content);

private:
    std::map<size_t, CLSSource*> m_source;
};
/** @} */

} //cls

#endif //CLS_SOURCE_LOADER_H