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
#include <algorithm>
#include <cmath>
#include <vector>
#include "window.h"
#include "objects.h"
#include "kdTree.h"

using namespace Geometry;

class RayTracer {
public:
    RayTracer(std::istream stream) {
        
    }
    RayTracer(Point3D origin, Window window) : origin_(origin), window_(window) { }
    
    ~RayTracer() {
        objects_.clear();
    }
    
    void addObject(Object3D* object) {
        objects_.push_back(object);
    }
    
    void addLight(Light* light) {
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
        kdTree = new KDNode(objects_);
        kdTree->build();
        window_.begin();
        
        int allias = 1;
        Point3D* rays = new Point3D[allias];

        for (int w = 0; w < window_.getPixelWidth(); ++w) {
            for (int h = 0; h < window_.getPixelHeight(); ++h) {
                Object3D* crossObject;
                Point3D crossPoint;
                Vec3 color = Vec3(0, 0, 0);
                
                window_.getPixelPoints(w, h, rays, allias);
                for (int i = 0; i < allias; ++i) {
                    if (traceRay(origin_, rays[i], &crossObject, &crossPoint)) {
                        
                        Vec3 lightEnergy = crossObject->baseIntencity(Vec3(0.7, 0.7, 0.7));
                        
                        for (auto light : lights_) {
                            Object3D* tmpObject;
                            Point3D tmpPoint;
                            if (traceRay(light->position(), crossPoint, &tmpObject, &tmpPoint)) {
                                if (areEqual(crossPoint, tmpPoint)) {
                                    lightEnergy += light->intencityAt(crossPoint, *crossObject, origin_);
                                }
                            }
                        }
                        
                        color += lightEnergy.limit(0, 1);
                    }
                }
                
                color /= allias;
                
                window_.setPixelColor(w, h, makeRGBA(color));
            }
        }
        
        window_.end();
    }
    
    bool traceRay(const Point3D& start, const Point3D& finish,
                  Object3D** crossObject, Point3D* crossPoint)
    {
        assert(crossObject != NULL);
        
        *crossObject = NULL;
        
        std::vector<std::pair<KDNode*, Point3D>> stack;
        KDNode* currentNode = kdTree;
        Point3D near, far;
        if (kdTree->intersect(start, finish, &near, &far)) {
            
            while (*crossObject == NULL) {
                if (currentNode->isLeaf()) {
                    Point3D tmpPoint;
                    for (int i = 0; i < currentNode->objects_.size(); ++i) {
                        if (currentNode->objects_[i]->intersect(start, finish, &tmpPoint) &&
                            currentNode->contains(tmpPoint) &&
                            (*crossObject == NULL || (tmpPoint - start).len2() < (*crossPoint - start).len2()))
                        {
                            *crossObject = currentNode->objects_[i];
                            *crossPoint = tmpPoint;
                        }
                    }
                    
                    if (*crossObject != NULL || stack.empty()) {
                        break;
                    }
                    
                    currentNode = stack.back().first;
                    near = far;
                    far = stack.back().second;
                    
                    stack.pop_back();
                } else {
                    int axis = currentNode->getSplitAxis();
                    long double coord = currentNode->getSplitCoord();
                    
                    KDNode* farNode  = (far [axis] < coord) ? currentNode->left_ : currentNode->right_;
                    KDNode* nearNode = (near[axis] < coord) ? currentNode->left_ : currentNode->right_;
                    
                    if (nearNode != farNode) {
                        stack.push_back(std::make_pair(farNode, far));
                        far = near + ((far - near) * std::fabs(coord - near[axis])) / std::fabs(far[axis] - near[axis]);
                    }
                    currentNode = nearNode;
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
    KDNode* kdTree;
    
    std::vector<Object3D*> objects_;
    std::vector<Light*> lights_;
};

#endif /* scene_h */
