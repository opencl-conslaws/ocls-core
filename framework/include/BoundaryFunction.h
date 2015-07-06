#ifndef BOUNDARY_FUNCTION_H
#define BOUNDARY_FUNCTION_H


#include "CLSSource.h"
#include "ProgramLauncher.h"
#include "CallableFunction.h"

namespace ocls {

/**
*  Coordinate systems are cool
*
*  1D:
*     NEG_X ├───────────┤ POS_X     ·───>
*                                       X
*  2D:
*               POS_Y
*           ┌───────────┐
*           │           │
*     NEG_X │           │ POS_X     ^ Y
*           │           │           │
*           │           │           ·───>
*           └───────────┘               X
*               NEG_Y
*
*  3D:
*                ┌───────────┐
*               /     NEG_Z /│
*             / POS_Y     /  │          ^ Y
*           ┌───────────┐    │          │
*           │           │    │          ·───>
*     NEG_X │  POS_Z    │ POS_X       /     X
*           │           │   /       └ Z
*           │           │ /
*           └───────────┘
*               NEG_Y
*/

enum OnBoundary{
    POS_X, NEG_X,
    POS_Y, NEG_Y,
    POS_Z, NEG_Z,

    /**
    * Redefinitions for easier naming conventions.
    */
    RIGHT = POS_X,
    EAST = POS_X,

    LEFT = NEG_X,
    WEST = NEG_X,

    NORTH = POS_Y,
    TOP = POS_Y ,
    UP = POS_Y,

    SOUTH = NEG_Y,
    BOTTOM = NEG_Y,
    DOWN = NEG_Y,

    FRONT = POS_Z,
    NEAR = POS_Z,

    BACK = NEG_Z,
    FAR = NEG_Z
};

/** \ingroup main_api
*  @{
*/
class BoundaryFunction :
    public CallableFunction{
public:

    BoundaryFunction(Domain* domain, Framework* framework, CLSSource::Function* function);
    virtual ~BoundaryFunction();

protected:
    /**
    * Launch boundary program
    */
    virtual ReturnType call(std::vector<Program::Parameter>& params);

private:
    Program* m_program[6]; //< One kernel for each boundary

};
/** @} */


}

#endif