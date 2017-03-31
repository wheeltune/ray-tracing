//
//  scene.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include <iostream>
#include <vector>
#include "window.h"
#include "objects.h"
#include "light.h"

using namespace Geometry;

class RayTracer {
public:
    ~RayTracer() {
        objects_.clear();
    }
    
    RayTracer(Point3D origin, Window window) : origin_(origin), window_(window) { }
    
    void addObject(Object3D* object) {
        objects_.push_back(object);
    }
    
    void addLight(DotLight* light) {
        lights_.push_back(light);
    }
    
    bool start() {
        return window_.open();
    }
    
    void stop() {
        window_.close();
    }
    
    void setPixelColor(int x, int y, SDL_Color color) {
        window_.setPixelColor(x, y, color);
    }
    
    void draw() {
        window_.begin();

        for (int w = 0; w < window_.getPixelWidth(); ++w) {
            for (int h = 0; h < window_.getPixelHeight(); ++h) {
                Object3D* crossObject;
                Point3D crossPoint;
                
                if (traceRay(origin_, window_.getPixelPoint(w, h), &crossObject, &crossPoint)) {
                    long double lightEnergy = 2;
                    
                    for (auto light : lights_) {
                        Object3D* tmpObject;
                        Point3D tmpPoint;
                        if (traceRay(light->getPosition(), crossPoint, &tmpObject, &tmpPoint)) {
                            if (areEqual(crossPoint, tmpPoint)) {
//                                visible = true;
                                Point3D guide = light->getPosition() - crossPoint;
                                Point3D norm = crossObject->getNormal(crossPoint);
                                long double cosAlpha = std::abs(cos(guide, norm));
                                lightEnergy += light->getIntensity() / guide.len2() * cosAlpha;
                            }
                        }
                    }
                    
                    lightEnergy = 1 - 1 / lightEnergy;
                    SDL_Color objColor = crossObject->getColor(crossPoint);
                    window_.setPixelColor(w, h, SDL_Color{static_cast<Uint8>(objColor.r * lightEnergy),
                                                          static_cast<Uint8>(objColor.g * lightEnergy),
                                                          static_cast<Uint8>(objColor.b * lightEnergy),
                                                          objColor.a});
                } else {
                    window_.setPixelColor(w, h, SDL_Color{255, 255, 255, 255});
                }
            }
        }
        
        window_.end();
    }
    
    bool traceRay(const Point3D& start, const Point3D& finish,
                  Object3D** crossObject, Point3D* crossPoint)
    {
        *crossObject = NULL;
        Point3D tmpPoint;
        
        for (int i = 0; i < objects_.size(); ++i) {
            if (objects_[i]->intersect(start, finish, &tmpPoint) &&
                (*crossObject == NULL || (tmpPoint - start).len2() < (*crossPoint - start).len2()))
            {
                if (crossObject != NULL) {
                    *crossObject = objects_[i];
                    *crossPoint = tmpPoint;
                }
            }
        }
        
        return *crossObject != NULL;
    }
    
    void flush() {
        window_.flush();
    }
    
    bool shouldClose() {
        SDL_Event event;
        if (window_.getEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return true;
            }
        }
        return false;
    }
private:
    Point3D origin_;
    Window window_;
    
    std::vector<Object3D*> objects_;
    std::vector<DotLight*> lights_;
};

#endif /* scene_h */
