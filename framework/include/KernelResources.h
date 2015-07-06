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


#ifndef KERNELS_H
#define KERNELS_H

namespace dt {
#include "cl_src/dt.h"
}
namespace header {
#include "cl_src/header.h"
}
namespace helper {
#include "cl_src/helper.h"
}
namespace utility_kernels {
#include "cl_src/utility_kernels.h"
}
namespace limiters {
#include "cl_src/limiters.h"
}
namespace reconstructors {
#include "cl_src/reconstructors.h"
}
namespace default_programs{
#include "cl_src/default.h"
}

#endif
