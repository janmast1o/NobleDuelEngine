#include "static_hitbox.h"
#include "model.h"

StaticHitbox::StaticHitbox(int id, const std::vector<Point> realtiveHull) : 
    Hitbox(id, realtiveHull),
    rectangle_(relativeRectangle_),
    hull_(relativeHull_),
    gentleSlopeTop_(relativeGentleSlopeTop_),
    top_(relativeTop_),
    bottom_(relativeBottom_) {}


StaticHitbox::StaticHitbox(int id, const StaticHitbox& otherStaticHitbox) :
    Hitbox(id, otherStaticHitbox),
    rectangle_(otherStaticHitbox.rectangle_),
    hull_(otherStaticHitbox.hull_),
    gentleSlopeTop_(otherStaticHitbox.gentleSlopeTop_),
    top_(otherStaticHitbox.top_),
    bottom_(otherStaticHitbox.bottom_) {}


void StaticHitbox::setOwnerCenterPtr(Point* ownerCenterPtr) {
    ownerCenterPtr_ = ownerCenterPtr;
    rectangle_ = relativeRectangle_;
    hull_ = relativeHull_;
    gentleSlopeTop_ = relativeGentleSlopeTop_;
    top_ = relativeTop_;
    bottom_ = relativeBottom_;
    if (ownerCenterPtr_ != nullptr) {
        rectangle_.lowerLeft += *ownerCenterPtr_;
        rectangle_.lowerRight += *ownerCenterPtr_;
        rectangle_.upperRight += *ownerCenterPtr_;
        rectangle_.upperLeft += *ownerCenterPtr_;
        for (int i=0; i<hull_.size(); i++) {
            hull_[i] += *ownerCenterPtr_;
        }
        for (int i=0; i<gentleSlopeTop_.size(); i++) {
            gentleSlopeTop_[i] += *ownerCenterPtr_;
        }
        for (int i=0; i<top_.size(); i++) {
            top_[i] += *ownerCenterPtr_;
        }
        for (int i=0; i<bottom_.size(); i++) {
            bottom_[i] += *ownerCenterPtr_;
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