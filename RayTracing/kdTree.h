//
//  kdTree.h
//  RayTracing
//
//  Created by wheeltune on 04.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef kdTree_h
#define kdTree_h

#include "objects.h"

const long double C_I = 1;
const long double C_T = 4;

class KDNode {
public:
    KDNode(BoundingBox bBox, const std::vector<Object3D*>& objects) : bBox_(bBox), objects_(objects), left_(NULL), right_(NULL), splitAxis_(-1) { }
    KDNode(const std::vector<Object3D*>& objects) : KDNode(BoundingBox(objects), objects) { }
    ~KDNode() {
        objects_.clear();
        delete left_;
        delete right_;
    }
    
    void build() {
        int cnt = 32;
        
        if (objects_.size() < 2) return;

        long double minSah = C_I * objects_.size();

        int minAxis = -1;
        long double minProp = 0.0;
        
        for (int axis = 0; axis < 3; ++axis) {
            std::vector<int> low(cnt, 0), high(cnt, 0);
            
            for (auto obj : objects_) {
                int ind;
                
                ind = (int)((obj->boundingBox().low(axis) - bBox_.low(axis)) / bBox_.length(axis) * cnt);
                ind = std::min(std::max(0, ind), cnt - 1);

                low[ind]++;
                
                ind = (int)((obj->boundingBox().high(axis) - bBox_.low(axis)) / (bBox_.length(axis)) * cnt);
                ind = std::min(std::max(0, ind), cnt - 1);
                
                high[ind]++;
            }
            
            // find suffix for low and prefics for high
            for (int i = 1; i < cnt; ++i) {
                low[cnt - i - 1] += low[cnt - i];
                high[i] += high[i - 1];
            }
            
            // find minimum of SAH
            long double stepLength = bBox_.length(axis) / cnt;
            long double sStep = (stepLength * bBox_.length((axis + 1) % 3) +
                                 stepLength * bBox_.length((axis + 2) % 3));
            long double sBase = bBox_.length((axis + 1) % 3) * bBox_.length((axis + 2) % 3);
            
            long double sParent = sBase + cnt * sStep;
            long double sLeft  = sBase + sStep;
            long double sRight = sParent - sStep;
            
            for (int i = 0; sLeft < sParent; sLeft += sStep, sRight -= sStep, i++) {
                int cntLeft = (int) objects_.size() - low[i + 1];
                int cntRight = (int) objects_.size() - high[i];
                long double sah = C_T + C_I * (sLeft * cntLeft + sRight * cntRight) / sParent;

                if (sah < minSah) {
                    minSah = sah;
                    minAxis = axis;
                    minProp = (1.0 + i) / cnt;
                }
            }
        }
        
        if (minAxis >= 0) {
            std::pair<BoundingBox, BoundingBox> bBoxes = bBox_.split(minAxis, minProp);
            std::vector<Object3D*> rightObjects, leftObjects;
            long double splitCoord = bBoxes.first.high(minAxis);
            
            for (int i = 0; i < objects_.size(); ++i) {
                if ((objects_[i]->boundingBox().low(minAxis) < splitCoord + EPS)) {
                    leftObjects.push_back(objects_[i]);
                }
                if ((objects_[i]->boundingBox().high(minAxis) > splitCoord - EPS)) {
                    rightObjects.push_back(objects_[i]);
                }
            }
            
            objects_.clear();
                        
            left_ = new KDNode(bBoxes.first, leftObjects);
            right_ = new KDNode(bBoxes.second, rightObjects);
            
            splitAxis_ = minAxis;
            
            left_->build();
            right_->build();
        }
    }
    
    bool intersect(Point3D start, Point3D finish, Point3D* crossPoint1, Point3D* crossPoint2) {
        return bBox_.intersect(start, finish, crossPoint1, crossPoint2);
    }
    
    bool isLeaf() {
        return splitAxis_ == -1;
    }
    
    int getSplitAxis() {
        return splitAxis_;
    }
    
    long double getSplitCoord() {
        if (right_ != NULL) {
            return right_->bBox_.low(splitAxis_);
        } else {
            return 0;
        }
    }
    
    bool contains(const Point3D& p) {
        return bBox_.contains(p);
    }

    BoundingBox bBox_;
    int splitAxis_;
    KDNode *left_, *right_;
    
    std::vector<Object3D*> objects_;
};

#endif /* kdTree_h */
