//
//  CURect.cpp
//  Cornell University Game Library (CUGL)
//
//  This module provides support for a 2d bouding rectangle.  This rectangle
//  is not intended for drawing.  Use CUPoly2 instead for rectangle graphics.
//
//  Because math objects are intended to be on the stack, we do not provide
//  any shared pointer support in this class.
//
//  CUGL MIT License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 2/25/20
#include <algorithm>
#include <sstream>
#include <cugl/math/CURect.h>
#include <cugl/math/CUAffine2.h>
#include <cugl/math/CUMat4.h>

using namespace cugl;

#pragma mark -
#pragma mark Setters
    /**
     * Assigns this rect to have the given position and dimensions.
     *
     * @param x      The x-coordinate of the bottom left corner
     * @param y      The y-coordinate of the bottom left corner
     * @param width  The width of the rect
     * @param height The width of the rect
     *
     * @return This rectangle, after assignment
     */
    Rect& Rect::set(float x, float y, float width, float height) {
        origin.set(x,y); 
        size.set(width,height);
        return *this;
    }

    /**
     * Sets the elements of this rect from the values in the specified array.
     *
     * @param array An array containing the elements in the order origin, size.
     *
     * @return This rectangle, after assignment
     */
    Rect& Rect::set(const float* array) {
        origin = &array[0]; 
        size = &array[2];
        return *this;
    }
    
    /**
     * Assigns this rect to have the given position and dimensions.
     *
     * @param pos   The position the bottom left corner
     * @param dimen The size of the rect
     *
     * @return This rectangle, after assignment
     */
    Rect& Rect::set(const Vec2 pos, const Size dimen) {
        origin.set(pos); 
        size.set(dimen);
        return *this;
    }
    
    /**
     * Assigns this rect to be a copy of the given rectangle.
     *
     * @param other The rectangle to copy
     *
     * @return This rectangle, after assignment
     */
    Rect& Rect::set(const Rect& other) {
        origin.set(other.origin); 
        size.set(other.size);
        return *this;
    }

#pragma mark -
#pragma mark Comparisons
/**
 * Returns true if this rect properly fits inside of the given rect.
 *
 * Proper containment means that no edges of the two rects touch.
 *
 * This operator is provided for convenience.  However, this does not
 * provide a total order, making it unsafe for std::sort.
 *
 * @param rect The potentially larger rect
 *
 * @return True if this rect properly fits inside of the given rect.
 */
bool Rect::operator<(const Rect rect) const {
    bool horz = rect.getMinX() < getMinX() && getMaxX() < rect.getMaxX();
    bool vert = rect.getMinY() < getMinY() && getMaxY() < rect.getMaxY();
    return horz && vert;
}

/**
 * Returns true if this rect can properly hold the given rect.
 *
 * Proper containment means that no edges of the two rects touch.
 *
 * This operator is provided for convenience.  However, this does not
 * provide a total order, making it unsafe for std::sort.
 *
 * @param rect The potentially smaller rect
 *
 * @return True if this rect can properly hold the given rect.
 */
bool Rect::operator>(const Rect rect) const {
    bool horz = getMinX() < rect.getMinX() && rect.getMaxX() < getMaxX();
    bool vert = getMinY() < rect.getMinY() && rect.getMaxY() < getMaxY();
    return horz && vert;
}

/**
 * Returns true if this rect fits inside of the given rect.
 *
 * This method does not test for strict containment.  To test for
 * strict containment, use the comparison operator <.
 *
 * @param rect The potentially larger rect
 *
 * @return True if this rect fits inside of the given rect.
 */
bool Rect::inside(const Rect rect) const {
    bool horz = rect.getMinX() <= getMinX() && getMaxX() <= rect.getMaxX();
    bool vert = rect.getMinY() <= getMinY() && getMaxY() <= rect.getMaxY();
    return horz && vert;
}

/**
 * Returns true if this rect can hold the given rect.
 *
 * This method does not test for strict containment.  To test for
 * strict containment, use the comparison operator >.
 *
 * @param rect The potentially smaller rect
 *
 * @return True if this rect can hold the given rect.
 */
