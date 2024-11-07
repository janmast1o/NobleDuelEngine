#include "mobile_hitbox.h"
#include "model.h"


void translateStdVectorByVector(std::vector<Point>& vectorToTranslate, const Point& translationVector) {
    int n = vectorToTranslate.size();
    for (int i=0; i<n; i++) {
        vectorToTranslate[i] += translationVector;
    }
}


MobileHitbox::MobileHitbox(const std::vector<Point> relativeHull) :
    Hitbox(relativeHull) {}


MobileHitbox::MobileHitbox(const MobileHitbox& otherMobileHitbox) :
    Hitbox(otherMobileHitbox) {}


const Rectangle MobileHitbox::getCurrentRectangle() const {
    Rectangle currentRectangle = relativeRectangle_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            currentRectangle.lowerLeft += *objectOwnerCenterPtr;
            currentRectangle.lowerRight += *objectOwnerCenterPtr;
            currentRectangle.upperRight += *objectOwnerCenterPtr;
            currentRectangle.upperLeft += *objectOwnerCenterPtr;
        }
    }
    return currentRectangle;
}


const std::vector<Point> MobileHitbox::getCurrentHull() const {
    std::vector<Point> currentHull = relativeHull_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentHull, *objectOwnerCenterPtr);
        }
    }
    return currentHull;
}


const std::vector<Point> MobileHitbox::getCurrentGentleSlopeTop() const {
    std::vector<Point> currentGentleSlopeTop = relativeGentleSlopeTop_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentGentleSlopeTop, *objectOwnerCenterPtr);
        }
    }
    return currentGentleSlopeTop;
}


const std::vector<Point> MobileHitbox::getCurrentTop() const {
    std::vector<Point> currentTop = relativeTop_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentTop, *objectOwnerCenterPtr);
        }
    }
    return currentTop;
}


const std::vector<Point> MobileHitbox::getCurrentBottom() const {
    std::vector<Point> currentBottom = relativeBottom_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentBottom, *objectOwnerCenterPtr);
        }
    }
    return currentBottom;
}


const Rectangle MobileHitbox::getRectangleAfterVectorTranslation(const Point& translationVector) const {
    Rectangle currentRectangle = relativeRectangle_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            Point v = *objectOwnerCenterPtr + translationVector;
            currentRectangle.lowerLeft += v;
            currentRectangle.lowerRight += v;
            currentRectangle.upperRight += v;
            currentRectangle.upperLeft += v;
        }
    }
    return currentRectangle;
}


const std::vector<Point> MobileHitbox::getHullAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentHull = relativeHull_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentHull, *objectOwnerCenterPtr+translationVector);
        }
    }
    return currentHull;
}


const std::vector<Point> MobileHitbox::getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentGentleSlopeTop = relativeGentleSlopeTop_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentGentleSlopeTop, *objectOwnerCenterPtr+translationVector);
        }
    }
    return currentGentleSlopeTop;
}


const std::vector<Point> MobileHitbox::getTopAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentTop = relativeTop_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentTop, *objectOwnerCenterPtr+translationVector);
        }
    }
    return currentTop;
}


const std::vector<Point> MobileHitbox::getBottomAfterVectorTranslation(const Point& translationVector) const {
    std::vector<Point> currentBottom = relativeBottom_;
    if (ownerModel_ != nullptr) {
        Point* objectOwnerCenterPtr = ownerModel_->getCurrentOwnerCenterPtr();
        if (objectOwnerCenterPtr != nullptr) {
            translateStdVectorByVector(currentBottom, *objectOwnerCenterPtr+translationVector);
        }
    }
    return currentBottom;
}