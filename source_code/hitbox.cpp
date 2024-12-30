#include "hitbox.h"
#include "model.h"
#include "utility_functions.h"
#include "constants.h"

Hitbox::Hitbox(int id, const std::vector<Point> relativeHull) : 
    id_(id),
    relativeHull_(relativeHull),
    relativeGentleSlopeTop_(findGentleSlopeTop(relativeHull)),
    relativeTop_(findTop(relativeHull)),
    // relativeLeftTop_(findLeftTop(relativeHull)),
    // relativeRightTop_(findRightTop(relativeHull)),
    relativeBottom_(findBottom(relativeHull)) {
        ownerCenterPtr_ = nullptr;
        active_ = false;
        float leftmostX = relativeHull_[support(relativeHull_, Point(-1,0))].x;
        float rightmostX = relativeHull_[support(relativeHull_, Point(1,0))].x;
        float upmostY = relativeHull_[support(relativeHull_, Point(0,1))].y;
        float downmostY = relativeHull_[support(relativeHull_, Point(-1,0))].y;
        Point lowerLeft(leftmostX, downmostY);
        Point upperRight(rightmostX, upmostY);
        relativeRectangle_ = Rectangle(lowerLeft, upperRight);
    }


Hitbox::Hitbox(int id, const Hitbox& otherHitbox) :
    id_(id),
    relativeHull_(otherHitbox.relativeHull_),
    relativeGentleSlopeTop_(otherHitbox.relativeGentleSlopeTop_),
    relativeTop_(otherHitbox.relativeTop_),
    // relativeLeftTop_(otherHitbox.relativeLeftTop_),
    // relativeRightTop_(otherHitbox.relativeRightTop_),
    relativeBottom_(otherHitbox.relativeBottom_),
    ownerCenterPtr_(otherHitbox.ownerCenterPtr_),
    active_(otherHitbox.active_),
    relativeRectangle_(otherHitbox.relativeRectangle_) {}


int Hitbox::getId() const {
    return id_;
}


Point* Hitbox::getOwnerCenterPtr() const {
    return ownerCenterPtr_;
} 


void Hitbox::setOwnerCenterPtr(Point* ownerCenterPtr) {
    ownerCenterPtr_ = ownerCenterPtr;
}


void Hitbox::makeActive() {
    active_ = true;
}


void Hitbox::makeInactive() {
    active_ = false;
}


const Rectangle Hitbox::getCurrentRectangle() const {;}

const std::vector<Point> Hitbox::getCurrentHull() const {;}

const std::vector<Point> Hitbox::getCurrentGentleSlopeTop() const {;}

const std::vector<Point> Hitbox::getCurrentTop() const {;}

const std::vector<Point> Hitbox::getCurrentBottom() const {;}


const Rectangle Hitbox::getRectangleAfterVectorTranslation(const Point& translationVector) const {;}

const std::vector<Point> Hitbox::getHullAfterVectorTranslation(const Point& translationVector) const {;}

const std::vector<Point> Hitbox::getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const {;}

const std::vector<Point> Hitbox::getTopAfterVectorTranslation(const Point& translationVector) const {;}

const std::vector<Point> Hitbox::getBottomAfterVectorTranslation(const Point& translationVector) const {;}


float Hitbox::getCurrentLeftmostX() const {;}

float Hitbox::getCurrentRightmostX() const {;}
    

bool Hitbox::collidesWith(const Hitbox& otherHitbox) const {
    const Rectangle currentRectangle = getCurrentRectangle();
    const Rectangle otherCurrentRectangle = otherHitbox.getCurrentRectangle();
    if (!(currentRectangle.collidesWith(otherCurrentRectangle))) {
        return false;
    }
    const std::vector<Point> currentHitboxHull = getCurrentHull();
    std::vector<Point> otherCurrentHitboxHull = otherHitbox.getCurrentHull();
    return gjk(currentHitboxHull, otherCurrentHitboxHull);
}


bool Hitbox::collidesWithAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const {
    const Rectangle currentRectangle = getRectangleAfterVectorTranslation(translationVector);
    const Rectangle otherCurrentRectangle = otherHitbox.getCurrentRectangle();
    if (!(currentRectangle.collidesWith(otherCurrentRectangle))) {
        // if (currentRectangle.upperRight.x - currentRectangle.lowerLeft.x < ERROR_EPS) {
        //     std::cout << currentRectangle.lowerLeft << " " << currentRectangle.upperRight << std::endl;
        // }
        
        return false;
    }
    const std::vector<Point> currentHitboxHull = getHullAfterVectorTranslation(translationVector);
    const std::vector<Point> otherCurrentHitboxHull = otherHitbox.getCurrentHull();
    
    return gjk(currentHitboxHull, otherCurrentHitboxHull);
}


