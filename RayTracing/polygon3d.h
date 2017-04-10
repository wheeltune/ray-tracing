//
//  polygon3d.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef polygon3d_h
#define polygon3d_h

#include "point3d.h"

namespace Geometry {
    struct Polygon3D {
        Point3D* points;
        int cnt;
        
        ~Polygon3D();
        Polygon3D() { }
        Polygon3D(Point3D* points, int cnt);
        
        Point3D& operator[] (int index);
        const Point3D operator[]  (int index) const;
    };
    
    struct Triangle3D : Polygon3D {
        Triangle3D() { }
        Triangle3D(Point3D points[3]);
    };

    Polygon3D::~Polygon3D() {
        delete[] this->points;
    }
    
    Polygon3D::Polygon3D(Point3D* points, int cnt) : cnt(cnt) {
        this->points = new Point3D[cnt];
        memcpy(this->points, points, cnt * sizeof(Point3D));
    }
    
    Triangle3D::Triangle3D(Point3D points[3]) : Polygon3D(points, 3) { }
    
    Point3D& Polygon3D::operator[] (int index) {
        assert(index >= 0);
        assert(index < cnt);

        return points[index];
    }
    
    const Point3D Polygon3D::operator[]  (int index) const {
        assert(index >= 0);
        assert(index < cnt);
        
        return points[index];
    }
}


#endif /* polygon3d_h */
