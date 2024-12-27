#include "utility_functions.h"
#include "constants.h"
#include <vector>
#include <list>
#include <optional>

void drawPoint(SDL_Renderer* renderer, float x, float y, float pointSize) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect pointRect;
    pointRect.x = x - pointSize/2;
    pointRect.y = y - pointSize/2;
    pointRect.w = pointSize;
    pointRect.h = pointSize;
    SDL_RenderFillRectF(renderer, &pointRect);
}


float dotProduct(const Point& A, const Point& B) {
    return A.x*B.x + A.y*B.y;
}


Point tripleProduct(const Point& A, const Point& B, const Point& C) {
    float alpha = A.x*B.y - A.y*B.x;
    return Point(-alpha*C.y, alpha*C.x);
}


int support(const std::vector<Point>& S, const Point& d) {
    int sought = 0;
    float soughtDotProduct = dotProduct(S[0], d);
    Point v;
    float currentDotProduct;
    int n = S.size();

    for (int i=1; i<n; ++i) {
        v = S[i];
        currentDotProduct = dotProduct(v, d);
        if (currentDotProduct > soughtDotProduct) {
            sought = i;
            soughtDotProduct = currentDotProduct;
        }
    }

    return sought;
}


float findSlopeCoefficient(const Point& A, const Point& B) {
    if (std::abs(A.x-B.x) < ERROR_EPS) {
        return INFINITY;
    } else {
        return (A.y-B.y) / (A.x-B.x);
    }
}


std::pair<float, float> findLineCoefficients(const Point& A, const Point& B) {
    float a = findSlopeCoefficient(A, B);
    float b = A.y - a*A.x;
    return {a, b};
}


unsigned int convertPairToFrames(std::pair<unsigned int, unsigned int>& P, int framesInCycle) {
    return P.first*framesInCycle+P.second;
}


float seeIfLinesOverlapAlongXAxis(float firstStartX, float firstEndX, float secondStartX, float secondEndX) {
    return !(firstStartX > secondEndX || firstEndX < secondStartX);
}


float findEscapeDisXAxisForTwoLines(float firstStartX, float firstEndX, float secondStartX, float secondEndX, float soughtDirection) {
    if (soughtDirection < 0) {
        float toTheLeft = 0;
        if (!seeIfLinesOverlapAlongXAxis(firstStartX, firstEndX, secondStartX, secondEndX)) {;}
        else {
            toTheLeft = secondStartX - firstEndX;
        }

        return toTheLeft;
    } else {
        float toTheRight = 0;
        if (!seeIfLinesOverlapAlongXAxis(firstStartX, firstEndX, secondStartX, secondEndX)) {;}
        else {
            toTheRight = secondEndX - firstStartX;
        }

        return toTheRight;
    }

}


float calculateVelocityAfterCollision(float mass, float hVelocity, float otherMass, float otherHVelocity) {
    return ((mass-otherMass)*hVelocity + 2*otherMass*otherHVelocity) / (mass+otherMass);
}


std::vector<Point> findGentleSlopeTop(const std::vector<Point>& polygon) {
    float maxSlopeCoefficient = MAXIMUM_GENTLE_SLOPE_COEFFICIENT;
    int n = polygon.size();
    int leftmostIndex = support(polygon, Point(-1, 0));
    int rightmostIndex = support(polygon, Point(1, 0));
    int i = rightmostIndex+1;
    int start, end;

    while (true) {
        if (std::abs(findSlopeCoefficient(polygon[i%n], polygon[(i-1)%n])) <= maxSlopeCoefficient) {
            break;
        } else if (i%n == leftmostIndex) {
            ++i;
            break;
        } else {
            ++i;
        }
    }
    start = (i-1)%n;

    while (true) {
        if (std::abs(findSlopeCoefficient(polygon[i%n], polygon[(i-1)%n])) > maxSlopeCoefficient) {
            break;
        } else if (i%n == leftmostIndex) {
            ++i;
            break;
        } else {
            ++i;
        }
    }
    end = (i-1)%n;

    if (start == end) {
        return std::vector<Point>();
    } else {
        std::vector<Point> gentleSlopeTop;
        i = start;
        while (true) {
            gentleSlopeTop.push_back(polygon[i%n]);
            if (i%n == end) {
                break;
            } else {
                ++i;
            }
        }
        return gentleSlopeTop;
    }
}