bool Rect::contains(const Rect rect) const {
    bool horz = getMinX() <= rect.getMinX() && rect.getMaxX() <= getMaxX();
    bool vert = getMinY() <= rect.getMinY() && rect.getMaxY() <= getMaxY();
    return horz && vert;
}


/**
 * Returns true if the given point is on the boundar of this rect.
 *
 * Combining this method with contains() allows you to test for strict
 * containment.
 *
 * @param point The point to test
 *
 * @return True if the given point is on the boundar of this rect.
 */
bool Rect::touches(const Vec2 point) const {
    bool horz = (getMinX() == point.x || getMaxX() == point.x) && 
                 getMinY() <= point.y && point.y <= getMaxY();
    bool vert = (getMinY() == point.y || getMaxY() == point.y) && 
                 getMinX() <= point.x && point.x <= getMaxX();
    return horz || vert;
}

/**
 * Returns true if this rect contains the given point.
 *
 * This method does not test for strict containment; it includes the
 * boundardy.  To test for strict containment, combine this with the
 * method touches().
 *
 * @param point The point to test
 *
 * @return True if this rect contains the given point.
 */
bool Rect::contains(const Vec2 point) const {
    return  (getMinX() <= point.x && point.x <= getMaxX() &&
             getMinY() <= point.y && point.y <= getMaxY());
}

/**
 * Returns true if this rect contains the given circle.
 *
 * This method does not test for strict containment; it includes the
 * boundary of both the circle and the rectangle.
 *
 * @param center The center of the circle
 * @param radius The radius of the circle
 *
 * @return True if this rect contains the given circle.
 */
bool Rect::contains(const Vec2 center, float radius) const {
    Rect bounds(center.x-radius,center.y-radius,2*radius,2*radius);
    return contains(bounds);
}

/**
 * Returns true if this rect intersects the other.
 *
 * This method allows for intersections where the edges of the rects
 * are touching.  In this case, the size of the intersection is empty.
 *
 * @param rect The rect to test
 *
 * @return true if this rect intersects the other.
 */
bool Rect::doesIntersect(const Rect rect) const {
    return !(getMaxX() < rect.getMinX() || rect.getMaxX() < getMinX() ||
             getMaxY() < rect.getMinY() || rect.getMaxY() < getMinY());
}

/**
 * Returns true if this rect intersects the given circle.
 *
 * This method allows for intersections where the edge of the rect
 * simply touches the boundary of the circle.
 *
 * @param center The center of the circle
 * @param radius The radius of the circle
 *
 * @return true if this rect intersects the given circle.
 */
bool Rect::doesIntersect(const Vec2 center, float radius) const {
    Vec2 rectangleCenter((origin.x + size.width / 2),
                         (origin.y + size.height / 2));
    
    float w = size.width / 2;
    float h = size.height / 2;
    
    float dx = fabs(center.x - rectangleCenter.x);
    float dy = fabs(center.y - rectangleCenter.y);
    
    if (dx > (radius + w) || dy > (radius + h)) {
        return false;
    }
    
    Vec2 circleDistance(fabs(center.x - origin.x - w),
                        fabs(center.y - origin.y - h));
    
    if (circleDistance.x <= (w)) {
        return true;
    }
    
    if (circleDistance.y <= (h)) {
        return true;
    }
    
    float cornerDistanceSq = powf(circleDistance.x - w, 2) +
    powf(circleDistance.y - h, 2);
    return (cornerDistanceSq <= (powf(radius, 2)));
}

#pragma mark -
#pragma mark Transforms
/**
 * Translates this rectangle by the given vector.
 *
 * The size of the rectangle is unaffected.  To alter the rectangle size, scale the 
 * rectangle.
 *
 * @param vec   The translation vector
 *
 * @return This rectangle, translated.
 */
Rect& Rect::translate(const Vec2 vec) {
    origin += vec;
    return *this;
}

/**
 * Translates this rectangle by the given vector.
 *
 * The size of the rectangle is unaffected.  To alter the rectangle size, scale the 
 * rectangle.
 *
 * @param x      The translation x amount
 * @param y      The translation y amount
 *
 * @return This rectangle, translated.
 */
