#ifndef STATIC_HITBOX_H
#define STATIC_HITBOX_H

#include "hitbox.h"

class StaticHitbox : public Hitbox {

private:

    Rectangle rectangle_;
    
    std::vector<Point> hull_;
    std::vector<Point> gentleSlopeTop_;
    std::vector<Point> top_;
    std::vector<Point> bottom_;

public:

    StaticHitbox(int id, const std::vector<Point> relativeHull);
    StaticHitbox(int id, const StaticHitbox& otherStaticHitbox);

    void setOwnerCenterPtr(Point* ownerCenterPtr) override;

    const Rectangle getCurrentRectangle() const override;
    const std::vector<Point> getCurrentHull() const override;
    const std::vector<Point> getCurrentGentleSlopeTop() const override;
    const std::vector<Point> getCurrentTop() const override;
    const std::vector<Point> getCurrentBottom() const override;

    const Rectangle getRectangleAfterVectorTranslation(const Point& translationVector) const override;
    const std::vector<Point> getHullAfterVectorTranslation(const Point& translationVector) const override;
    const std::vector<Point> getGentleSlopeTopAfterVectorTranslation(const Point& translationVector) const override;
    const std::vector<Point> getTopAfterVectorTranslation(const Point& translationVector) const override;
    const std::vector<Point> getBottomAfterVectorTranslation(const Point& translationVector) const override;

    float getCurrentLeftmostX() const override;
    float getCurrentRightmostX() const override;

};

#endif