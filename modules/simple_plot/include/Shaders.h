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



#ifndef SHADERS_H
#define SHADERS_H

namespace surface_vert {
#include "glsl_src/surface_vert.h"
}
namespace surface_frag {
#include "glsl_src/surface_frag.h"
}

namespace graph_vert {
#include "glsl_src/graph_vert.h"
}
namespace graph_frag {
#include "glsl_src/graph_frag.h"
}

namespace volume_pone_vert {
#include "glsl_src/volume_pone_vert.h"
}
namespace volume_pone_frag {
#include "glsl_src/volume_pone_frag.h"
}

namespace volume_ptwo_vert {
#include "glsl_src/volume_ptwo_vert.h"
}
namespace volume_ptwo_frag {
#include "glsl_src/volume_ptwo_frag.h"
}


#endif