std::vector<Point> findTop(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int leftmostIndex = 0;
    int rightmostIndex = 0;

    for (int i=1; i<n; ++i) {
        if (polygon[i].x < polygon[leftmostIndex].x) {
            leftmostIndex = i;
        } else if (std::abs(polygon[i].x - polygon[leftmostIndex].x) < ERROR_EPS && polygon[i].y > polygon[leftmostIndex].y) {
            leftmostIndex = i;
        }
        if (polygon[i].x > polygon[rightmostIndex].x) {
            rightmostIndex = i;
        } else if (std::abs(polygon[i].x - polygon[rightmostIndex].x) < ERROR_EPS && polygon[i].y > polygon[rightmostIndex].y) {
            rightmostIndex = i;
        }
    }

    std::vector<Point> top;
    int i = rightmostIndex;
    while (true) {
        top.push_back(polygon[i%n]);
        if (i % n == leftmostIndex) {
            break;
        } else {
            ++i;
        }
    }
    return top;
}


std::vector<Point> findLeftTop(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int topmostIndex = support(polygon, Point(0, 1));
    int leftmostIndex = support(polygon, Point(-1, 0));
    std::vector<Point> leftTop;
    int i = topmostIndex;

    while (true) {
        leftTop.push_back(polygon[i%n]);
        if (i%n == leftmostIndex) {
            break;
        } else {
            ++i;
        }
    }
    return leftTop;
}


std::vector<Point> findRightTop(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int topmostIndex = support(polygon, Point(0, 1));
    int rightmostIndex = support(polygon, Point(1, 0));
    std::vector<Point> rightTop;
    int i = rightmostIndex;

    while (true) {
        rightTop.push_back(polygon[i%n]);
        if (i%n == topmostIndex) {
            break;
        } else {
            ++i;
        }
    }
    return rightTop;
}


std::vector<Point> findBottom(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int lowest = support(polygon, Point(0, -1));
    float lowestY = polygon[lowest].y;
    std::vector<Point> bottom;

    if (lowest == 0) {
        if (std::abs(polygon[n-1].y - lowestY) < ERROR_EPS) {
            bottom = {polygon[lowest], polygon[n-1]};
        } else if (polygon[(lowest+1)%n].y == lowestY) {
            bottom = {polygon[lowest], polygon[(lowest+1)%n]};
        } else {
            bottom = {polygon[lowest]};
        }
    } else {
        if (std::abs(polygon[lowest-1].y - lowestY) < ERROR_EPS) {
            bottom = {polygon[lowest], polygon[lowest-1]};
        } else if (polygon[(lowest+1)%n].y == lowestY) {
            bottom = {polygon[lowest], polygon[(lowest+1)%n]};
        } else {
            bottom = {polygon[lowest]};
        }
    }
    if (bottom.size() == 2 && bottom[0].x > bottom[1].x) {
        std::swap(bottom[0], bottom[1]);
    }
    return bottom;
}


bool gjk(const std::vector<Point>& S, const std::vector<Point>& R) {
    if (S.size() == 0 || R.size() == 0) {
        return false;
    }

    Point d(1, 0);
    Point origin(0, 0);
    Point A, B, C, AB, AC, AO, ABPerpendicular, ACPerpendicular;
    A = S[support(S, d)] - R[support(R, -d)];
    d = origin - A;
    std::vector<Point> simplex = {A};

    while (true) {
        A = S[support(S, d)] - R[support(R, -d)];
        if (dotProduct(A, d) < 0) {
            return false;
        } else {
            simplex.push_back(A);
            if (simplex.size() == 2) {
                B = simplex[0];
                A = simplex[1];
                AB = B-A;
                AO = origin-A;
                ABPerpendicular = tripleProduct(AB, AO, AB);
                d = ABPerpendicular;
            } else {
                C = simplex[0];
                B = simplex[1];
                A = simplex[2];
                AO = origin-A;
                AB = B-A;
                AC = C-A;
                ABPerpendicular = tripleProduct(AC, AB, AB);
                ACPerpendicular = tripleProduct(AB, AC, AC);
                if (dotProduct(ABPerpendicular, AO) > ERROR_EPS) {
                    simplex.erase(simplex.begin());
                    d = ABPerpendicular;
                } else if (dotProduct(ACPerpendicular, AO) > ERROR_EPS) {
                    simplex.erase(simplex.begin() + 1);
                    d = ACPerpendicular;
                } else {
                    return true;
                }
            }
        }
    }
}


