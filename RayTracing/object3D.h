//
//  Object3D.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef Object3D_h
#define Object3D_h

#include <assert.h>
#include <SDL2/sdl.h>
#include <cmath>
#include <algorithm>

using namespace Geometry;

class BoundingBox {
public:
    BoundingBox(Point3D low, Point3D high) : low_(low), high_(high) { }
        
    long double getLow(int axis) {
        return low_.get(axis);
    }
    
    long double getHigh(int axis) {
        return high_.get(axis);
    }
    
    long double getLength(int axis) {
        return high_.get(axis) - low_.get(axis);
    }
    
    std::pair<BoundingBox, BoundingBox> split(int axis, long double proportion) {
        Point3D high1(high_);
        Point3D low2(low_);
        
        long double axisValue = getLength(axis) * proportion + low_.get(axis);
        high1.set(axis, axisValue);
        low2.set(axis, axisValue);
        
        return std::make_pair(BoundingBox(low_, high1), BoundingBox(low2, high_));
    }
    
    void expand(const BoundingBox& bBox) {
        low_.x = std::min(low_.x, bBox.low_.x);
        low_.y = std::min(low_.y, bBox.low_.y);
        low_.z = std::min(low_.z, bBox.low_.z);
        
        high_.x = std::max(high_.x, bBox.high_.x);
        high_.y = std::max(high_.y, bBox.high_.y);
        high_.z = std::max(high_.z, bBox.high_.z);
    }
private:
    Point3D low_, high_;
};

struct Material {
    Vec3 kAmbient;
    Vec3 kDiffuse;
    Vec3 kSpecular;
    Vec3 kEmit;
    Vec3 kTransparency;
    long double kShine;
    
    Material(Vec3 kAmbient,
             Vec3 kDiffuse,
             Vec3 kSpecular,
             long double kShine = 1,
             Vec3 kEmit = Vec3(0, 0, 0),
             Vec3 kTransparency = Vec3(0, 0, 0))
    : kAmbient(kAmbient), kDiffuse(kDiffuse), kSpecular(kSpecular), kShine(kShine),
      kEmit(kEmit), kTransparency(kTransparency) { }
};

class LightParams {
public:
    LightParams(long double iAmbient, long double iDiffuse, long double iSpecular, Vec3 kDistance = Vec3(0, 0, 1))
    : iAmbient(Vec3(iAmbient, iAmbient, iAmbient)), iDiffuse(Vec3(iDiffuse, iDiffuse, iDiffuse)),
      iSpecular(Vec3(iSpecular, iSpecular, iSpecular)), kDistance(kDistance) { }
    
    LightParams(Vec3 iAmbient, Vec3 iDiffuse, Vec3 iSpecular, Vec3 kDistance = Vec3(0, 0, 1))
    : iAmbient(iAmbient), iDiffuse(iDiffuse), iSpecular(iSpecular), kDistance(kDistance) { }
    
    Vec3 getIntensity(long double d2, Vec3 kAmbient, Vec3 kDiffuse, Vec3 kSpecular) const {
        return (kAmbient * iAmbient + kDiffuse * iDiffuse + kSpecular * iSpecular)
                / (kDistance[0] + kDistance[1] * sqrt(d2) + kDistance[2] * d2);
    }
protected:
    Vec3 iAmbient;
    Vec3 iDiffuse;
    Vec3 iSpecular;
    Vec3 kDistance;
};

class DotLight : public LightParams {
public:
    DotLight(Point3D position, LightParams params) : LightParams(params), position_(position) { }
    
    Point3D getPosition() {
        return position_;
    }
private:
    Point3D position_;
};

class Object3D {
public:
    virtual bool intersect(Point3D start, Point3D finish, Point3D* crossPoint) {
        return false;
    }
    
    virtual Vec3 getBaseIntencity(Vec3 iGlobal) {
        return material_.kEmit + material_.kAmbient * iGlobal;
    }
    
    virtual Vec3 getLightIntencity(const Point3D& point, const Point3D& origin, const Point3D& light, const LightParams& lightParams) {
        Point3D n = getNormal(point).normalize();
        Point3D v = (origin - point).normalize();
        Point3D l = (light - point).normalize();
        Point3D r = 2 * (n * l) * n - l;
        
        Vec3 intensity = lightParams.getIntensity((light - point).len2(),
                                                  material_.kAmbient,
                                                  material_.kDiffuse * (l * n),
                                                  material_.kSpecular * std::pow(std::max((long double) 0, r * v), material_.kShine));
        
        return intensity;
    }
    
    virtual Point3D getNormal(const Point3D& point) {
        return Point3D(0, 0, 0);
    }
    
    virtual BoundingBox getBoundingBox() {
        return BoundingBox(Point3D(0, 0, 0), Point3D(0, 0, 0));
    }
protected:
    Object3D(Material material) : material_(material) { }
    virtual ~Object3D() { }
private:
    Material material_;
};

#endif /* Object3D_h */
