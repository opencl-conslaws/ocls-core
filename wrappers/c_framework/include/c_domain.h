#ifndef C_DOMAIN_H
#define C_DOMAIN_H

#ifdef __cplusplus
extern "C" {
#endif
/**
*  \defgroup c_wrap_domain Domain
*  \ingroup c_wrap
*  @{
*/

/**
* Set the number of dimensions
*/
int oclsSetDimensions(Domain* domain, const unsigned short dimensions_);

/**
* @{
* Set the physical domain bounds
*/
int oclsSetBoundsX(Domain* domain, const float left_, const float right_);
int oclsSetBoundsXY(Domain* domain, const float left_, const float right_, const float bottom_, const float top_);
int oclsSetBoundsXYZ(Domain* domain, const float left_, const float right_, const float bottom_, const float top_, const float front_, const float back_);
/** @}*/

/**
* @{
* Set the mesh refinement in cell count
*/
int oclsSetMeshX(Domain* domain, const size_t x_);
int oclsSetMeshXY(Domain* domain, const size_t x_, const size_t y_);
int oclsSetMeshXYZ(Domain* domain, const size_t x_, const size_t y_, const size_t z_);
/** @}*/

/**
* Set the number of ghost cells
*/
int oclsSetGhostCells(Domain* domain, const unsigned short ghost_cells_);

/**
* Find Delta
*/
float oclsGetDelta(Domain domain, const unsigned short dimension);


/** @}*/

#ifdef __cplusplus
}
#endif

#endif