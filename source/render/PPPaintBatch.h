#ifndef PANICPAINTER_PPPAINTBRUSH_H
#define PANICPAINTER_PPPAINTBRUSH_H

#include "PPHeader.h"
#include <SDL/SDL.h>
#include <vector>

// Default memory sizes
#define DEFAULT_CAPACITY  1024
namespace utils {
	class PaintBatch {
	private:
        /** Whether this sprite batch has been initialized yet */
        bool _initialized;
        /** Whether this sprite batch is currently active */
        bool _active;
		/** The shader for this sprite batch */
		std::shared_ptr<Shader> _shader;
		/** The vertex buffer for this sprite batch */
		std::shared_ptr<VertexBuffer>  _vertbuff;
		/** The vertex buffer for this sprite batch */
		std::shared_ptr<UniformBuffer> _unifbuff;
        /** The sprite batch vertex mesh */
        SpriteVertex3* _vertData;
        /** The vertex capacity of the mesh */
        unsigned int _vertMax;
        /** The number of vertices in the current mesh */
        unsigned int _vertSize;
        std::shared_ptr<Mat4> _perspective;
        /** The indices for the vertex mesh */
        GLuint* _indxData;
        /** The index capacity of the mesh */
        unsigned int _indxMax;
        /** The number of indices in the current mesh */
        unsigned int _indxSize;

        // Monitoring values
/** The number of vertices drawn in this pass (so far) */
        unsigned int _vertTotal;
        /** The number of OpenGL calls in this pass (so far) */
        unsigned int _callTotal;

	public: 
        /**
     * Creates a degenerate sprite batch with no buffers.
     *
     * You must initialize the buffer before using it.
     */
        PaintBatch();

        /**
         * Deletes the sprite batch, disposing all resources
         */
        ~PaintBatch() { dispose(); }

        /**
         * Deletes the vertex buffers and resets all attributes.
         *
         * You must reinitialize the sprite batch to use it.
         */
        void dispose();

        /**
         * Initializes a sprite batch with the default vertex capacity.
         *
         * The default vertex capacity is 8192 vertices and 8192*3 = 24576 indices.
         * If the mesh exceeds these values, the sprite batch will flush before
         * before continuing to draw. Similarly uniform buffer is initialized with
         * 512 buffer positions. This means that the uniform buffer is comparable
         * in memory size to the vertices, but only allows 512 gradient or scissor
         * mask context switches before the sprite batch must flush. If you wish to
         * increase (or decrease) the capacity, use the alternate initializer.
         *
         * The sprite batch begins with no active texture, and the color white.
         * The perspective matrix is the identity.
         *
         * @return true if initialization was successful.
         */
        bool init();
        /**
   * Returns a new sprite batch with the default vertex capacity.
   *
   * The default vertex capacity is 8192 vertices and 8192*3 = 24576 indices.
   * If the mesh exceeds these values, the sprite batch will flush before
   * before continuing to draw. Similarly uniform buffer is initialized with
   * 512 buffer positions. This means that the uniform buffer is comparable
   * in memory size to the vertices, but only allows 512 gradient or scissor
   * mask context switches before the sprite batch must flush. If you wish to
   * increase (or decrease) the capacity, use the alternate allocator.
   *
   * The sprite batch begins with no active texture, and the color white.
   * The perspective matrix is the identity.
   *
   * @return a new sprite batch with the default vertex capacity.
   */
        static std::shared_ptr<PaintBatch> alloc() {
            std::shared_ptr<PaintBatch> result = std::make_shared<PaintBatch>();
            return (result->init() ? result : nullptr);
        }
#pragma mark -
#pragma mark Attributes
        /**
     * Returns true if this sprite batch has been initialized and is ready for use.
     *
     * @return true if this sprite batch has been initialized and is ready for use.
     */
        bool isReady() const { return _initialized; }

        /**
         * Returns whether this sprite batch is actively drawing.
         *
         * A sprite batch is in use if begin() has been called without the
         * requisite end() to flush the pipeline.
         *
         * @return whether this sprite batch is actively drawing.
         */
        bool isDrawing() const { return _active; }

        /**
         * Returns the number of vertices drawn in the latest pass (so far).
         *
         * This value will be reset to 0 whenever begin() is called.
         *
         * @return the number of vertices drawn in the latest pass (so far).
         */
        unsigned int getVerticesDrawn() const { return _vertTotal; }

        /**
         * Returns the number of OpenGL calls in the latest pass (so far).
         *
         * This value will be reset to 0 whenever begin() is called.
         *
         * @return the number of OpenGL calls in the latest pass (so far).
         */
        unsigned int getCallsMade() const { return _callTotal; }
        void setViewport(const Vec2 res);

        /**
         *Sets the splatter locations for this batch.
         *
         *@param
         */
        void setSplats(const Vec2 s1, const Vec2 s2, const Vec2 s3, const Vec2 s4, const Vec4 c1, const Vec4 c2, const Vec4 c3, const Vec4 c4);

        /**
         * Sets the active perspective matrix of this sprite batch
         *
         * The perspective matrix is the combined modelview-projection from the
         * camera. By default, this is the identity matrix.
         *
         * @param perspective   The active perspective matrix for this sprite batch
         */
        void setPerspective(const Mat4& perspective);

        /**
         * Returns the active perspective matrix of this sprite batch
         *
         * The perspective matrix is the combined modelview-projection from the
         * camera.  By default, this is the identity matrix.
         *
         * @return the active perspective matrix of this sprite batch
         */
        const Mat4& getPerspective() const {
            return *_perspective;
        }
#pragma mark -
#pragma mark Rendering
        /**
    * Starts drawing with the current perspective matrix.
    *
    * This call will disable depth buffer writing. It enables blending and
    * texturing. You must call either {@link #flush} or {@link #end} to
    * complete drawing.
    *
    * Calling this method will reset the vertex and OpenGL call counters to 0.
    */
        void begin();
        /**
     * Completes the drawing pass for this sprite batch, flushing the buffer.
     *
     * This method enables depth writes and disables blending and texturing. It
     * must always be called after a call to {@link #begin}.
     */
        void end();

        /**
         * Flushes the current mesh without completing the drawing pass.
         *
         * This method is called whenever you change any attribute other than color
         * mid-pass. It prevents the attribute change from retoactively affecting
         * previuosly drawn shapes.
         */
        void flush();
    };
}
#endif //PANICPAINTER_PPPAINTBRUSH_H