#include <dirent.h>
#include "c_data.h"
#include "c_structures.h"
#include "Framework.h"

extern "C" {

int oclsGetDataPointer(Data d, float **ptr_out) {
    try {
        *ptr_out = reinterpret_cast<ocls::Data *>(d)->getDataPointer();
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        *ptr_out = NULL;
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsGetDataPointerWithBounds(Data d, size_t bounds[3], float** ptr_out){
    try {
        ocls::Data* cd = reinterpret_cast<ocls::Data *>(d);
        *ptr_out = cd->getDataPointer();
        memcpy(&bounds[0], cd->getUnpaddedDimSize(), sizeof(size_t)*3);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        *ptr_out = NULL;
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

Parameter oclsGetParameter(Data d) {
    ocls::Program::Parameter param = reinterpret_cast<ocls::Data *>(d)->getParameter();
    return *reinterpret_cast<Parameter*>(&param);
}

int oclsUpload(Data d, float *data, size_t *size, size_t *offset) {
    try {
        reinterpret_cast<ocls::Data *>(d)->upload(data, size, offset);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

float oclsAtX(Data d, size_t x) {
    return reinterpret_cast<ocls::Data *>(d)->at(x);
}
int oclsAtXSafe(Data d, size_t x, float* out) {
    try {
        *out = reinterpret_cast<ocls::Data *>(d)->at(x);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

float oclsAtXY(Data d, size_t x, size_t y) {
    return reinterpret_cast<ocls::Data *>(d)->at(x, y);
}
int oclsAtXYSafe(Data d, size_t x, size_t y, float* out) {
    try {
        *out = reinterpret_cast<ocls::Data *>(d)->at(x, y);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

float oclsAtXYZ(Data d, size_t x, size_t y, size_t z) {
    return reinterpret_cast<ocls::Data *>(d)->at(x, y, z);
}
int oclsAtXYZSafe(Data d, size_t x, size_t y, size_t z, float* out) {
    try {
        *out = reinterpret_cast<ocls::Data *>(d)->at(x, y, z);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsCopy(Data src, Data dest) {
    try {
        reinterpret_cast<ocls::Data *>(dest)->copy(reinterpret_cast<ocls::Data *>(src));
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsFindMax(Data d, float *value_out) {
    try {
        reinterpret_cast<ocls::Data *>(d)->max(value_out);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsFindMin(Data d, float *value_out) {
    try {
        reinterpret_cast<ocls::Data *>(d)->min(value_out);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsDimensions(Data d) {
    return (int)reinterpret_cast<ocls::Data *>(d)->getDim();
}

int oclsSize(Data d, size_t *size) {
    try {
        size_t* tmp = reinterpret_cast<ocls::Data *>(d)->getUnpaddedDimSize();
        memcpy(size, tmp, sizeof(size_t)*3);
    } catch (std::exception &e) {
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

}



