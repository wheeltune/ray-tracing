//
//  kdTree.h
//  RayTracing
//
//  Created by wheeltune on 04.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef kdTree_h
#define kdTree_h

#include "object3D.h"

const long double C_I = 10;
const long double C_T = 100;

class KDNode {
public:
    KDNode(BoundingBox bBox, const std::vector<Object3D*>& objects) : bBox_(bBox), objects_(objects), left_(NULL), right_(NULL) { }
    KDNode(BoundingBox bBox) : KDNode(bBox, {}) { }
    ~KDNode() {
        objects_.clear();
        delete left_;
        delete right_;
    }
    
    void build() {
        int cnt = 32;
        int *low = new int[cnt], *high = new int[cnt];
        
        long double minSah = C_I * objects_.size();
        long double minAxis = -1;
        long double minProp = 0.0;
        
        for (int axis = 0; axis < 3; ++axis) {
            for (int i = 0; i < cnt; ++i) {
                low[i] = 0;
                high[i] = 0;
            }
            
            for (auto obj : objects_) {
                int ind;
                
                ind = (int)((obj->getBoundingBox().getLow(axis) - bBox_.getLow(axis)) / bBox_.getLength(axis) * cnt);
                ind = std::min(std::max(0, ind), cnt - 1);

                low[ind]++;
                
                ind = (int)((obj->getBoundingBox().getHigh(axis) - bBox_.getLow(axis)) / (bBox_.getLength(axis)) * cnt);
                ind = std::min(std::max(0, ind), cnt - 1);
                
                high[ind]++;
            }
            
            // find suffix for low and prefics for high
            for (int i = 1; i < cnt; ++i) {
                low[cnt - i - 1] += low[cnt - i];
                high[i] += high[i - 1];
            }
            
            // find minimum of SAH
            long double stepLength = bBox_.getLength(axis) / cnt;
            long double sStep = (stepLength * bBox_.getLength((axis + 1) % 3) +
                                 stepLength * bBox_.getLength((axis + 2) % 3));
            long double sBase = bBox_.getLength((axis + 1) % 3) * bBox_.getLength((axis + 2) % 3);
            
            long double sParent = sBase + cnt * sStep;
            long double sLeft  = sBase + sStep;
            long double sRight = sParent - sStep;
            int i = 0;
            
            for (; sLeft < sParent; sLeft += sStep, sRight -= sStep, i++) {
                int cntLeft = (int) objects_.size() - low[i + 1];
                int cntRight = low[i + 1];
                long double sah = C_T + C_I * (sLeft * cntLeft + sRight * cntRight) / sParent;
                if (sah < minSah) {
                    minSah = sah;
                    minAxis = axis;
                    minProp = (i + 1) / cnt;
                }
            }
        }
        
        delete[] low; delete[] high;
        
        if (minAxis >= 0) {
            std::pair<BoundingBox, BoundingBox> bBoxes = bBox_.split(minAxis, minProp);
            std::vector<Object3D*> rightObjects, leftObjects;
            long double splitCoord = bBox_.getLow(minAxis) + bBox_.getLength(minAxis) * minProp;
            
            for (int i = 0; i < objects_.size(); ++i) {
                if ((objects_[i]->getBoundingBox().getLow(minAxis) < splitCoord)) {
                    leftObjects.push_back(objects_[i]);
                }
                    if ((objects_[i]->getBoundingBox().getHigh(minAxis) > splitCoord)) {
                    rightObjects.push_back(objects_[i]);
                }
            }
                        
            left_ = new KDNode(bBoxes.first, leftObjects);
            right_ = new KDNode(bBoxes.second, rightObjects);
            
            left_->build();
            right_->build();
        }
    }
private:
    BoundingBox bBox_;
    KDNode *left_, *right_;
    
    std::vector<Object3D*> objects_;
};

#endif /* kdTree_h */
