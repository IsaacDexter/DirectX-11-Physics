#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Vector3.h"

//https://en.wikipedia.org/wiki/Drag_coefficient
#define SPHERE_DRAG_COEFFICIENT 0.47f
#define HALF_SPHERE_DRAG_COEFFICIENT 0.42f
#define CONE_DRAG_COEFFICIENT 0.5f
#define CUBE_DRAG_COEFFICIENT 1.05f
#define ANGLED_CUBE_DRAG_COEFFICIENT 0.8f
#define LONG_CYLINDER_DRAG_COEFFICIENT 0.82f
#define SHORT_CYLINDER_DRAG_COEFFICIENT 0.82f

//Surface area of the frontal, plan and elevation cross sections, for unit shapes
#define SPHERE_REFERENCE_AREA Vector3(0.785f, 0.785f, 0.785f)
#define HALF_SPHERE_REFERENCE_AREA Vector3(0.785f, 0.393f, 0.393f)
#define CONE_REFERENCE_AREA Vector3(0.393f, 1.0f, 1.0f)
#define CUBE_REFERENCE_AREA Vector3(1.0f, 1.0f, 1.0f)
#define ANGLED_CUBE_REFERENCE_AREA Vector3(1.414f, 1.0f, 1.414f)
#define LONG_CYLINDER_REFERENCE_AREA Vector3(0.785f, 1.0f, 1.0f)
#define SHORT_CYLINDER_REFERENCE_AREA Vector3(0.785f, 0.5f, 0.5f)

//https://en.wikipedia.org/wiki/Density_of_air
#define AIR_FLUID_DENSITY 1.204f

//https://en.wikipedia.org/wiki/Standard_gravity
#define EARTH_GRAVITY_ACCELERATION -0.981f

//https://en.wikipedia.org/wiki/Friction
#define STEEL_STEEL_STATIC_FRICTION_COEFFICIENT 0.74f
#define STEEL_STEEL_DYNAMIC_FRICTION_COEFFICIENT 0.42f

#define ANGULAR_DAMPING 0.90f

#endif // !CONSTANTS_H
