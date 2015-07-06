/**
* C Wrapper
*/

#ifndef C_VISUALIZER_H
#define C_VISUALIZER_H

#ifdef __cplusplus
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

#else
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#endif

#include "c_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef PointerHandle Visualizer;
typedef PointerHandle Figure;

/**
* Initialize API
*/
int oclsVizInit(Framework framework, Visualizer* visualizer);

/**
 *
 */
int oclsVisPlot(Visualizer visualizer, Data data, Domain domain, Figure* figure);

/**
 *
 */
int oclsVisUpdate(Figure figure, Data data);

#ifdef __cplusplus
}
#endif

#endif