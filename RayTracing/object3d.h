//
//  Object3D.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>

#include "material.h"
#include "geometry.h"

class Object3D;

class BoundingBox {
public:
    BoundingBox(const std::vector<Object3D*>& objects);
    BoundingBox(Geometry::Point3D low, Geometry::Point3D high) : low_(low), high_(high) { }
    
    bool contains(const Geometry::Point3D& p) {
        if (p.x >= low_.x - Geometry::EPS && p.x <= high_.x + Geometry::EPS &&
            p.y >= low_.y - Geometry::EPS && p.y <= high_.y + Geometry::EPS &&
            p.z >= low_.z - Geometry::EPS && p.z <= high_.z + Geometry::EPS) {
            return true;
        }
        return false;
    }
    
    Geometry::Point3D low() {
        return low_;
    }
    
    Geometry::Point3D high() {
        return high_;
    }
    
    long double low(int axis) {
        return low_[axis];
    }
    
    long double high(int axis) {
        return high_[axis];
    }
    
    long double length(int axis) {
        return high_[axis] - low_[axis];
    }
    
    std::pair<BoundingBox, BoundingBox> split(int axis, long double proportion) {
        Geometry::Point3D high1(high_);
        Geometry::Point3D low2(low_);
        
        long double axisValue = length(axis) * proportion + low_[axis];
        high1[axis] = axisValue;
        low2 [axis] = axisValue;
        
        return std::make_pair(BoundingBox(low_, high1), BoundingBox(low2, high_));
    }
    
    void expand(const BoundingBox& bBox) {
        for (int axis = 0; axis < 3; ++axis) {
            low_ [axis] = std::min(low_ [axis], bBox.low_ [axis]);
            high_[axis] = std::max(high_[axis], bBox.high_[axis]);
        }
    }
    
    bool intersect(Geometry::Point3D start,
                   Geometry::Point3D finish,
                   Geometry::Point3D* crossPoint1,
                   Geometry::Point3D* crossPoint2) {
        
        Geometry::Point3D guide = (finish - start).normalize();
        Geometry::Point3D invGuide = 1 / guide;
        
        long double toLow  = invGuide[0] * (low_ [0] - start[0]);
        long double toHigh = invGuide[0] * (high_[0] - start[0]);
        long double tmin = std::min(toLow, toHigh);
        long double tmax = std::max(toLow, toHigh);
        
        toLow  = invGuide[1] * (low_ [1] - start[1]);
        toHigh = invGuide[1] * (high_[1] - start[1]);
        tmin = std::max(tmin, std::min(toLow, toHigh));
        tmax = std::min(tmax, std::max(toLow, toHigh));
        
        toLow  = invGuide[2] * (low_ [2] - start[2]);
        toHigh = invGuide[2] * (high_[2] - start[2]);
        tmin = std::max(tmin, std::min(toLow, toHigh));
        tmax = std::min(tmax, std::max(toLow, toHigh));
        
        if ((tmin <= tmax) && (tmax > 0)) {
            if (tmin > 0) {
                *crossPoint1 = start + guide * tmin;
            } else {
                *crossPoint1 = start;
            }
            *crossPoint2 = start + guide * tmax;
            
            return true;
        }
        return false;
    }
private:
    Geometry::Point3D low_, high_;
};

class Object3D {
public:
    virtual bool intersect(Geometry::Point3D start, Geometry::Point3D finish, Geometry::Point3D* crossPoint) const = 0;
    virtual Geometry::Point3D normalAt(const Geometry::Point3D& point) const = 0;
    virtual BoundingBox boundingBox() const = 0;
    
    Geometry::Vec3 baseIntencity(Geometry::Vec3 global) const {
        return material_.emit() + material_.ambient() * global;
    }
    
    Material material() const {
        return material_;
    }
protected:
    Object3D(Material material) : material_(material) { }
    virtual ~Object3D() { }
private:
    Material material_;
};


BoundingBox::BoundingBox(const std::vector<Object3D*>& objects)  {
    assert(objects.size() != 0);
    assert(objects[0] != NULL);
    
    *this = objects[0]->boundingBox();
    for (int i = 1; i < objects.size(); ++i) {
        assert(objects[i] != NULL);
        
        this->expand(objects[i]->boundingBox());
    }
}

#endif /* OBJECT3D_H */
