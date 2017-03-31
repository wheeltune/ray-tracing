//
//  light.h
//  RayTracing
//
//  Created by wheeltune on 31.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef light_h
#define light_h

using namespace Geometry;

class DotLight {
public:
    DotLight(Point3D position, long double intensity) : position_(position), intensity_(intensity) { }
    
    Point3D getPosition() {
        return position_;
    }
    
    long double getIntensity() {
        return intensity_;
    }
private:
    Point3D position_;
    long double intensity_;
};

#endif /* light_h */