bool Hitbox::isDirectlyAbove(const Hitbox& otherHitbox) const {
    const Point slightRaiseVector(0, SLIGHT_RAISE);
    const Rectangle pushedDownCurrentRectangle = getRectangleAfterVectorTranslation(-slightRaiseVector);
    const Rectangle otherCurrentRectangle = otherHitbox.getCurrentRectangle();
    if (!pushedDownCurrentRectangle.collidesWith(otherCurrentRectangle)) {
        return false;
    }
    std::vector<Point> currentHitboxHull = getHullAfterVectorTranslation(slightRaiseVector);
    const std::vector<Point> otherCurrentHitboxTop = otherHitbox.getCurrentHull();
    if (!gjk(currentHitboxHull, otherCurrentHitboxTop)) {
        for (int i = 0; i < currentHitboxHull.size(); ++i) {
            currentHitboxHull[i] = currentHitboxHull[i] - 2*slightRaiseVector;
        }
        if (gjk(currentHitboxHull, otherCurrentHitboxTop)) {
            return true;
        }
    }
    return false;
}

bool Hitbox::isDirectlyAboveAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const {
    const Point slightRaiseVector(0, SLIGHT_RAISE);
    const Rectangle pushedDownCurrentRectangle = getRectangleAfterVectorTranslation(translationVector-slightRaiseVector);
    const Rectangle otherCurrentRectangle = otherHitbox.getCurrentRectangle();
    if (!pushedDownCurrentRectangle.collidesWith(otherCurrentRectangle)) {
        return false;
    }
    std::vector<Point> currentHitboxHull = getHullAfterVectorTranslation(slightRaiseVector+translationVector);
    const std::vector<Point> otherCurrentHitboxTop = otherHitbox.getCurrentHull();
    if (!gjk(currentHitboxHull, otherCurrentHitboxTop)) {
        for (int i = 0; i < currentHitboxHull.size(); ++i) {
            currentHitboxHull[i] = currentHitboxHull[i] - 2*slightRaiseVector;
        }
        if (gjk(currentHitboxHull, otherCurrentHitboxTop)) {
            return true;
        }
    }
    return false;
}


bool Hitbox::collidesWithTopAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const {
    const Rectangle currentRectangle = getRectangleAfterVectorTranslation(translationVector);
    const Rectangle otherCurrentRectangle = otherHitbox.getCurrentRectangle();
    if (!currentRectangle.collidesWith(otherCurrentRectangle)) {
        return false;
    }
    std::vector<Point> currentHitboxHull = getHullAfterVectorTranslation(translationVector);
    const std::vector<Point> otherCurrentHitboxTop = otherHitbox.getCurrentTop();
    if (gjk(currentHitboxHull, otherCurrentHitboxTop)) {
        float sx = translationVector.x;
        Point horizontalPushVector;
        if (sx > 0) {
            horizontalPushVector = {-SLIGHT_HORIZONTAL_PUSH, 0};
        }
        else if (sx < 0) {
            horizontalPushVector = {SLIGHT_HORIZONTAL_PUSH, 0};
        }
        else {
            return true;
        }
        for (int i = 0; i < currentHitboxHull.size(); ++i) {
            currentHitboxHull[i] = currentHitboxHull[i] + horizontalPushVector;
        }
        return gjk(currentHitboxHull, otherCurrentHitboxTop);
    }
    else {
        return false;
    }
}


float Hitbox::findMinVertDistanceFromTop(const Hitbox& otherHitbox) const {
    const std::vector<Point> currentBottom = getCurrentBottom();
    const std::vector<Point> otherCurrentTop = otherHitbox.getCurrentTop();
    return minVertDistance(currentBottom, otherCurrentTop);
}


float Hitbox::findMinVertDistanceFromTopAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const {
    const std::vector<Point> currentBottom = getBottomAfterVectorTranslation(translationVector);
    const std::vector<Point> otherCurrentTop = otherHitbox.getCurrentTop();
    return minVertDistance(currentBottom, otherCurrentTop);
}


