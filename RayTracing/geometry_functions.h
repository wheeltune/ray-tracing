//
//  geometry_functions.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef geometry_functions_h
#define geometry_functions_h

#include <SDL2/sdl.h>

#include "geometry_constants.h"
#include "vec3.h"
#include "point3d.h"
#include "sphere3d.h"
#include "polygon3d.h"

namespace Geometry {
    bool isZero(long double a) {
        return std::abs(a) < EPS;
    }
    
    int sign(long double a) {
        if (isZero(a)) return 0;
        return a > 0 ? 1 : -1;
    }
    
    long double cos(const Point3D& p1, const Point3D& p2) {
        return (p1 * p2) / p1.len() / p2.len();
    }
    
    bool areEqual(long double x, long double y) {
        return sign(x - y) == 0;
    }
    
    bool areEqual(const Point3D& p1, const Point3D& p2) {
        return areEqual(p1.x, p2.x) && areEqual(p1.y, p2.y) && areEqual(p1.z, p2.z);
    }
    
    bool areCollinear(const Point3D& p1, const Point3D& p2) {
        return isZero((p1 ^ p2).len2());
    }
    
    bool areComplanar(const Point3D& p1, const Point3D& p2, const Point3D& p3) {
        return isZero(p1 * (p2 ^ p3));
    }
    
    bool isOnLine(const Point3D& p, const Point3D& A, const Point3D& B) {
        return areCollinear(p - A, B - A);
    }
    
    bool isOnRay(const Point3D& p, const Point3D& A, const Point3D& B) {
        return isOnLine(p, A, B) && sign((A - p) * (B - A)) <= 0;
    }
    
    bool isOnSegment(const Point3D& p, const Point3D& A, const Point3D& B) {
        return isOnLine(p, A, B) && sign((A - p) * (B - p)) <= 0;
    }
    
    bool areLinesParallelOrEqual(const Point3D& A1, const Point3D& A2, const Point3D& B1, const Point3D& B2) {
        return areCollinear(A2 - A1, B2 - B1);
    }
    
    bool areLinesOnOnePlane(const Point3D& A1, const Point3D& A2, const Point3D& B1, const Point3D& B2) {
        return areComplanar(A1 - B1, A2 - A1, B2 - B1);
    }
    
    bool findLinesIntersection(const Point3D& A1, const Point3D& A2, const Point3D& B1, const Point3D& B2, Point3D* crossPoint) {
        if (areLinesParallelOrEqual(A1, A2, B1, B2)) {
            return false;
        }
        
        Point3D n1 = (B1 - A1) ^ (B2 - B1);
        Point3D n2 = (A2 - A1) ^ (B2 - B1);
        long double t = sign(n1 * n2) * sqrt(n1.len2() / n2.len2());
        
        *crossPoint = A1 + t * (A2 - A1);
        return true;
    }
    
    bool doesSegmentIntersectLine(const Point3D& s1, const Point3D& s2, const Point3D& l1, const Point3D& l2) {
        if (areLinesOnOnePlane(s1, s2, l1, l2)) {
            Point3D n1 = (s1 - l1) ^ (l2 - l1);
            Point3D n2 = (s2 - l1) ^ (l2 - l1);
            
            return sign(n1 * n2) <= 0;
        }
        return false;
    }
    
    bool doesSegmentIntersectRay(const Point3D& s1, const Point3D& s2, const Point3D& r1, const Point3D& r2) {
        if (areLinesOnOnePlane(s1, s2, r1, r2)) {
            Point3D n1 = (s1 - r1) ^ (r2 - r1);
            Point3D n2 = (s2 - r1) ^ (r2 - r1);
            
            Point3D n3 = (s2 - s1) ^ (r2 - r1);
            Point3D n4 = (s1 - r1) ^ (s2 - r1);
            
            return sign(n1 * n2) <= 0 && sign(n3 * n4) <= 0;
        }
        return false;
    }
    
    bool isPointInPolygon(const Point3D& p, const Polygon3D& polygon) {
        unsigned int crossCnt = 0;
        
        Point3D p2 = polygon.points[0];    // second point to ray
        
        if (areEqual(p, p2)) {
            return true;  // p is polygon[0]
        }
        
        // process all points
        for (int i = 0; i < polygon.cnt; ++i) {
            int next_i = i + 1;
            if (next_i >= polygon.cnt) {
                next_i -= polygon.cnt;
            }
            
            int prev_i = i - 1;
            if (prev_i < 0) {
                prev_i += polygon.cnt;
            }
            
            if (isOnRay(polygon[i], p, p2)) {
                if (isOnRay(polygon[next_i], p, p2)) {
                    crossCnt++;
                } else {
                    if (doesSegmentIntersectLine(polygon[prev_i], polygon[next_i], p, p2)) {
                        crossCnt++;
                    }
                }
            } else if (!isOnRay(polygon[next_i], p, p2) &&
                       doesSegmentIntersectRay(polygon[i], polygon[next_i], p, p2)) {
                crossCnt++;
            }
        }
        return crossCnt % 2 != 0;
    }
    
    SDL_Color makeRGBA(Vec3 color) {
        return SDL_Color{static_cast<Uint8>(std::min((long double) 1, color[0]) * 255),
            static_cast<Uint8>(std::min((long double) 1, color[1]) * 255),
            static_cast<Uint8>(std::min((long double) 1, color[2]) * 255),
            255};
    }
    
    Point3D reflect(Point3D v1, Point3D v2) {
        v1.normalize();
        v2.normalize();
        return 2 * v1 * (v1 * v2) - v2;
    }
}

#endif /* geometry_functions_h */
