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

/**
 * Deletes the vertex buffers and resets all attributes.
 *
 * You must reinitialize the sprite batch to use it.
 */
void PaintBatch::dispose() {
    if (_vertData) {
        delete[] _vertData; _vertData = nullptr;
    }
    if (_indxData) {
        delete[] _indxData; _indxData = nullptr;
    }
    _shader = nullptr;
    _vertbuff = nullptr;
    _unifbuff = nullptr;

    _vertMax = 0;
    _vertSize = 0;
    _indxMax = 0;
    _indxSize = 0;

    _vertTotal = 0;
    _callTotal = 0;

    _initialized = false;
    _active = false;
}
/**
 * Initializes a sprite batch with the default vertex capacity.
 *
 * The default vertex capacity is 1024 vertices and 1024*3 = 3072 indices.
 * If the mesh exceeds these values, the sprite batch will flush before
 * before continuing to draw. Similarly uniform buffer is initialized with
 * 512 buffer positions. This means that the uniform buffer is comparable
 * in memory size to the vertices, but only allows 512 gradient or scissor
 * mask context switches before the sprite batch must flush. If you wish to
 * increase (or decrease) the capacity, use the alternate initializer.
 *
 * The sprite batch begins with the default blank texture, and color white.
 * The perspective matrix is the identity.
 *
 * @return true if initialization was successful.
 */
bool PaintBatch::init() {
    if (_initialized) {
        CUAssertLog(false, "PaintBatch is already initialized");
        return false; // If asserts are turned off.
    }

    _shader = Shader::alloc(SHADER(oglShaderVert), SHADER(oglShaderFrag));

   /* _vertbuff = VertexBuffer::alloc(sizeof(SpriteVertex3));
    _vertbuff->setupAttribute("aPosition", 3, GL_FLOAT, GL_FALSE, 0);
    _vertbuff->setupAttribute("aColor", 4, GL_FLOAT, GL_TRUE,
        offsetof(cugl::SpriteVertex3, color));
    _vertbuff->setupAttribute("aTexCoord", 2, GL_FLOAT, GL_FALSE,
        offsetof(cugl::SpriteVertex3, texcoord));
    _vertbuff->attach(_shader);*/

    // Set up data arrays;
    _vertMax = DEFAULT_CAPACITY;
    _vertData = new SpriteVertex3[_vertMax];
    _indxMax = DEFAULT_CAPACITY * 3;
    _indxData = new GLuint[_indxMax];

    // Create uniform buffer (this has its own backing array)
    _unifbuff = UniformBuffer::alloc(25 * sizeof(float), DEFAULT_CAPACITY / 16);

    // Layout std140 format
    _unifbuff->setOffset("uS1", 0);
    _unifbuff->setOffset("uS2", 8);
    _unifbuff->setOffset("uS3", 16);
    _unifbuff->setOffset("uS4", 24);
    _unifbuff->setOffset("uC1", 40);
    _unifbuff->setOffset("uC2", 56);
    _unifbuff->setOffset("uC3", 72);
    _unifbuff->setOffset("uC4", 86);

    _shader->setUniformBlock("uContext", _unifbuff);


    //Set all needed uniforms here
    //TODO:
    //Vec2 tempVec = ;
//    _shader->setUniformVec2("uSplatPosition", Vec2(0,0));

    _shader->setUniformVec2("uViewport", Vec2(0, 0));
}