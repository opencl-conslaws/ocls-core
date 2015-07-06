#include "CLSSourceLoader.h"
#include "CLSSource.h"
#include <Framework.h>

namespace ocls {
CLSSourceLoader::CLSSourceLoader(Framework* framework){
}
    
CLSSourceLoader::~CLSSourceLoader(){
    std::map<size_t, CLSSource*>::iterator it;
    
    for (it=m_source.begin(); it!=m_source.end(); ++it){
        delete it->second;
    }
}
    
CLSSource* CLSSourceLoader::loadFromString(std::string str){
    removeComments(&str);
    
    CLSSource* src = new CLSSource();
    
    size_t i = str.find_first_not_of(" \t\n");
    while (i != std::string::npos) {
        size_t j = str.find_first_of(";(", i);
        
        if (str.substr(i,6) == "struct") {
            i = src->parseStructure(str, i);
        }else if (str[j] == ';') {
            i = src->parseVariable(str, i);
        }else if(str[j] == '('){
            i = src->parseFunction(str, i);
        }else{
            logger->log(Logger::ERROR, "Syntax error: found %c but expected ; or (", str[j]);
            i++;
        }
        
        i = str.find_first_not_of(" \t\n", i);
    }
    src->solveDependencies();
    
    m_source[src->id()] = src;
    return src;
}
    
CLSSource* CLSSourceLoader::loadFromFile(std::string path){
    std::string src;
    if (!readFile(path, &src)) {
        logger->log(Logger::ERROR, "Failed to load file");
        return NULL;
    }
    return loadFromString(src);
}
    
void CLSSourceLoader::removeComments(std::string* str){
    size_t i = str->find("/*");
    while (i != std::string::npos) {
        size_t j = str->find("*/", i);
        str->replace(i,j-i+2,"");
        i = str->find("/*", i);
    }
    i = str->find("//");
    while (i != std::string::npos) {
        size_t j = str->find("\n", i);
        str->replace(i,j-i,"");
        i = str->find("//", i);
    }
}
    
bool CLSSourceLoader::readFile(std::string file, std::string* content) {
    long length;
    std::string buffer;
        
    std::ifstream is;
    is.open(file.c_str());
        
    if (!is.good()) {
        return false;
    }
        
    // get length of file:
    is.seekg (0, std::ios::end);
    length = is.tellg();
    is.seekg (0, std::ios::beg);
        
    // reserve memory:
    content->reserve(length);
        
    // read data
    while(getline(is,buffer)) {
        content->append(buffer);
        content->append("\n");
    }
    is.close();
        
    return true;
}
    
}