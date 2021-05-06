#include "PPPaintBatch.h"

/**
 * Default fragment shader
 *
 * This trick uses C++11 raw string literals to put the shader in a separate
 * file without having to guarantee its presence in the asset directory.
 * However, to work properly, the #include statement below MUST be on its
 * own separate line.
 */
const std::string oglShaderFrag =
#include "SplatShader.frag"
;

/**
 * Default vertex shader
 *
 * This trick uses C++11 raw string literals to put the shader in a separate
 * file without having to guarantee its presence in the asset directory.
 * However, to work properly, the #include statement below MUST be on its
 * own separate line.
 */
const std::string oglShaderVert =
#include "SplatShader.vert"
;

PaintBatch::PaintBatch() :
    _initialized(false),
    _active(false),
    _vertData(nullptr),
    _indxData(nullptr),
    _vertMax(0),
    _vertSize(0),
    _indxMax(0),
    _indxSize(0),
    _vertTotal(0),
    _callTotal(0) {
    _shader = nullptr;
    _vertbuff = nullptr;
    _unifbuff = nullptr;

}