//
//  geometry.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef geometry_h
#define geometry_h

#include <cmath>
#include <iostream>
#include <algorithm>

namespace Geometry {
    
    // Constants
    const long double EPS = 1e-10;
    
    // Structs
    struct Point3D {
        long double x, y, z;
        
        Point3D() {}
        Point3D(long double x, long double y, long double z);
        
        Point3D normalize();
        
        long double len2() const;
        long double len()  const;
        
        Point3D& operator +=(const Point3D& p);
        Point3D& operator -=(const Point3D& p);
        Point3D& operator ^=(const Point3D& p);
        
        Point3D& operator *=(long double a);
        Point3D& operator /=(long double a);
    };
    
    struct Sphere3D {
        Point3D center;
        long double r;
        
        Sphere3D() { }
        Sphere3D(Point3D center, long double r);
    };
    
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
    
    // Constructors/Destructors
    Point3D::Point3D(long double x, long double y, long double z) : x(x), y(y), z(z) { }
    
    Sphere3D::Sphere3D(Point3D center, long double r) : center(center), r(r) { }
    
    Polygon3D::~Polygon3D() {
        delete[] this->points;
    }
    
    Polygon3D::Polygon3D(Point3D* points, int cnt) : cnt(cnt) {
        this->points = new Point3D[cnt];
        memcpy(this->points, points, cnt * sizeof(Point3D));
    }
    
    Triangle3D::Triangle3D(Point3D points[3]) : Polygon3D(points, 3) { }
    
    // Operators
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
    
    Point3D operator +(const Point3D& p1, const Point3D& p2) {
        return Point3D(p1) += p2;
    }
    
    Point3D operator -(const Point3D& p1, const Point3D& p2) {
        return Point3D(p1) -= p2;
    }
    
    Point3D operator ^(const Point3D& p1, const Point3D& p2) {
        return Point3D(p1) ^= p2;
    }
    
    Point3D operator *(const Point3D& p, const long double a) {
        return Point3D(p) *= a;
    }
    
    Point3D operator *(const long double a, const Point3D& p) {
        return Point3D(p) *= a;
    }
    
    Point3D operator /(const Point3D& p, const long double a) {
        return Point3D(p) /= a;
    }
    
    Point3D operator /(const long double a, const Point3D& p) {
        return Point3D(p) /= a;
    }
    
    long double operator *(const Point3D& p1, const Point3D& p2) {
        return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
    }
    
    Point3D& Polygon3D::operator[] (int index) {
        if (index < 0) {
            index = cnt + (index % cnt);
        } else if (index > cnt) {
            index %= cnt;
        }
        return points[index];
    }
    
    const Point3D Polygon3D::operator[]  (int index) const {
        if (index < 0) {
            index = cnt + (index % cnt);
        } else if (index > cnt) {
            index %= cnt;
        }
        return points[index];
    }

    // Functions
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
            if (isOnRay(polygon[i], p, p2)) {
                if (isOnRay(polygon[i + 1], p, p2)) {
                    crossCnt++;
                } else {
                    if (doesSegmentIntersectLine(polygon[i - 1], polygon[i + 1], p, p2)) {
                        crossCnt++;
                    }
                }
            } else if (!isOnRay(polygon[i + 1], p, p2) &&
                       doesSegmentIntersectRay(polygon[i], polygon[i + 1], p, p2)) {
                crossCnt++;
            }
        }
        return crossCnt % 2 != 0;
    }
    
    // Methods
    long double Point3D::len2() const {
        return x * x + y * y + z * z;
    }
    
    long double Point3D::len() const {
        return sqrt(len2());
    }
    
    Point3D Point3D::normalize() {
        Point3D n = Point3D(*this);
        return n / n.len();
    }
}

#endif /* geometry_h */
