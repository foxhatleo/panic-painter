#include "PPPaintBatch.h"

/**
 * Default fragment shader
 *
 * This trick uses C++11 raw string literals to put the shader in a separate
 * file without having to guarantee its presence in the asset directory.
 * However, to work properly, the #include statement below MUST be on its
 * own separate line.
 */
const std::string splatShaderFrag =
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
const std::string splatShaderVert =
#include "SplatShader.vert"
;

PaintBatch::PaintBatch() :
    _initialized(false),
    _active(false),
    _vertData(nullptr),
    _indxData(nullptr),
    _perspectiveChanged(false),
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
 * The default vertex capacity is 1 vertices and 1*3 = 3 indices.
 * If the mesh exceeds these values, the paint batch will flush before
 * before continuing to draw. 
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
    ptr<Shader> storeShader = Shader::alloc(SHADER(splatShaderVert), SHADER(splatShaderFrag));
    _shader = storeShader;


    _vertbuff = VertexBuffer::alloc(sizeof(PaintVertex));
    _vertbuff->setupAttribute("aPosition", 2, GL_FLOAT, GL_FALSE, 0);
    _vertbuff->attach(_shader);

    // Set up data arrays;
    _vertMax = 12;
    _vertData = new PaintVertex[_vertMax];
    _indxMax = _vertMax*3;
    _indxData = new GLuint[_indxMax];

    // Create uniform buffer (this has its own backing array)
    _unifbuff = UniformBuffer::alloc(26 * sizeof(float));

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
    _shader->setUniformVec2("uViewport", Vec2(0, 0));
    return true; 
}

void PaintBatch::setViewport(const Vec2 res) {
    _shader->setUniformVec2("uViewport", res);
}

void PaintBatch::setSplats(const Vec2 s1, const Vec2 s2, const Vec2 s3, const Vec2 s4, const Vec4 c1, const Vec4 c2, const Vec4 c3, const Vec4 c4) {
    _unifbuff->setUniformVec2(0, "uS1", s1);
    _unifbuff->setUniformVec2(0, "uS2", s2);
    _unifbuff->setUniformVec2(0, "uS3", s3);
    _unifbuff->setUniformVec2(0, "uS4", s4);
    _unifbuff->setUniformVec4(0, "uC1", c1);
    _unifbuff->setUniformVec4(0, "uC2", c2);
    _unifbuff->setUniformVec4(0, "uC3", c3);
    _unifbuff->setUniformVec4(0, "uC4", c4);

}
void PaintBatch::setPerspective(const Mat4& perspective) {
        auto matrix = std::make_shared<Mat4>(perspective);
        _perspective = matrix;
        _perspectiveChanged = true; 
}


void PaintBatch::begin(const Mat4& perspective) {
    setPerspective(perspective);
    glDisable(GL_CULL_FACE);
    glDepthMask(true);
    glEnable(GL_BLEND);

    // DO NOT CLEAR.  This responsibility lies elsewhere
    _shader->bind();
    _vertbuff->bind();
    _unifbuff->bind(false);
    _unifbuff->deactivate();
    _active = true;
    _callTotal = 0;
    _vertTotal = 0;
}

void PaintBatch::end() {
    CUAssertLog(_active, "PaintBatch is not active");
    flush();
    _shader->unbind();
    _active = false;
}
void PaintBatch::flush() {
    if (_indxSize == 0 || _vertSize == 0) {
        return;
    }
    _vertbuff->loadVertexData(_vertData, _vertSize);
    _vertbuff->loadIndexData(_indxData, _indxSize);
    _unifbuff->activate();
    _unifbuff->flush();

    if (_perspectiveChanged) {
        _shader->setUniformMat4("uPerspective", *_perspective);
        _perspectiveChanged = false; 
    }
    _unifbuff->deactivate();

    // Increment the counters
    _vertTotal += _indxSize;

    _vertSize = _indxSize = 0;
}

void PaintBatch::prepare(Vec2 tl, Vec2 tr, Vec2 bl, Vec2 br) {
    _vertData[_vertSize].position = tl;
    GLuint indTl = _vertSize;
    _vertSize++;

    _vertData[_vertSize].position = tr;
    GLuint indTr = _vertSize;
    _vertSize++;

    _vertData[_vertSize].position = bl;
    GLuint indBl = _vertSize;
    _vertSize++;

    _vertData[_vertSize].position = br;
    GLuint indBr = _vertSize;
    _vertSize++;
    // Add indices counterclockwise
        // Top left triangle
        _indxData[_indxSize] = indTr;
    _indxSize++;
    _indxData[_indxSize] = indTl;
    _indxSize++;
    _indxData[_indxSize] = indBl;
    _indxSize++;

    // Bottom right triangle
    _indxData[_indxSize] = indTr;
    _indxSize++;
    _indxData[_indxSize] = indBl;
    _indxSize++;
    _indxData[_indxSize] = indBr;
    _indxSize++;
}