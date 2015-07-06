#include "CLSSource.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include "Logger.h"

namespace ocls {

size_t CLSSource::m_id_gen = 0;
    
CLSSource::CLSSource():m_id(++m_id_gen){
        
}
    
CLSSource::~CLSSource(){
    std::map<std::string, Variable*>::iterator v_it;
    std::map<std::string, Function*>::iterator f_it;
    std::map<std::string, Structure*>::iterator s_it;
    
    for (v_it=m_variables.begin(); v_it!=m_variables.end(); ++v_it){
        delete v_it->second;
    }
    for (f_it=m_functions.begin(); f_it!=m_functions.end(); ++f_it){
        delete f_it->second;
    }
    for (s_it=m_structures.begin(); s_it!=m_structures.end(); ++s_it){
        std::map<std::string, Variable*>::iterator sv_it;
        for (sv_it=s_it->second->variables.begin(); sv_it!=s_it->second->variables.end(); ++sv_it){
            delete sv_it->second;
        }
        delete s_it->second;
    }
}
 
size_t CLSSource::parseStructure(std::string& src, size_t start){
    Structure* structure = new Structure();
    structure->source = this;
    
    size_t i = src.find_first_of(" \t\n", start);
    i = src.find_first_not_of(" \t\n", i+1);
    size_t j = src.find_first_of(" \t\n{", i+1);
    structure->name = src.substr(i, j-i);
    j = src.find_first_of("{", j);
    
    //std::cout << "Creating data structure: \"" << structure->name << "\"" << std::endl;
    m_structures[structure->name] = structure;
    
    i = src.find_first_not_of(" \t\n", j+1);
    while (src[i] != '}') {
        i = parseVariable(src, i, structure->variables);
        i = src.find_first_not_of(" \t\n", i);
    }
    
    return i+2;
}
    
size_t CLSSource::parseVariable(std::string& src, size_t start){
    return parseVariable(src, start, m_variables);
}
    
size_t CLSSource::parseVariable(std::string& src, size_t start, std::map<std::string, Variable*>& container){
    Variable* variable = new Variable();
    variable->source = this;
        
    // Variable form:
    // type name = value;
        
    size_t i = start;
    size_t j = src.find_first_of(" \t\n", i);
    variable->type = src.substr(i, j-i);
    i = src.find_first_not_of(" \t\n", j+1);//j+1;
    j = src.find_first_of(" \t\n=;", i);
    variable->name = src.substr(i, j-i);
    if (src[j] != ';') {
        if (src[j] != '=') {
            i = j;
            j = src.find_first_of("=", i);
        }
        i = j+1;
        j = src.find_first_of(";", j-1);
        variable->value = src.substr(i, j-i);
        variable->value.erase(
            remove_if(variable->value.begin(), variable->value.end(), isspace), variable->value.end()
        );

    }
    
    i = j+1;

    container[variable->name] = variable;
        
    return i;
}
    
size_t CLSSource::parseFunction(std::string& src, size_t start){
    Function* function = new Function();
    function->source = this;
    
    size_t i = src.find_first_of(" \t\n", start);
    i = src.find_first_not_of(" \t\n", i+1);
    size_t j = src.find_first_of(" \t\n(", i+1);
    function->name = src.substr(i, j-i);
    
    i = src.find_first_of("{", i);
    function->signature = src.substr(start, i-start);
    j = i+1;
    
    bool found = false;
    int scope = 0;
    while (!found) {
        if (src[j] == '{') {
            scope++;
        }
        if (src[j] == '}') {
            if (scope == 0) {
                found = true;
            }else{
                scope--;
            }
        }
        j++;
    }
    function->block = src.substr(i, j-i);

    parseSignature(function);
    m_functions[function->name] = function;
    
    return j+1;
}
    
void CLSSource::solveDependencies(){
    std::map<std::string, Function*>::iterator i;
    std::map<std::string, Function*>::iterator j;
    std::map<std::string, Structure*>::iterator k;
    std::map<std::string, Variable*>::iterator l;
    
    for (i=m_functions.begin(); i!=m_functions.end(); ++i){
        for (j=m_functions.begin(); j!=m_functions.end(); ++j){
            size_t index = i->second->block.find(j->second->name);
            if (index != std::string::npos) {
                logger->log(Logger::DEBUG, "%s depends on function %s", i->second->name.c_str(), j->second->name.c_str());
                i->second->function_dependencies.push_back(j->second);
            }
        }
        for (k=m_structures.begin(); k!=m_structures.end(); ++k) {
            size_t index = i->second->block.find(k->second->name);
            if (index != std::string::npos) {
                logger->log(Logger::DEBUG, "%s depends on structure %s", i->second->name.c_str(), k->second->name.c_str());
                i->second->structure_dependencies.push_back(k->second);
            }else {
                index = i->second->signature.find(k->second->name);
                if (index != std::string::npos) {
                    logger->log(Logger::DEBUG, "%s depends on structure %s", i->second->name.c_str(), k->second->name.c_str());
                    i->second->structure_dependencies.push_back(k->second);
                }
            }
        }
        for (l=m_variables.begin(); l!=m_variables.end(); ++l) {
            size_t index = i->second->block.find(l->second->name);
            if (index != std::string::npos) {
                logger->log(Logger::DEBUG, "%s depends on variable %s", i->second->name.c_str(), l->second->name.c_str());
                i->second->variable_dependencies.push_back(l->second);
            }
        }
    }

}
    
CLSSource::Function* CLSSource::getFunction(std::string name){
    std::map<std::string, CLSSource::Function*>::iterator it = m_functions.find(name);
    if (it == m_functions.end()) {
        logger->log(Logger::WARNING, "Failed to find function");
        return NULL;
    }
    return it->second;
}
    
    
CLSSource::Variable* CLSSource::getVariable(std::string name){
    std::map<std::string, CLSSource::Variable*>::iterator it = m_variables.find(name);
    if (it == m_variables.end()) {
        logger->log(Logger::WARNING, "Failed to find variable");
        return NULL;
    }
    return it->second;
}
 
size_t CLSSource::id(){
    return m_id;
}

void CLSSource::parseSignature(CLSSource::Function *function) {
    std::string sign = function->signature;
    //std::cout << std::endl;
    //std::cout << "Parsing signature " << std::endl;
    //std::cout << sign << std::endl;

    size_t i = sign.find_first_of(" \t\n", 0);
    function->returntype = sign.substr(0, i);

    //std::cout << "Return type:  " << std::endl;
    //std::cout << function->returntype << std::endl;

    bool done = false;
    i = sign.find_first_of("(", i)+1; // < start of param list
    size_t j = i;
    while(!done){
        if (sign[i] == ')') {
            function->parameters.push_back(parseParameter(sign.substr(j, i-j)));
            done = true;
        }
        if (sign[i] == ',') {
            function->parameters.push_back(parseParameter(sign.substr(j, i-j)));
            j = i = sign.find_first_not_of(" \t\n", i+1);
        }
        i++;
    }
    /*
    std::cout << "Parameters ["<<function->parameters.size()<<"]:" << std::endl;
    for (int k = 0; k < function->parameters.size(); ++k) {
        std::cout << "[" << function->parameters[k]->type
                << " " << function->parameters[k]->name;
        if(function->parameters[k]->array_type){
            std::cout << "[" << function->parameters[k]->array_size << "]";
        }

        std::cout << "], ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    */
}

CLSSource::Variable *CLSSource::parseParameter(std::string str) {
    Variable* var = new Variable;
    size_t i = str.find_first_of(" \t\n", 0);
    var->type = str.substr(0, i);
    if(var->type == "void"){
        var->name = "";
        var->array_size = 0;
        var->array_type = false;
        return var;
    }

    var->name = str.substr(i+1, str.length()-i);
    i = var->name.find_first_of("[", 0);
    if(i != std::string::npos){
        var->array_type = true;
        size_t j = var->name.find_first_of("]", i);
        std::string array_deff = var->name.substr(i, j-i);
        var->name = var->name.substr(0, i);
        var-> array_size = (size_t)atoi(array_deff.substr(1, array_deff.length()-1).c_str());
    }else{
        var->array_type = false;
        var->array_size = 0;
    }

    return var;
}
}


