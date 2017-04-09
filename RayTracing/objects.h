//
//  objects.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef objects_h
#define objects_h

#include "object3D.h"
#include "geometry.h"
#include <assert.h>

using namespace Geometry;

class Sphere : public Object3D {
public:
    Sphere(Point3D center, int r, Material material) : Object3D(material), center_(center), r_(r) { }
    
    virtual bool intersect(Point3D start, Point3D finish, Point3D* crossPoint) {
        Point3D guide = (finish - start).normalize();
        
        long double d2 = ((start - center_) ^ guide).len2() / guide.len2();
        if (d2 > r_ * r_) {
            return false;
        }
        
        if (isOnLine(center_, start, finish)) {
            *crossPoint = center_ - r_ * guide;
        } else {
            Point3D t = (center_ - start) ^ guide;
            Point3D n = t ^ guide;
            n = n.normalize() * sqrt(d2);
            
            *crossPoint = center_ + n - guide * sqrt(r_ * r_ - d2);
        }
        
        return true;
    }
    
    virtual Point3D getNormal(const Point3D& point) {
        return (point - center_).normalize();
    }
    
    virtual BoundingBox getBoundingBox() {
        return BoundingBox(center_ - Point3D(r_, r_, r_), center_ + Point3D(r_, r_, r_));
    }
private:
    Point3D center_;
    int r_;
};

class Polygon : public Object3D {
public:
    Polygon(Point3D* points, int cnt, Material material, Point3D orientation) : Object3D(material),
                                                                                polygon_(points, cnt),
                                                                                orientation_(orientation) { }
    
    Polygon(Point3D* points, int cnt, Material material) : Polygon(points, cnt, material, Point3D(0, 0, 0)) { }
    
    virtual bool intersect(Point3D start, Point3D finish, Point3D* crossPoint) {
        assert(!areEqual(start, finish));
        
        Point3D guide = finish - start;
        
        Point3D norm = getNormal(); // Normal to the plane
        long double d = norm * (polygon_[0] - start);
        long double e = norm * guide;
        
        if (!isZero(e)) {
            if (sign(d) != sign(e)) {
                // sign(d) == 0 mean that line on plane
                // cross point on line but not on ray
                return false;
            }
            
            // Let's find cross point then
            *crossPoint = start + guide * (d / e);
            
            return isPointInPolygon(*crossPoint, polygon_);
        }
        return false;
    }
    
    void setOrientation(const Point3D& orientation) {
        orientation_ = orientation;
    }
    
    virtual Point3D getNormal(const Point3D& point) {
        return getNormal();
    }
    
    virtual BoundingBox getBoundingBox() {
        Point3D low = polygon_[0], high = polygon_[0];
        
        for (int i = 1; i < polygon_.cnt; ++i) {
            low.x = std::min(low.x, polygon_[i].x);
            low.y = std::min(low.y, polygon_[i].y);
            low.z = std::min(low.z, polygon_[i].z);
            
            high.x = std::max(high.x, polygon_[i].x);
            high.y = std::max(high.y, polygon_[i].y);
            high.z = std::max(high.z, polygon_[i].z);
        }
        return BoundingBox(low, high);
    }
    
protected:
    Polygon3D polygon_;
    SDL_Color color_;
    Point3D orientation_;
    
    virtual Point3D getNormal() {
        Point3D normal = ((polygon_[1] - polygon_[0]) ^ (polygon_[2] - polygon_[0])).normalize();
        if (sign(normal * (orientation_ - polygon_[0])) < 0) {
            normal *= -1;
        }
        return normal;
    }
};

class Triangle : public Polygon {
public:
    Triangle(Point3D points[3], Material material) : Polygon(points, 3, material) { }
    
    virtual bool intersect(Point3D start, Point3D finish, Point3D* crossPoint) {
        assert(!areEqual(start, finish));
        
        Point3D guide = finish - start;
        
        Point3D norm = getNormal(); // Normal to the plane
        long double d = norm * (polygon_[0] - start);
        long double e = norm * guide;
        
        if (!isZero(e)) {
            if (sign(d) != sign(e)) {
                // sign(d) == 0 means that line on plane
                // cross point on line but not on ray
                return false;
            }
            
            // Let's find cross point then
            *crossPoint = start + guide * (d / e);
            
            // Find normals
            Point3D norm[3] = {
                (polygon_[0] - *crossPoint) ^ (polygon_[1] - *crossPoint),
                (polygon_[1] - *crossPoint) ^ (polygon_[2] - *crossPoint),
                (polygon_[2] - *crossPoint) ^ (polygon_[0] - *crossPoint)
            };
            
            // If they have same orientation
            int sum = sign(norm[0] * norm[1]) +
                      sign(norm[1] * norm[2]) +
                      sign(norm[2] * norm[0]);
            
            if (sum == 3 || sum == -3) {
                return true;
            }
        }
        return false;
    }
};

class Quadrangle : public Polygon {
public:
    Quadrangle(Point3D points[4], Material material) : Polygon(points, 4, material) { }
};

#endif /* objects_h */
