//
//  sphere.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef OBJECTS_SAMPLES_H
#define OBJECTS_SAMPLES_H

class Sphere : public Object3D {
public:
    Sphere(Geometry::Point3D center, int r, Material material) : Object3D(material), center_(center), r_(r) { }
    
    virtual bool intersect(Geometry::Point3D start, Geometry::Point3D finish, Geometry::Point3D* crossPoint) const {
        Geometry::Point3D guide = (finish - start).normalize();
        
        long double d2 = ((start - center_) ^ guide).len2() / guide.len2();
        if (d2 > r_ * r_) {
            return false;
        }
        
        if (isOnLine(center_, start, finish)) {
            *crossPoint = center_ - r_ * guide;
        } else {
            Geometry::Point3D t = (center_ - start) ^ guide;
            Geometry::Point3D n = t ^ guide;
            n = n.normalize() * sqrt(d2);
            
            *crossPoint = center_ + n - guide * sqrt(r_ * r_ - d2);
        }
        
        return true;
    }
    
    virtual Geometry::Point3D normalAt(const Geometry::Point3D& point) const {
        return (point - center_).normalize();
    }
    
    virtual BoundingBox boundingBox() const {
        return BoundingBox(center_ - Geometry::Point3D(r_, r_, r_), center_ + Geometry::Point3D(r_, r_, r_));
    }
private:
    Geometry::Point3D center_;
    int r_;
};

class Polygon : public Object3D {
public:
    Polygon(Geometry::Point3D* points, int cnt, Material material, Geometry::Point3D orientation) : Object3D(material),
    polygon_(points, cnt),
    orientation_(orientation) { }
    
    Polygon(Geometry::Point3D* points, int cnt, Material material) : Polygon(points, cnt, material, Geometry::Point3D(0, 0, 0)) { }
    
    virtual bool intersect(Geometry::Point3D start, Geometry::Point3D finish, Geometry::Point3D* crossPoint) const {
        assert(!areEqual(start, finish));
        
        Geometry::Point3D guide = finish - start;
        
        Geometry::Point3D norm = normal(); // Normal to the plane
        long double d = norm * (polygon_[0] - start);
        long double e = norm * guide;
        
        if (!Geometry::isZero(e)) {
            if (Geometry::sign(d) != Geometry::sign(e)) {
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
    
    void setOrientation(const Geometry::Point3D& orientation) {
        orientation_ = orientation;
    }
    
    virtual Geometry::Point3D normalAt(const Geometry::Point3D& point) const {
        return normal();
    }
    
    virtual BoundingBox boundingBox() const {
        Geometry::Point3D low = polygon_[0], high = polygon_[0];
        
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
    Geometry::Polygon3D polygon_;
    SDL_Color color_;
    Geometry::Point3D orientation_;
    
    virtual Geometry::Point3D normal() const {
        Geometry::Point3D normal = ((polygon_[1] - polygon_[0]) ^ (polygon_[2] - polygon_[0])).normalize();
        if (Geometry::sign(normal * (orientation_ - polygon_[0])) < 0) {
            normal *= -1;
        }
        return normal;
    }
};

class Triangle : public Polygon {
public:
    Triangle(Geometry::Point3D points[3], Material material) : Polygon(points, 3, material) { }
    
    virtual bool intersect(Geometry::Point3D start, Geometry::Point3D finish, Geometry::Point3D* crossPoint) const {
        assert(!areEqual(start, finish));
        
        Geometry::Point3D guide = finish - start;
        
        Geometry::Point3D norm = normal(); // Normal to the plane
        long double d = norm * (polygon_[0] - start);
        long double e = norm * guide;
        
        if (!Geometry::isZero(e)) {
            if (Geometry::sign(d) != Geometry::sign(e)) {
                // sign(d) == 0 means that line on plane
                // cross point on line but not on ray
                return false;
            }
            
            // Let's find cross point then
            *crossPoint = start + guide * (d / e);
            
            // Find normals
            Geometry::Point3D norm[3] = {
                (polygon_[0] - *crossPoint) ^ (polygon_[1] - *crossPoint),
                (polygon_[1] - *crossPoint) ^ (polygon_[2] - *crossPoint),
                (polygon_[2] - *crossPoint) ^ (polygon_[0] - *crossPoint)
            };
            
            // If they have same orientation
            int sum = Geometry::sign(norm[0] * norm[1]) +
            Geometry::sign(norm[1] * norm[2]) +
            Geometry::sign(norm[2] * norm[0]);
            
            if (sum == 3 || sum == -3) {
                return true;
            }
        }
        return false;
    }
};

class Quadrangle : public Polygon {
public:
    Quadrangle(Geometry::Point3D points[4], Material material) : Polygon(points, 4, material) { }
};


#endif /* OBJECTS_SAMPLES_H */