float Hitbox::isCollisionAfterVectorTranslationCausedByGentleSlope(const Hitbox& otherHitbox, const Point& translationVector) const {
    if (otherHitbox.relativeGentleSlopeTop_.size() == 0) {
        return -INFINITY;
    }
    const std::vector<Point> currentHitboxBottom = getCurrentBottom();
    const std::vector<Point> otherCurrentHitboxGentleSlopeTop = otherHitbox.getCurrentGentleSlopeTop();
    float alpha = -INFINITY;
    int currentHitboxBottomLast = currentHitboxBottom.size() - 1;
    if (translationVector.x > 0) {
        for (int i = 1; i < otherCurrentHitboxGentleSlopeTop.size(); ++i) {
            if (otherCurrentHitboxGentleSlopeTop[i-1].x - currentHitboxBottom[currentHitboxBottomLast].x >= -ERROR_EPS 
                && currentHitboxBottom[currentHitboxBottomLast].x - otherCurrentHitboxGentleSlopeTop[i].x >= -ERROR_EPS) {
                alpha = findSlopeCoefficient(otherCurrentHitboxGentleSlopeTop[i-1], otherCurrentHitboxGentleSlopeTop[i]);
                break;
            }
        }
    }
    else if (translationVector.x < 0) {
        for (int i = 1; i < otherCurrentHitboxGentleSlopeTop.size(); ++i) {
            if (otherCurrentHitboxGentleSlopeTop[i-1].x - currentHitboxBottom[0].x >= -ERROR_EPS
                && currentHitboxBottom[0].x - otherCurrentHitboxGentleSlopeTop[i].x >= -ERROR_EPS) {
                alpha = findSlopeCoefficient(otherCurrentHitboxGentleSlopeTop[i-1], otherCurrentHitboxGentleSlopeTop[i]);
                break;
            }
        }
    }
    if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
        Point vTranslationVector(translationVector.x, translationVector.x*alpha);
        if (isDirectlyAboveAfterVectorTranslation(otherHitbox, vTranslationVector)) {
            return alpha;
        } else {
            return -INFINITY;
        }
    }
    return alpha;
}


float Hitbox::findSlopeCoefficientDirectlyBelowAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const {
    const std::vector<Point> currentHitboxBottom = getBottomAfterVectorTranslation(translationVector);
    const std::vector<Point> otherCurrentHitboxTop = otherHitbox.getCurrentTop();
    int currentHitboxBottomLast = currentHitboxBottom.size()-1;
    float alpha = 0.0;
    float slopeCoefficient;
    for (int i=1; i<otherCurrentHitboxTop.size(); i++) {
        slopeCoefficient = findSlopeCoefficient(otherCurrentHitboxTop[i-1], otherCurrentHitboxTop[i]);
        if (slopeCoefficient < -ERROR_EPS) {
            if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[0].x >= -ERROR_EPS
                && currentHitboxBottom[0].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            }
        } else if (slopeCoefficient > ERROR_EPS) {
            if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[currentHitboxBottomLast].x >= -ERROR_EPS
                && currentHitboxBottom[currentHitboxBottomLast].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            }
        } else {
            if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[0].x >= -ERROR_EPS
                && currentHitboxBottom[0].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            } else if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[currentHitboxBottomLast].x >= -ERROR_EPS
                && currentHitboxBottom[currentHitboxBottomLast].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            }
        }
    }

    return alpha;
}


float Hitbox::findSlopeCoefficientDirectlyBelow(const Hitbox& otherHitbox) const {
    const std::vector<Point> currentHitboxBottom = getCurrentBottom();
    const std::vector<Point> otherCurrentHitboxTop = otherHitbox.getCurrentTop();
    int currentHitboxBottomLast = currentHitboxBottom.size()-1;
    float alpha = 0.0;
    float slopeCoefficient;
    for (int i=1; i<otherCurrentHitboxTop.size(); i++) {
        slopeCoefficient = findSlopeCoefficient(otherCurrentHitboxTop[i-1], otherCurrentHitboxTop[i]);
        if (slopeCoefficient < -ERROR_EPS) {
            if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[0].x >= -ERROR_EPS
                && currentHitboxBottom[0].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            }
        } else if (slopeCoefficient > ERROR_EPS) {
            if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[currentHitboxBottomLast].x >= -ERROR_EPS
                && currentHitboxBottom[currentHitboxBottomLast].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            }
        } else {
            if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[0].x >= -ERROR_EPS
                && currentHitboxBottom[0].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            } else if (otherCurrentHitboxTop[i-1].x - currentHitboxBottom[currentHitboxBottomLast].x >= -ERROR_EPS
                && currentHitboxBottom[currentHitboxBottomLast].x - otherCurrentHitboxTop[i].x >= -ERROR_EPS) {
                    alpha = slopeCoefficient;
                    break;
            }
        }
    }

    return alpha;
}


float Hitbox::findEscapeDisAlongXAxis(const Hitbox& otherHitbox, float soughtDirection) const {
    float currentLeftmostX = getCurrentLeftmostX();
    float currentRightmostX = getCurrentRightmostX();
    float otherCurrentLeftmostX = otherHitbox.getCurrentLeftmostX();
    float otherCurrentRightmostX = otherHitbox.getCurrentRightmostX();

    return findEscapeDisXAxisForTwoLines(currentLeftmostX, currentRightmostX, otherCurrentLeftmostX, otherCurrentRightmostX, soughtDirection);
}