Rect& Rect::translate(float x, float y) {
    origin.x += x;
    origin.y += y;
    return *this;
}

/**
 * Returns the translation of this rectangle by the given vector.
 *
 * The size of the rectangle is unaffected.  To alter the rectangle size, scale the 
 * rectangle.
 *
 * @param vec   The translation vector
 *
 * @return the translation of this rectangle by the given vector.
 */
Rect Rect::getTranslation(const Vec2 vec) const {
    Rect rect(*this);
    return rect.translate(vec);
}

/**
 * Returns the translation of this rectangle by the given vector.
 *
 * The size of the rectangle is unaffected.  To alter the rectangle size, scale the 
 * rectangle.
 *
 * @param x      The translation x amount
 * @param y      The translation y amount
 *
 * @return the translation of this rectangle by the given vector.
 */
Rect Rect::getTranslation(float x, float y) const {
    Rect rect(*this);
    return rect.translate(x,y);
}

/**
 * Uniformly the scales the size of this rectangle.
 *
 * The origin of the rectangle is unaffected.  To move the origin, 
 * translate the rectangle.
 *
 * @param scale The uniform scaling factor
 *
 * @return This rectangle, scaled uniformly.
 */
Rect& Rect::scale(float scale) {
    size *= scale;
    return *this;
}

/**
 * Nonuniformly scales the size of this rectangle.
 *
 * The origin of the rectangle is unaffected.  To move the origin, 
 * translate the rectangle.
 *
 * @param sx   The x-axis scale factor
 * @param sy   The y-axis scale factor
 *
 * @return This polygon, scaled non-uniformly.
 */
Rect& Rect::scale(float x, float y) {
    size *= Vec2(x,y);
    return *this;
}

/**
 * Nonuniformly scales the size of this rectangle.
 *
 * The origin of the rectangle is unaffected.  To move the origin, 
 * translate the rectangle.
 *
 * @param scale The non-uniform scaling factor
 *
 * @return This polygon, scaled non-uniformly.
 */
Rect& Rect::scale(const Vec2 scale) {
    size *= scale;
    return *this;
}

/**
 * Nonuniformly scales this rectangle.
 *
 * The origin scale is the given anchor point, which is a percentage of the
 * rectangle. (0,0) is the origin (bottom left corner) of the rectangle, while 
 * (1,1) is the top right corner of the rectangle.
 *
 * @param scale    The non-uniform scaling factor
 * @param anchor   The anchor for the scale origin point.
 *
 * @return This polygon, scaled non-uniformly.
 */
Rect& Rect::scale(const Vec2 scale, const Vec2 anchor) {
    Size nsize = size*scale;
    origin += (size-nsize)*anchor;
    return *this;
}

/**
 * Returns the uniform scale of this rectangle.
 *
 * The origin of the rectangle is unaffected.  To move the origin, 
 * translate the rectangle.
 *
 * @param scale The uniform scaling factor
 *
 * @return the uniform scale of this rectangle.
 */
Rect Rect::getScale(float scale) const {
    Rect rect(*this);
    rect.scale(scale);
    return rect;
}

/**
 * Returns the non-uniform scale of this rectangle.
 *
 * The origin of the rectangle is unaffected.  To move the origin, 
 * translate the rectangle.
 *
 * @param sx   The x-axis scale factor
 * @param sy   The y-axis scale factor
 *
 * @return the non-uniform scale of this rectangle.
 */
Rect Rect::getScale(float x, float y) const {
    Rect rect(*this);
    rect.scale(x,y);
    return rect;
}

/**
 * Returns the non-uniform scale of this rectangle.
 *
 * The origin of the rectangle is unaffected.  To move the origin, 
 * translate the rectangle.
 *
 * @param scale The non-uniform scaling factor
 *
 * @return the non-uniform scale of this rectangle.
 */
Rect Rect::getScale(const Vec2 scale) const {
    Rect rect(*this);
    rect.scale(scale);
    return rect;
}

