#include "mobile_hitbox.h"
#include "model.h"


void translateStdVectorByVector(std::vector<Point>& vectorToTranslate, const Point& translationVector) {
    int n = vectorToTranslate.size();
    for (int i=0; i<n; ++i) {
        vectorToTranslate[i] += translationVector;
    }
}


MobileHitbox::MobileHitbox(int id, const std::vector<Point> relativeHull) :
    Hitbox(id, relativeHull) {}


MobileHitbox::MobileHitbox(int id, const MobileHitbox& otherMobileHitbox) :
    Hitbox(id, otherMobileHitbox) {}


const Rectangle MobileHitbox::getCurrentRectangle() const {
    Rectangle currentRectangle = relativeRectangle_;
    if (ownerCenterPtr_ != nullptr) {
        currentRectangle.lowerLeft += *ownerCenterPtr_;
        currentRectangle.lowerRight += *ownerCenterPtr_;
        currentRectangle.upperRight += *ownerCenterPtr_;
        currentRectangle.upperLeft += *ownerCenterPtr_;
    }
    return currentRectangle;
}


const std::vector<Point> MobileHitbox::getCurrentHull() const {
    std::vector<Point> currentHull = relativeHull_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentHull, *ownerCenterPtr_);
    }
    return currentHull;
}


const std::vector<Point> MobileHitbox::getCurrentGentleSlopeTop() const {
    std::vector<Point> currentGentleSlopeTop = relativeGentleSlopeTop_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentGentleSlopeTop, *ownerCenterPtr_);
    }
    return currentGentleSlopeTop;
}


const std::vector<Point> MobileHitbox::getCurrentTop() const {
    std::vector<Point> currentTop = relativeTop_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentTop, *ownerCenterPtr_);
    }
    return currentTop;
}


const std::vector<Point> MobileHitbox::getCurrentBottom() const {
    std::vector<Point> currentBottom = relativeBottom_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentBottom, *ownerCenterPtr_);
    }
    return currentBottom;
}


const Rectangle MobileHitbox::getRectangleAfterVectorTranslation(const Point& translationVector) const {
    Rectangle currentRectangle = relativeRectangle_;
    if (ownerCenterPtr_ != nullptr) {
        currentRectangle.lowerLeft += *ownerCenterPtr_+translationVector;
        currentRectangle.lowerRight += *ownerCenterPtr_+translationVector;
        currentRectangle.upperRight += *ownerCenterPtr_+translationVector;
        currentRectangle.upperLeft += *ownerCenterPtr_+translationVector;
    }
    return currentRectangle;
}


const std::vector<Point> MobileHitbox::getHullAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentHull = relativeHull_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentHull, *ownerCenterPtr_+translationVector);
    }
    return currentHull;
}


const std::vector<Point> MobileHitbox::getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentGentleSlopeTop = relativeGentleSlopeTop_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentGentleSlopeTop, *ownerCenterPtr_+translationVector);
    }
    return currentGentleSlopeTop;
}


const std::vector<Point> MobileHitbox::getTopAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentTop = relativeTop_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentTop, *ownerCenterPtr_+translationVector);
    }
    return currentTop;
}


const std::vector<Point> MobileHitbox::getBottomAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentBottom = relativeBottom_;
    if (ownerCenterPtr_ != nullptr) {
        translateStdVectorByVector(currentBottom, *ownerCenterPtr_+translationVector);
    }
    return currentBottom;
}