void fillT(typename std::list<TEntry>::iterator it, int startEl, int endEl) {
    int i = startEl;
    while (i <= endEl) {
        if (!it->sPoint.has_value()) {
            float x = it->rPoint.value().x;
            std::pair<float, float> coeff = findLineCoefficients(it->previousSPoint.value(), it->nextSPoint.value());
            it->sPoint = {x, coeff.first*x+coeff.second};
        } else if (!it->rPoint.has_value()) {
            float x = it->sPoint.value().x;
            std::pair<float, float> coeff = findLineCoefficients(it->previousRPoint.value(), it->nextRPoint.value());
            it->rPoint = {x, coeff.first*x+coeff.second};
        }
        ++i;
        ++it;
    }
}


float minVertDistance(const std::vector<Point>& S, const std::vector<Point>& R) {
    int startS, endS, incS, startR, endR, incR;
    if (S[0].x < S.back().x) {
        startS = 0;
        endS = S.size();
        incS = 1;
    } else {
        startS = S.size()-1;
        endS = -1;
        incS = -1;
    }
    if (R[0].x < R.back().x) {
        startR = 0;
        endR = R.size();
        incR = 1;
    } else {
        startR = R.size()-1;
        endR = -1;
        incR = -1;
    }

    int i = startS;
    int j = startR;
    std::list<TEntry> T;
    while (i != endS && j != endR) {
        T.emplace_back();
        if (std::abs(S[i].x-R[j].x) < ERROR_EPS) {
            T.back().sPoint = S[i];
            T.back().rPoint = R[j];
            i += incS;
            j += incR;
        } else if (S[i].x < R[j].x) {
            T.back().sPoint = S[i];
            i += incS;
        } else {
            T.back().rPoint = R[j];
            j += incR;
        }
    }

    if (i == endS) {
        while (j != endR) {
            T.emplace_back();
            T.back().rPoint = R[j];
            j += incR;
        }
    } else {
        while (i != endS) {
            T.emplace_back();
            T.back().sPoint = S[i];
            i += incS;
        }
    }

    std::optional<Point> previous, next;
    for (auto it=T.begin(); it != T.end(); ++it) {
        if (it->sPoint.has_value()) {
            previous = it->sPoint.value();
        } else {
            it->previousSPoint = previous;
        }
    }

    for (auto it=T.rbegin(); it != T.rend(); ++it) {
        if (it->sPoint.has_value()) {
            next = it->sPoint.value();
        } else {
            it->nextSPoint = next;
        }
    } 

    previous.reset();
    next.reset();
    for (auto it=T.begin(); it != T.end(); ++it) {
        if (it->rPoint.has_value()) {
            previous = it->rPoint.value();
        } else {
            it->previousRPoint = previous;
        }
    }

    for (auto it=T.rbegin(); it != T.rend(); ++it) {
        if (it->rPoint.has_value()) {
            next = it->rPoint.value();
        } else {
            it->nextRPoint = next;
        }
    } 

    std::list<TEntry>::iterator start;
    int startEl, endEl;
    startEl = 0;
    for (auto it=T.begin(); it != T.end(); ++it) {
        if (it->sPoint.has_value() && it->rPoint.has_value()) {
            start = it;
            break;
        } else if (it->sPoint.has_value() && it->previousRPoint.has_value()) {
            start = it;
            break;
        } else if (it->rPoint.has_value() && it->previousSPoint.has_value()) {
            start = it;
            break;
        }
        ++startEl;
    }

    endEl = T.size()-1;
    for (auto it=T.rbegin(); it != T.rend(); ++it) {
        if (it->sPoint.has_value() && it->rPoint.has_value()) {
            break;
        } else if (it->sPoint.has_value() && it->nextRPoint.has_value()) {
            break;
        } else if (it->rPoint.has_value() && it->nextSPoint.has_value()) {
            break;
        }
        --endEl;
    }

    if (startEl > endEl) {
        return -1;
    } else {
        int i = startEl;
        fillT(start, startEl, endEl);
        float minDistance = INFINITY;
        float currentMin;
        while (i <= endEl) {
            if ((currentMin=start->sPoint.value().y - start->rPoint.value().y) < minDistance) {
                minDistance = currentMin;
            }
            ++start;
            ++i;
        }
        return minDistance;
    }
}


bool isLeftFacing(State state) {
    return state%3 == 0;
}


bool isRightFacing(State state) {
    return state%3 == 2;
}
