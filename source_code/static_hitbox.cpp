#include "static_hitbox.h"
#include "model.h"

StaticHitbox::StaticHitbox(const std::vector<Point> realtiveHull) : 
    Hitbox(realtiveHull) {}


StaticHitbox::StaticHitbox(const StaticHitbox& otherStaticHitbox) :
    Hitbox(otherStaticHitbox),
    rectangle_(otherStaticHitbox.rectangle_),
    hull_(otherStaticHitbox.hull_),
    gentleSlopeTop_(otherStaticHitbox.gentleSlopeTop_),
    top_(otherStaticHitbox.top_),
    bottom_(otherStaticHitbox.bottom_) {}


void StaticHitbox::setModelOwner(Model* newOwnerModel) {
    ownerModel_ = newOwnerModel;
    rectangle_ = relativeRectangle_;
    hull_ = relativeHull_;
    gentleSlopeTop_ = relativeGentleSlopeTop_;
    top_ = relativeTop_;
    bottom_ = relativeBottom_;
    if (newOwnerModel != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            rectangle_.lowerLeft += *objectOwnerCenterPtr;
            rectangle_.lowerRight += *objectOwnerCenterPtr;
            rectangle_.upperRight += *objectOwnerCenterPtr;
            rectangle_.upperLeft += *objectOwnerCenterPtr;
            for (int i=0; i<hull_.size(); i++) {
                hull_[i] += *objectOwnerCenterPtr;
            }
            for (int i=0; i<gentleSlopeTop_.size(); i++) {
                gentleSlopeTop_[i] += *objectOwnerCenterPtr;
            }
            for (int i=0; i<top_.size(); i++) {
                top_[i] += *objectOwnerCenterPtr;
            }
            for (int i=0; i<bottom_.size(); i++) {
                bottom_[i] += *objectOwnerCenterPtr;
            }
        }  
    }
}   


const Rectangle StaticHitbox::getCurrentRectangle() const {
    return rectangle_;
}


const std::vector<Point> StaticHitbox::getCurrentHull() const {
    return hull_;
}


const std::vector<Point> StaticHitbox::getCurrentGentleSlopeTop() const {
    return gentleSlopeTop_;
}


const std::vector<Point> StaticHitbox::getCurrentTop() const {
    return top_;
}


const std::vector<Point> StaticHitbox::getCurrentBottom() const {
    return bottom_;
}


const Rectangle StaticHitbox::getRectangleAfterVectorTranslation(const Point& translationVector) const {
    return getCurrentRectangle();
}


const std::vector<Point> StaticHitbox::getHullAfterVectorTranslation(const Point& translationVector) const {
    return getCurrentHull();
}


const std::vector<Point> StaticHitbox::getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const {
    return getCurrentGentleSlopeTop();
}


const std::vector<Point> StaticHitbox::getTopAfterVectorTranslation(const Point& translationVector) const {
    return getCurrentTop();
}


const std::vector<Point> StaticHitbox::getBottomAfterVectorTranslation(const Point& translationVector) const {
    return getCurrentBottom();
}