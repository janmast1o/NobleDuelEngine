#ifndef HITBOX_H
#define HITBOX_H

#include "structs_and_enums.h"
#include <vector>

class HitboxContainer;

class Hitbox {

private:

    const int id_;
    
    bool active_;
    
protected:

    Point* ownerCenterPtr_;
    Rectangle relativeRectangle_;
    
    const std::vector<Point> relativeHull_;
    const std::vector<Point> relativeGentleSlopeTop_;
    const std::vector<Point> relativeTop_;
    const std::vector<Point> relativeBottom_;

public:

    Hitbox(int id, const std::vector<Point> relativeHull);
    Hitbox(int id, const Hitbox& otherHitbox);

    int getId() const;
    Point* getOwnerCenterPtr() const;
    virtual void setOwnerCenterPtr(Point* ownerCenterPtr);

    void makeActive();
    void makeInactive();

    const Rectangle getRelativeRectangle() const;
    const std::vector<Point> getRelativeHull() const;
    const std::vector<Point> getRelativeGentleSlope() const;
    const std::vector<Point> getRelativeTop() const;
    const std::vector<Point> getRelativeBottom() const;
    
    virtual const Rectangle getCurrentRectangle() const = 0;
    virtual const std::vector<Point> getCurrentHull() const = 0;
    virtual const std::vector<Point> getCurrentGentleSlopeTop() const = 0;
    virtual const std::vector<Point> getCurrentTop() const = 0;
    virtual const std::vector<Point> getCurrentBottom() const = 0;

    virtual const Rectangle getRectangleAfterVectorTranslation(const Point& translationVector) const = 0;
    virtual const std::vector<Point> getHullAfterVectorTranslation(const Point& translationVector) const = 0;
    virtual const std::vector<Point> getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const = 0;
    virtual const std::vector<Point> getTopAfterVectorTranslation(const Point& translationVector) const = 0;
    virtual const std::vector<Point> getBottomAfterVectorTranslation(const Point& translationVector) const = 0;

    virtual float getCurrentLeftmostX() const;
    virtual float getCurrentRightmostX() const;
    
    bool collidesWith(const Hitbox& otherHitbox) const;
    bool collidesWithAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;

    bool isDirectlyAbove(const Hitbox& otherHitbox) const;
    bool isDirectlyAboveAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;

    bool collidesWithTopAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;
    float findMinDistanceAlongTheLine(const Hitbox& otherHitbox, const Point& translationVector) const;
    float findMinVertDistanceFromTop(const Hitbox& otherHitbox) const;
    float findMinVertDistanceFromTopAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;
    float isCollisionAfterVectorTranslationCausedByGentleSlope(const Hitbox& otherHitbox, const Point& translationVector) const;

    float findSlopeCoefficientDirectlyBelowAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;
    float findSlopeCoefficientDirectlyBelow(const Hitbox& otherHitbox) const; 

    float findEscapeDisAlongXAxis(const Hitbox& otherHitbox, float soughtDirection) const;

    virtual ~Hitbox() = default;

};

#endif