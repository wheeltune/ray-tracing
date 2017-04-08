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
#include <SDL2/sdl.h>
#include <assert.h>

namespace Geometry {
    
    // Constants
    const long double PI = std::atan2(0, -1);
    const long double EPS = 1e-10;
    
    // Structs
    struct Vec3 {
        long double vec[3];
        
        Vec3() { }
        Vec3(long double v1, long double v2, long double v3);
        
        Vec3 limit(long double down, long double up);
        
        long double& operator[] (int index);
        const long double operator[]  (int index) const;
        
        Vec3& operator +=(const Vec3& v);
        Vec3& operator *=(const Vec3& v);
        Vec3& operator -=(const Vec3& v);
        Vec3& operator /=(const Vec3& v);
        
        Vec3 operator -();
        Vec3 operator +();
        
        Vec3& operator *=(float k);
        Vec3& operator /=(float k);
    };
    
    struct RGBA {
        double r, g, b, a;
        
        RGBA(double r, double g, double b, double a);
        RGBA(SDL_Color c);
    
        SDL_Color toSDL() {
            return SDL_Color{static_cast<Uint8>(r * 255),
                             static_cast<Uint8>(g * 255),
                             static_cast<Uint8>(b * 255),
                             static_cast<Uint8>(a * 255)};
        }
        
        RGBA& operator +=(const RGBA& c);
        RGBA& operator *=(double a);
    };
    
    struct Point3D {
        long double x, y, z;
        
        Point3D() {}
        Point3D(long double x, long double y, long double z);
        
        Point3D normalize() const;
        
        long double len2() const;
        long double len()  const;
        
        long double get(int axis) const;
        void set(int axis, long double k);
        
        Point3D& operator +=(const Point3D& p);
        Point3D& operator -=(const Point3D& p);
        Point3D& operator ^=(const Point3D& p);
        
        Point3D operator -();
        Point3D operator +();
        
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
    Vec3::Vec3(long double v1, long double v2, long double v3) {
        vec[0] = v1;
        vec[1] = v2;
        vec[2] = v3;
    }
    
    RGBA::RGBA(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) { }
    
    RGBA::RGBA(SDL_Color c) : r(1 / 255 * c.r), g(1 / 255 * c.g), b(1 / 255 * c.b), a(1 / 255 * c.a) { }
    
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
    long double& Vec3::operator[] (int index) {
        return vec[index];
    }
    
    const long double Vec3::operator[] (int index) const {
        return vec[index];
    }
    Vec3& Vec3::operator+=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] += v[i];
        return *this;
    }
    
    Vec3& Vec3::operator-=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] -= v[i];
        return *this;
    }
    
    Vec3& Vec3::operator*=(float k) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] *= k;
        return *this;
    }
    
    Vec3& Vec3::operator*=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] *= v[i];
        return *this;
    }
    
    Vec3& Vec3::operator/=(float k) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] /= k;
        return *this;
    }
    
    Vec3& Vec3::operator/=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] /= v[i];
        return *this;
    }
    
    Vec3 operator +(Vec3 p1, const Vec3& p2) {
        return p1 += p2;
    }
    
    Vec3 operator -(Vec3 p1, const Vec3& p2) {
        return p1 -= p2;
    }
    
    Vec3 operator *(Vec3 p1, const Vec3& p2) {
        return p1 *= p2;
    }
    
    Vec3 operator *(Vec3 p, float a) {
        return p *= a;
    }
    
    Vec3 operator *(float a, Vec3 p) {
        return p *= a;
    }
    
    Vec3 operator /(Vec3 p1, const Vec3& p2) {
        return p1 /= p2;
    }
    
    Vec3 operator /(Vec3 p, float a) {
        return p /= a;
    }
    
    Vec3 Vec3::operator -() {
        return Vec3(*this) * (-1);
    }
    
    Vec3 Vec3::operator+() {
        return Vec3(*this);
    }
    
    RGBA& RGBA::operator+=(const RGBA& c) {
        this->r += c.r;
        this->g += c.g;
        this->b += c.b;
        this->a += c.a;
        return *this;
    }
    
    RGBA& RGBA::operator*=(double a) {
        this->r = a * this->r;
        this->g = a * this->g;
        this->b = a * this->b;
        this->a = a * this->a;
        return *this;
    }
    
    RGBA operator +(const RGBA& c1, const RGBA& c2) {
        return RGBA(c1) += c2;
    }
    
    RGBA operator *(const RGBA& c, double a) {
        return RGBA(c) *= a;
    }
    
    RGBA operator *(double a, const RGBA& c) {
        return RGBA(c) *= a;
    }
    
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
    
    Point3D Point3D::operator -() {
        return Point3D(*this) * (-1);
    }
    
    Point3D Point3D::operator+() {
        return Point3D(*this);
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
    
    SDL_Color makeRGBA(Vec3 color) {
        return SDL_Color{static_cast<Uint8>(std::min((long double) 1, color[0]) * 255),
                         static_cast<Uint8>(std::min((long double) 1, color[1]) * 255),
                         static_cast<Uint8>(std::min((long double) 1, color[2]) * 255),
                         255};
    }
    
    // Methods
    Vec3 Vec3::limit(long double down, long double up) {
        Vec3 ans;
        for (int i = 0; i < 3; ++i) {
            ans[i] = std::max(std::min(vec[i], up), down);
        }
        return ans;
    }
    
    long double Point3D::len2() const {
        return x * x + y * y + z * z;
    }
    
    long double Point3D::len() const {
        return sqrt(len2());
    }
    
    long double Point3D::get(int axis) const {
        assert(axis >= 0);
        assert(axis <= 3);
        
        switch (axis) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
        }
        return 0;
    }
    
    void Point3D::set(int axis, long double k) {
        assert(axis >= 0);
        assert(axis <= 3);
        
        switch (axis) {
            case 0:
                x = k;
            case 1:
                y = k;
            case 2:
                z = k;
        }
    }
    
    Point3D Point3D::normalize() const {
        Point3D n = Point3D(*this);
        return n / n.len();
    }
    
    Point3D reflect(Point3D v1, Point3D v2) {
        v1 = v1.normalize();
        v2 = v2.normalize();
        return 2 * v1 * (v1 * v2) - v2;
    }
}

#endif /* geometry_h */
