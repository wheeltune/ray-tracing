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

using namespace Geometry;

class RayTracer {
public:
    RayTracer(Point3D origin, Window window) : origin_(origin), window_(window) { }
    
    void addObject(Object3D* object) {
        objects_.push_back(object);
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
                SDL_Color color = SDL_Color{255, 255, 255, 255}, tmpColor;
                bool isFound = false;
                Point3D near = origin_, tmp;
                
                for (int i = 0; i < objects_.size(); ++i) {
                    if (objects_[i]->isCross(
                                             origin_,
                                             window_.getPixelPoint(w, h),
                                             &tmp,
                                             &tmpColor
                                             )
                        ) {
                        if (isFound == false || (tmp - origin_).len2() < (near - origin_).len2()) {
                            isFound = true;
                            near = tmp;
                            color = tmpColor;
                        }
                    }
                    
                }
                
                if (isFound) {
                    window_.setPixelColor(w, h, color);
                }
            }
        }
        
        window_.end();
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
};

#endif /* scene_h */