/**
 * Returns the non-uniform scale of this rectangle.
 *
 * The origin scale is the given anchor point, which is a percentage of the
 * rectangle. (0,0) is the origin (bottom left corner) of the rectangle, while 
 * (1,1) is the top right corner of the rectangle.
 *
 * @param scale    The non-uniform scaling factor
 * @param anchor   The anchor for the scale origin point.
 *
 * @return the non-uniform scale of this rectangle.
 */
Rect Rect::getScale(const Vec2 scale, const Vec2 anchor) const {
    Rect rect(*this);
    rect.scale(scale,anchor);
    return rect;
}

/**
 * Inscribes this rectangle inside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique inscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If the inscription is loose, exactly one diagonal is inscribed in the rectangle
 * (but not both). This creates a slightly larger rectangle that is still a 
 * reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the inscription
 *
 * @return This inscribing rectangle
 */
Rect& Rect::inscribe(const Mat4& transform, bool loose) {
    // Transform the rectangle around it center.
    Vec2 p[4];
    p[0] = origin;
    p[1] = origin+Vec2(size.width,0);
    p[2] = origin+size;
    p[3] = origin+Vec2(0,size.height);
    
    for(int ii = 0; ii < 4; ii++) {
        p[ii] *= transform;
    }
    
    // Find the center and rezero
    cugl::Vec2 c = origin+size/2;
    c *= transform;
    for(int ii = 0; ii < 4; ii++) {
        p[ii] -= c;
    }
    
    // Compute normal to two edges
    cugl::Vec2 n[2];
    
    n[0] = (p[1]-p[0]).getPerp();
    n[1] = (p[2]-p[1]).getPerp();
    
    // Find rotated axes
    cugl::Vec2 v[4];
    for(int ii = 0; ii < 4; ii++) {
        v[ii] = cugl::Vec2::getIntersection(p[ii], p[(ii+1) % 4], 
                                            cugl::Vec2::ZERO, n[ii % 2]);
    }
    
    // Find the best diagonal
    n[0].x = (std::max(v[0].x,v[2].x)-std::min(v[0].x,v[2].x))/2;
    n[0].y = (std::max(v[0].y,v[2].y)-std::min(v[0].y,v[2].y))/2;
    n[1].x = (std::max(v[1].x,v[3].x)-std::min(v[1].x,v[3].x))/2;
    n[1].y = (std::max(v[1].y,v[3].y)-std::min(v[1].y,v[3].y))/2;
    if (loose) {
        if (n[0].lengthSquared() < n[1].lengthSquared()) {
            n[0] = n[1];
        }
    } else {
        if (n[0].lengthSquared() > n[1].lengthSquared()) {
            n[0] = n[1];
        }
    }
    
    origin = c-n[0];
    size = 2*n[0];
    return *this;
}

/**
 * Inscribes this rectangle inside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique inscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If the inscription is loose, exactly one diagonal is inscribed in the
 * rectangle (but not both). This creates a slightly larger rectangle that
 * is still a reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose     Whether to loosen the inscription
 *
 * @return This inscribing rectangle
 */
