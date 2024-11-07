#ifndef HITBOX_H
#define HITBOX_H

#include "structs_and_enums.cpp"
#include <vector>

class Model;

class Hitbox {

private:

    bool active_;
    
protected:

    Model* ownerModel_;
    Rectangle relativeRectangle_;
    
    const std::vector<Point> relativeHull_;
    const std::vector<Point> relativeGentleSlopeTop_;
    const std::vector<Point> relativeTop_;
    // const std::vector<Point> relativeLeftTop_;
    // const std::vector<Point> relativeRightTop_; // probaby should remove it
    const std::vector<Point> relativeBottom_;

public:

    Hitbox(const std::vector<Point> relativeHull);
    Hitbox(const Hitbox& otherHitbox);

    virtual void setModelOwner(Model* newOwnerModel);

    void makeActive();
    void makeInactive();

    virtual const Rectangle getCurrentRectangle() const;
    virtual const std::vector<Point> getCurrentHull() const;
    virtual const std::vector<Point> getCurrentGentleSlopeTop() const;
    virtual const std::vector<Point> getCurrentTop() const;
    // virtual const std::vector<Point> getCurrentLeftTop() const;
    // virtual const std::vector<Point> getCurrentRightTop() const;
    virtual const std::vector<Point> getCurrentBottom() const;

    virtual const Rectangle getRectangleAfterVectorTranslation(const Point& translationVector) const;
    virtual const std::vector<Point> getHullAfterVectorTranslation(const Point& translationVector) const;
    virtual const std::vector<Point> getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const;
    virtual const std::vector<Point> getTopAfterVectorTranslation(const Point& translationVector) const;
    // virtual const std::vector<Point> getLeftTopAfterVectorTranslation(const Point& translationVector) const;
    // virtual const std::vector<Point> getRightTopAfterVectorTranslation(const Point& translationVector) const;
    virtual const std::vector<Point> getBottomAfterVectorTranslation(const Point& translationVector) const;

    bool collidesWith(const Hitbox& otherHitbox) const;
    bool collidesWithAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;

    bool isDirectlyAbove(const Hitbox& otherHitbox) const;
    bool isDirectlyAboveAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;

    bool collidesWithTopAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;
    float isCollisionAfterVectorTranslationCausedByGentleSlope(const Hitbox& otherHitbox, const Point& translationVector) const;

    float findSlopeCoefficientDirectlyBelowAfterVectorTranslation(const Hitbox& otherHitbox, const Point& translationVector) const;
     
};

#endif