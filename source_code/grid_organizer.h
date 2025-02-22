#ifndef GRID_ORGANIZER_H
#define GRID_ORGANIZER_H

#include <unordered_map>
#include <unordered_set>
#include "structs_and_enums.h"

template <typename T>
class GridOrganizer {

private:

    std::unordered_map<T, std::list<std::pair<int, int>>> cellsCorrespondingToInstances_;
    std::vector<std::unordered_set<T>> gridCells_;
    std::unordered_set<T> outsideTheGrid_;
    
    Rectangle celledRectangle_;
    int numOfRows_;
    int numOfCols_;
    float cellWidth_; // width wise size
    float cellHeight_; // hight wise size

    std::unordered_set<T>& getCell(int r, int c) {
        return gridCells_[r*numOfCols_+c];
    }

public:    

    GridOrganizer(const Rectangle& celledRectangle, int numOfRows, int numOfCols) : 
        celledRectangle_(celledRectangle), numOfRows_(numOfRows), numOfCols_(numOfCols),
        cellWidth_(celledRectangle.getWidth() / numOfRows),
        cellHeight_(celledRectangle.getHeight() / numOfCols) {

        gridCells_ = std::vector<std::unordered_set<T>>(numOfRows*numOfCols, std::unordered_set<T>());
    }


    void emplaceInGrid(const T& newElement, const Rectangle& rectangle) {
        if (cellsCorrespondingToInstances_.find(newElement) != cellsCorrespondingToInstances_.end()) {
            for (std::pair<int, int>& p : cellsCorrespondingToInstances_.at(newElement)) {
                getCell(p.first, p.second).erase(newElement);
            }
            
            cellsCorrespondingToInstances_.at(newElement).clear();
        }

        outsideTheGrid_.erase(newElement);
        
        Point lowerLeft = rectangle.lowerLeft;
        Point upperRight = rectangle.upperRight;

        int iMin, iMax, jMin, jMax;
        iMin = ((float) lowerLeft.x - celledRectangle_.lowerLeft.x) / cellWidth_;
        iMax = ((float) upperRight.x - celledRectangle_.lowerLeft.x) / cellWidth_;
        jMin = ((float) lowerLeft.y - celledRectangle_.lowerLeft.y) / cellHeight_;
        jMax = ((float) upperRight.y - celledRectangle_.lowerLeft.y) / cellHeight_;

        int iMinDash, iMaxDash, jMinDash, jMaxDash;
        iMinDash = std::max(0, iMin);
        iMaxDash = std::min(iMax, (int) (celledRectangle_.getWidth() / cellWidth_)-1);
        jMinDash = std::max(0, jMin);
        jMaxDash = std::min(jMax, (int) (celledRectangle_.getHeight() / cellHeight_)-1);

        if (cellsCorrespondingToInstances_.find(newElement) == cellsCorrespondingToInstances_.end()) {
            cellsCorrespondingToInstances_.emplace(newElement, std::list<std::pair<int, int>>());
        }
        
        for (int i=iMinDash; i<=iMaxDash; ++i) {
            for (int j=jMinDash; j<=jMaxDash; ++j) {
                getCell(i,j).emplace(newElement);
                cellsCorrespondingToInstances_.at(newElement).emplace_back(std::pair<int, int>(i, j));
            }
        }

        if (iMin != iMinDash || iMax != iMaxDash || 
            jMin != jMinDash || jMax != jMaxDash) {

            outsideTheGrid_.emplace(newElement);
        }

    }


    void removeFromGrid(const T& element) {
        if (cellsCorrespondingToInstances_.find(element) != cellsCorrespondingToInstances_.end()) {
            for (std::pair<int, int>& p : cellsCorrespondingToInstances_.at(element)) {
                getCell(p.first, p.second).erase(element);
            }

            cellsCorrespondingToInstances_.at(element).clear(); 
            cellsCorrespondingToInstances_.erase(element);
        }

        outsideTheGrid_.erase(element);
        
    }


    std::list<T> getPotentiallyColliding(Rectangle collisionRectangle) {
        Point lowerLeft = collisionRectangle.lowerLeft;
        Point upperRight = collisionRectangle.upperRight;

        std::unordered_set<T> potentiallyColliding;
        
        int iMin, iMax, jMin, jMax;
        iMin = ((float) lowerLeft.x - celledRectangle_.lowerLeft.x) / cellWidth_;
        iMax = ((float) upperRight.x - celledRectangle_.lowerLeft.x) / cellWidth_;
        jMin = ((float) lowerLeft.y - celledRectangle_.lowerLeft.y) / cellHeight_;
        jMax = ((float) upperRight.y - celledRectangle_.lowerLeft.y) / cellHeight_;

        int iMinDash, iMaxDash, jMinDash, jMaxDash;
        iMinDash = std::max(0, iMin);
        iMaxDash = std::min(iMax, (int) (celledRectangle_.getWidth() / cellWidth_)-1);
        jMinDash = std::max(0, jMin);
        jMaxDash = std::min(jMax, (int) (celledRectangle_.getHeight() / cellHeight_)-1);

        for (int i=iMinDash; i<=iMaxDash; ++i) {
            for (int j=jMinDash; j<=jMaxDash; ++j) {
                std::unordered_set<T>& set = getCell(i,j);
                for (auto& t : set) {
                    potentiallyColliding.emplace(t);
                }
            }
        }

        if (iMin != iMinDash || iMax != iMaxDash || 
            jMin != jMinDash || jMax != jMaxDash) {

            std::unordered_set<T>& set = outsideTheGrid_;
            for (auto& t : set) {
                potentiallyColliding.emplace(t);
            }
        }

        return std::list<T>(potentiallyColliding.begin(), potentiallyColliding.end());
    }


    std::list<T> getPotentiallyColliding(Rectangle collisionRectangle, Point translationVector) {
        return getPotentiallyColliding(collisionRectangle.getSmallestRectContainingBoth(collisionRectangle+translationVector));
    }

};

#endif