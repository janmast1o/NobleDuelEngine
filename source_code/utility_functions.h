#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include "structs_and_enums.h"
#include "constants.h"
#include <vector>
#include <list>

void drawPoint(SDL_Renderer* renderer, float x, float y, float pointSize);

template <typename T>
T getSign(T x) {
    if (std::abs(x) <= ERROR_EPS) {
        return 0;
    } else if (x > 0) {
        return 1;
    } else {
        return -1;
    }
}

float dotProduct(const Point& A, const Point& B);

Point tripleProduct(const Point& A, const Point& B, const Point& C);

int support(const std::vector<Point>& S, const Point& d);

float findSlopeCoefficient(const Point& A, const Point& B);

std::pair<float, float> findLineCoefficients(const Point& A, const Point& B);

float seeIfLinesOverlapAlongXAxis(float firstStartX, float firstEndX, float secondStartX, float secondEndX);

float findEscapeDisXAxisForTwoLines(float firstStartX, float firstEndX, float secondStartX, float secondEndX, float soughtDirection);

float calculateVelocityAfterCollision(float mass, float hVelocity, float otherMass, float otherHVelocity);

std::vector<Point> findGentleSlopeTop(const std::vector<Point>& polygon);

std::vector<Point> findTop(const std::vector<Point>& polygon);

std::vector<Point> findLeftTop(const std::vector<Point>& polygon);

std::vector<Point> findRightTop(const std::vector<Point>& polygon);

std::vector<Point> findBottom(const std::vector<Point>& polygon);

bool gjk(const std::vector<Point>& S, const std::vector<Point>& R);

void fillT(typename std::list<TEntry>::iterator it, int startEl, int endEl);

float minVertDistance(const std::vector<Point>& S, const std::vector<Point>& R);

bool isLeftFacing(State state);

bool isRightFacing(State state);

#endif