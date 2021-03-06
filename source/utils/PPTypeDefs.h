#ifndef PANICPAINTER_PPTYPEDEFS_H
#define PANICPAINTER_PPTYPEDEFS_H

#include <cugl/cugl.h>

// TypeDefs are basically aliases of types.
// The following are templated type aliasing.

//                             ALIASES         ORIGINAL TYPES
// =============================================================================
template <typename T> using    ptr           = shared_ptr<T>;
template <typename T> using    vec           = vector<T>;

// The following are regular typedefs.

//       ORIGINAL TYPES                        ALIASES
// =============================================================================
typedef  ptr<cugl::AssetManager>               asset_t;
typedef  ptr<cugl::JsonValue>                  json_t;
typedef  unsigned int                          uint;

#endif //PANICPAINTER_PPTYPEDEFS_H