Rect& Rect::inscribe(const Affine2& transform, bool loose) {
    // Transform the rectangle around it center.
    Vec2 p[4];
    p[0] = origin;
    p[1] = origin+Vec2(size.width,0);
    p[2] = origin+size;
    p[3] = origin+Vec2(0,size.height);
    
    for(int ii = 0; ii < 4; ii++) {
        p[ii] *= transform;
    }
    
    // Find the center and rezero
    cugl::Vec2 c = origin+size/2;
    c *= transform;
    for(int ii = 0; ii < 4; ii++) {
        p[ii] -= c;
    }
    
    // Find the containing box
    Vec2 bl = p[0];
    Vec2 tr = p[0];
    for(int ii = 1; ii < 4; ii++) {
        if (p[ii].x < bl.x) {
            bl.x = p[ii].x;
        } else if (p[ii].x > tr.x) {
            tr.x = p[ii].x;
        }
        if (p[ii].y < bl.y) {
            bl.y = p[ii].y;
        } else if (p[ii].y > tr.y) {
            tr.y = p[ii].y;
        }
    }
    
    // Get the two diagonals
    cugl::Vec2 n[2];
    n[0] = (tr-bl)/2;
    n[1] = n[0].getPerp();

    // Find diagonal intersections
    for(int ii = 0; ii < 2; ii++) {
        bool search = true;
        for(int jj = 0; search && jj < 4; jj++) {
            if (cugl::Vec2::doesSegmentIntersect(p[jj], p[(jj+1) % 4], cugl::Vec2::ZERO, n[ii])) {
                n[ii] = cugl::Vec2::getIntersection(p[jj], p[(jj+1) % 4],
                                                    cugl::Vec2::ZERO, n[ii]);
                search = false;
            }
        }
    }

    // Pick the shorter/longer diagonal
    if (loose) {
        if (n[0].lengthSquared() < n[1].lengthSquared()) {
            n[0] = n[1].getPerp();
        }
    } else {
        if (n[0].lengthSquared() > n[1].lengthSquared()) {
            n[0] = n[1].getPerp();
        }
    }
   
    origin = c-n[0];
    size = 2*n[0];
    return *this;
}

/**
 * Returns the rectangle inscribed inside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * returns the unique inscribing rectangle that shares the same center as the 
 * quadrilateral.
 *
 * If the inscription is loose, exactly one diagonal is inscribed in the rectangle
 * (but not both). This creates a slightly larger rectangle that is still a 
 * reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the inscription
 *
 * @return the rectangle inscribed inside the transformed quadrilateral.
 */
Rect Rect::getInscription(const Mat4& transform, bool loose) const {
    Rect rect(*this);
    rect.inscribe(transform,loose);
    return rect;
}

/**
 * Returns the rectangle inscribed inside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique inscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If the inscription is loose, exactly one diagonal is inscribed in the rectangle
 * (but not both). This creates a slightly larger rectangle that is still a 
 * reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the inscription
 *
 * @return the rectangle inscribed inside the transformed quadrilateral.
 */
Rect Rect::getInscription(const Affine2& transform, bool loose) const {
    Rect rect(*this);
    rect.inscribe(transform,loose);
    return rect;
}

/**
 * Circumscribes this rectangle outside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique circumscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If circumscription is tight, this means the rectangle is the bounding
 * box of the four vertices. If it is loose, then the circumscription will
 * only contain two (opposing) vertices, picking the vertices that maximize
 * the minimal axis distance. This creates a smaller rectangle that is still
 * a reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the circumscription
 *
 * @return This circumscribing rectangle
 */
Rect& Rect::circumscribe(const Mat4& transform, bool loose) {
    // Transform the rectangle around it center.
    Vec2 p[4];
    p[0] = origin;
    p[1] = origin+Vec2(size.width,0);
    p[2] = origin+size;
    p[3] = origin+Vec2(0,size.height);

    for(int ii = 0; ii < 4; ii++) {
        p[ii] *= transform;
    }

    // Find the center and rezero
    cugl::Vec2 c = origin+size/2;
    c *= transform;
    for(int ii = 0; ii < 4; ii++) {
        p[ii] -= c;
    }

    // This is one right answer
    if (loose) {
        // Find the best diagonal
        cugl::Vec2 n[2];
        n[0].x = (std::max(p[0].x,p[2].x)-std::min(p[0].x,p[2].x))/2;
        n[0].y = (std::max(p[0].y,p[2].y)-std::min(p[0].y,p[2].y))/2;
        n[1].x = (std::max(p[1].x,p[3].x)-std::min(p[1].x,p[3].x))/2;
        n[1].y = (std::max(p[1].y,p[3].y)-std::min(p[1].y,p[3].y))/2;
        
        float minx = std::min(n[0].x,n[1].x);
        float miny = std::min(n[0].y,n[1].y);
        if (minx < miny) {
            if (n[0].x < n[1].x) {
                n[0] = n[1];
            }
        } else {
            if (n[0].y < n[1].y) {
                n[0] = n[1];
            }
        }
        
        origin = c-n[0];
        size = 2*n[0];
    } else {
        origin.x = std::min(p[0].x,std::min(p[1].x,std::min(p[2].x,p[3].x)));
        origin.y = std::min(p[0].y,std::min(p[1].y,std::min(p[2].y,p[3].y)));
        size.width  = std::max(p[0].x,std::max(p[1].x,std::max(p[2].x,p[3].x)));
        size.height = std::max(p[0].y,std::max(p[1].y,std::max(p[2].y,p[3].y)));
        size -= origin;
        origin += c;
    }
    return *this;
}

