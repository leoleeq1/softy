#ifndef GEOMETRY_GENERATOR_H_
#define GEOMETRY_GENERATOR_H_

#include <memory>

#include "render/mesh.h"

namespace softy {
std::unique_ptr<Mesh> CreateCubeVertex8();
std::unique_ptr<Mesh> CreateCube();
std::unique_ptr<Mesh> CreateQuad();
}  // namespace softy

#endif  // GEOMETRY_GENERATOR_H_