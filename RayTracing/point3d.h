//
//  point3d.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef point3d_h
#define point3d_h

#include <cassert>
#include "geometry_constants.h"

namespace Geometry {
    
    struct Point3D {
        long double x, y, z;
        
        Point3D() {}
        Point3D(long double x, long double y, long double z);
        
        Point3D getNormalized() const;
        Point3D& normalize();
        
        long double len2() const;
        long double len()  const;
        
        long double& operator[] (int axis);
        const long double operator[] (int axis) const;
        
        Point3D& operator +=(const Point3D& p);
        Point3D& operator -=(const Point3D& p);
        Point3D& operator ^=(const Point3D& p);
        
        Point3D operator -();
        Point3D operator +();
        
        Point3D& operator *=(long double a);
        Point3D& operator /=(long double a);
    };
    

    Point3D::Point3D(long double x, long double y, long double z) : x(x), y(y), z(z) { }
    
    
    Point3D& Point3D::operator +=(const Point3D& p) {
        this->x += p.x;
        this->y += p.y;
        this->z += p.z;
        return *this;
    }
    
    Point3D& Point3D::operator -=(const Point3D& p) {
        this->x -= p.x;
        this->y -= p.y;
        this->z -= p.z;
        return *this;
    }
    
    Point3D& Point3D::operator ^=(const Point3D& p) {
        Point3D old(*this);
        
        this->x = old.y * p.z - old.z * p.y;
        this->y = old.z * p.x - old.x * p.z;
        this->z = old.x * p.y - old.y * p.x;
        
        return *this;
    }
    
    Point3D& Point3D::operator *=(long double a) {
        this->x *= a;
        this->y *= a;
        this->z *= a;
        return *this;
    }
    
    Point3D& Point3D::operator /=(long double a) {
        this->x /= a;
        this->y /= a;
        this->z /= a;
        return *this;
    }
    
    Point3D operator +(Point3D p1, const Point3D& p2) {
        return p1 += p2;
    }
    
    Point3D operator -(Point3D p1, const Point3D& p2) {
        return p1 -= p2;
    }
    
    Point3D operator ^(Point3D p1, const Point3D& p2) {
        return p1 ^= p2;
    }
    
    Point3D operator *(Point3D p, long double a) {
        return p *= a;
    }
    
    Point3D operator *(long double a, Point3D p) {
        return p *= a;
    }
    
    Point3D operator /(Point3D p, long double a) {
        return p /= a;
    }
    
    Point3D operator /(long double a, const Point3D& p) {
        return Point3D(a / p.x, a / p.y, a / p.z);
    }
    
    Point3D Point3D::operator -() {
        return Point3D(*this) * (-1);
    }
    
    Point3D Point3D::operator+() {
        return Point3D(*this);
    }
    
    long double operator *(const Point3D& p1, const Point3D& p2) {
        return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
    }
    
    long double Point3D::len2() const {
        return x * x + y * y + z * z;
    }
    
    long double Point3D::len() const {
        return sqrt(len2());
    }
    
    const long double Point3D::operator[](int axis) const {
        assert(axis >= 0);
        assert(axis <= 3);
        
        if (axis == 0) {
            return x;
        } else if (axis == 1) {
            return y;
        } else {
            return z;
        }
    }
    
    long double& Point3D::operator[](int axis) {
        assert(axis >= 0);
        assert(axis <= 3);
        
        if (axis == 0) {
            return x;
        } else if (axis == 1) {
            return y;
        } else {
            return z;
        }
    }
    
    Point3D Point3D::getNormalized() const {
        Point3D n = Point3D(*this);
        return n / n.len();
    }
    
    Point3D& Point3D::normalize() {
        return (*this) /= this->len();
    }
}

#endif /* point3d_h */