/**
 * Circumscribes this rectangle outside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique circumscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If circumscription is tight, this means the rectangle is the bounding
 * box of the four vertices. If it is loose, then the circumscription will
 * only contain two (opposing) vertices, picking the vertices that maximize
 * the minimal axis distance. This creates a smaller rectangle that is still
 * a reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the circumscription
 *
 * @return This circumscribing rectangle
 */
Rect& Rect::circumscribe(const Affine2& transform, bool loose) {
    // Transform the rectangle around it center.
    Vec2 p[4];
    p[0] = origin;
    p[1] = origin+Vec2(size.width,0);
    p[2] = origin+size;
    p[3] = origin+Vec2(0,size.height);

    for(int ii = 0; ii < 4; ii++) {
        p[ii] *= transform;
    }

    // Find the center and rezero
    cugl::Vec2 c = origin+size/2;
    c *= transform;
    for(int ii = 0; ii < 4; ii++) {
        p[ii] -= c;
    }

    // This is one right answer
    if (loose) {
        // Find the best diagonal
        cugl::Vec2 n[2];
        n[0].x = (std::max(p[0].x,p[2].x)-std::min(p[0].x,p[2].x))/2;
        n[0].y = (std::max(p[0].y,p[2].y)-std::min(p[0].y,p[2].y))/2;
        n[1].x = (std::max(p[1].x,p[3].x)-std::min(p[1].x,p[3].x))/2;
        n[1].y = (std::max(p[1].y,p[3].y)-std::min(p[1].y,p[3].y))/2;
        
        float minx = std::min(n[0].x,n[1].x);
        float miny = std::min(n[0].y,n[1].y);
        if (minx < miny) {
            if (n[0].x < n[1].x) {
                n[0] = n[1];
            }
        } else {
            if (n[0].y < n[1].y) {
                n[0] = n[1];
            }
        }
        
        origin = c-n[0];
        size = 2*n[0];
    } else {
        origin.x = std::min(p[0].x,std::min(p[1].x,std::min(p[2].x,p[3].x)));
        origin.y = std::min(p[0].y,std::min(p[1].y,std::min(p[2].y,p[3].y)));
        size.width  = std::max(p[0].x,std::max(p[1].x,std::max(p[2].x,p[3].x)));
        size.height = std::max(p[0].y,std::max(p[1].y,std::max(p[2].y,p[3].y)));
        size -= origin;
        origin += c;
    }
    return *this;
}

/**
 * Returns the rectangle circumscribed inside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique circumscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If circumscription is tight, this means the rectangle is the bounding
 * box of the four vertices. If it is loose, then the circumscription will
 * only contain two (opposing) vertices, picking the vertices that maximize
 * the minimal axis distance. This creates a smaller rectangle that is still
 * a reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the circumscription
 *
 * @return the rectangle circumscribed inside the transformed quadrilateral.
 */
Rect Rect::getCircumscription(const Mat4& transform, bool loose) const {
    Rect rect(*this);
    rect.circumscribe(transform,loose);
    return rect;
}

/**
 * Returns the rectangle circumscribed inside the transformed quadrilateral.
 *
 * A rectangle must be axis-aligned, so transforming a rectangle does not 
 * necessarily produce a rectangle (particularly when the transform includes
 * a rotation). Instead, it produces a quadrilateral polygon. The method
 * sets this rectangle to be the unique circumscribing rectangle that shares 
 * the same center as the quadrilateral.
 *
 * If circumscription is tight, this means the rectangle is the bounding
 * box of the four vertices. If it is loose, then the circumscription will
 * only contain two (opposing) vertices, picking the vertices that maximize
 * the minimal axis distance. This creates a smaller rectangle that is still
 * a reasonable fit for the transformed quadrilateral.
 *
 * @param transform The affine transform
 * @param loose    Whether to loosen the circumscription
 *
 * @return the rectangle circumscribed inside the transformed quadrilateral.
 */
