//
//  objects.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef objects_h
#define objects_h

#include "Object3D.h"
#include "geometry.h"

using namespace Geometry;

class Sphere : public Object3D {
public:
    Sphere(Point3D center, int r, SDL_Color color) : center_(center), r_(r), color_(color) { }
    
    bool isCross(Point3D start, Point3D finish, Point3D* crossPoint, SDL_Color* crossColor) {
        Point3D guide = finish - start;
        *crossColor = color_;
        
        long double d2 = ((start - center_) ^ guide).len2() / guide.len2();
        if (d2 > r_ * r_) {
            return false;
        }
        
        if (isOnLine(center_, start, finish)) {
            *crossPoint = center_ - r_ * guide.normalize();
        } else {
            Point3D t = (center_ - start) ^ guide;
            Point3D n = t ^ guide;
            n = n.normalize() * sqrt(d2);
            
            *crossPoint = center_ + n - guide.normalize() * sqrt(r_ * r_ - d2);
        }
        
        return true;
    }
private:
    Point3D center_;
    int r_;
    SDL_Color color_;
};

class Polygon : public Object3D {
public:
    Polygon(Point3D* points, int cnt, SDL_Color color) : cnt_(cnt), color_(color) {
        points_ = new Point3D[cnt];
        for (int i = 0; i < cnt; ++i) {
            points_[i] = points[i];
        }
    }
    
    ~Polygon() {
        delete[] points_;
    }
    
    bool isCross(Point3D start, Point3D finish, Point3D* crossPoint, SDL_Color* crossColor) {
        Point3D guide = finish - start;
        *crossColor = color_;
        
        Point3D norm = (points_[1] - points_[0]) ^ (points_[2] - points_[0]); // Normal to the plane
        long double d = norm * (points_[0] - start);
        long double e = norm * guide;
        
        if (!isZero(e)) {
            if (isZero(d)) {
                // line on plane
                return true;
            }
            
            // Let's find cross point then
            *crossPoint = start + guide * (d / e);
            
            return isPointInPolygon(*crossPoint, Polygon3D(points_, cnt_));
        }
        return false;
    }
protected:
    Point3D* points_;
    int cnt_;
    SDL_Color color_;
};

class Triangle : public Polygon {
public:
    Triangle(Point3D points[3], SDL_Color color) : Polygon(points, 3, color) { }
    
    bool isCross(Point3D start, Point3D finish, Point3D* crossPoint, SDL_Color* crossColor) {
        Point3D guide = finish - start;
        *crossColor = color_;
        
        Point3D norm = (points_[1] - points_[0]) ^ (points_[2] - points_[0]); // Normal to the plane
        long double d = norm * (points_[0] - start);
        long double e = norm * guide;
        
        if (!isZero(e)) {
            if (isZero(d)) {
                // line on plane
                return true;
            }
            
            // Let's find cross point then
            *crossPoint = start + guide * (d / e);
            
            // Find normals
            Point3D norm[3] = {
                (points_[0] - *crossPoint) ^ (points_[1] - *crossPoint),
                (points_[1] - *crossPoint) ^ (points_[2] - *crossPoint),
                (points_[2] - *crossPoint) ^ (points_[0] - *crossPoint)
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
    Quadrangle(Point3D points[4], SDL_Color color) : Polygon(points, 4, color) { }
};

#endif /* objects_h */
