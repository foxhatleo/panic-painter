#ifndef PANICPAINTER_PPTYPEDEFS_H
#define PANICPAINTER_PPTYPEDEFS_H

#include <cugl/cugl.h>

// TypeDefs are basically aliases of types.
// The following are templated type aliasing.

//                             ALIASES         ORIGINAL TYPES
// =============================================================================
template <typename T> using    ptr           = std::shared_ptr<T>;
template <typename T> using    vec           = std::vector<T>;

// The following are regular typedefs.

//       ORIGINAL TYPES                        ALIASES
// =============================================================================
typedef  shared_ptr<cugl::AssetManager>        asset;
typedef  ptr<cugl::JsonValue>                  json;
typedef  unsigned int                          uint;

#endif //PANICPAINTER_PPTYPEDEFS_H