Rect Rect::getCircumscription(const Affine2& transform, bool loose) const {
    Rect rect(*this);
    rect.circumscribe(transform,loose);
    return rect;
}


#pragma mark -
#pragma mark Rectangle Arithmetic
/**
 * Computes the union of this rect and the other, assigning it in place.
 *
 * @param rect  The rect to union with this one.
 *
 * @return This rect, after the union operation.
 */
Rect& Rect::merge(const Rect & rect) {
    float minX = std::min(getMinX(), rect.getMinX());
    float minY = std::min(getMinY(), rect.getMinY());
    float maxX = std::max(getMaxX(), rect.getMaxX());
    float maxY = std::max(getMaxY(), rect.getMaxY());
    set(minX, minY, maxX - minX, maxY - minY);
    return *this;
}

/**
 * Computes the intersection of this rect and the other, assigning it in place.
 *
 * If there is no intersection, this rect becomes the zero rectangle.
 *
 * @param rect  The rect to intersect with this one.
 *
 * @return This rect, after the intersection operation.
 */
Rect& Rect::intersect(const Rect & rect) {
    float minX = std::max(getMinX(), rect.getMinX());
    float minY = std::max(getMinY(), rect.getMinY());
    float maxX = std::min(getMaxX(), rect.getMaxX());
    float maxY = std::min(getMaxY(), rect.getMaxY());
    if (maxX - minX  < 0 || maxY - minY < 0) {
        minX = maxX = minY = maxY = 0;
    }
    set(minX, minY, maxX - minX, maxY - minY);
    return *this;
}

/**
 * Expands this rectangle uniformly from its center.
 *
 * Each edge of the rectangle is adjusted factor away from the center
 * point.  As a result, this method changes both origin and size.  The
 * value factor can be negative, in which case the rect shrinks in size.
 *
 * @param factor  The amount to expand each edge from the center.
 *
 * @return This rect, after the expansion.
 */
Rect& Rect::expand(float factor) {
    origin.set(origin.x-factor,origin.y-factor);
    size.set(size.width+factor,size.height+factor);
    return *this;
}

/**
 * Expands this rectangle to the minimal one containing the given point.
 *
 * If the rectangle already contains the point, it is unchanged.
 *
 * @param point  The point to envelop
 *
 * @return This rect, after the expansion.
 */
Rect& Rect::expand(const Vec2 point) {
    if (point.x < origin.x) {
        size.width += origin.x - point.x;
        origin.x = point.x;
    } else if (point.x > origin.x+size.width) {
        size.width = point.x-origin.x;
    }
    if (point.y < origin.y) {
        size.height += origin.y - point.y;
        origin.y = point.y;
    } else if (point.y > origin.y+size.height) {
        size.height = point.y-origin.y;
    }
    return *this;
}

#pragma mark -
#pragma mark Conversion Methods
/**
 * Returns a string representation of this rectangle for debugging purposes.
 *
 * If verbose is true, the string will include class information.  This
 * allows us to unambiguously identify the class.
 *
 * @param verbose Whether to include class information
 *
 * @return a string representation of this rectangle for debuggging purposes.
 */
std::string Rect::toString(bool verbose) const {
    std::stringstream ss;
    ss << (verbose ? "cugl::Rect[" : "[");
    ss << origin.toString();
    ss << "x";
    ss << size.toString();
    ss << "]";
    return ss.str();
}


#pragma mark -
#pragma mark Constants
/** The zero vector Vec2(0,0) */
const Rect Rect::ZERO(Vec2::ZERO,Size::ZERO);
/** The unit vector Vec2(1,1) */
const Rect Rect::UNIT(Vec2::ZERO,Size(1.0f,1.0f));
