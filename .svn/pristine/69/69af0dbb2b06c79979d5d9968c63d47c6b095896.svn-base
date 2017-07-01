/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   routeInstruction.h
 * Author: liuhao22
 *
 * Created on March 18, 2017, 2:05 AM
 */

#ifndef ROUTEINSTRUCTION_H
#define ROUTEINSTRUCTION_H

#include "load_map.h"
#include <vector>
#include <cmath>

#define segVec (*getPtrToStSegSet())
#define itsVec (*getPtrToIntersectSet())

enum direction {
    STRAIGHT = 0,
    LEFT,
    RIGHT
};

enum polar {
    NORTH = 0,
    SOUTH,
    WEST,
    EAST
};

// Helper class for judging direction
// equation y(x) = m*x + b
class equation {
public :
    equation();
    
    // Calculate y(x) for a given x
    double y(double x);
    
    void update(double _m, double _b);
    
    int slopeIsNegative();
    
private:
    double m;
    double b;
};

class routeInstruction {
public:
    
    routeInstruction();

    routeInstruction(const std::vector<unsigned> & route, std::string destination);

    void printAllInstructions();
    
    unsigned getNumOfInstructions();
    
    std::pair< std::pair< StreetSegmentIndex, StreetSegmentIndex >, std::pair<std::string, std::string > > getOneInstructPair(unsigned index);
    
private:
    
    unsigned instructionCount;
    
    unsigned polarDirection(unsigned curSegIdx, unsigned nextSegIdx);
    
    unsigned findDirection(unsigned curSeg, unsigned nextSeg);
    
    std::pair< std::string, std::string > makeInstruction(unsigned prevIndex, unsigned nextIndex, float distSum);
    
    // A copy of passed in route
    std::vector<unsigned> path;
    
    // Instructions will be stored in following manna
    //  { { {lastSegmentId, nextSegmentId} , "instruction within two segments" } ,
    //    { {lastSegmentId, nextSegmentId} , "instruction within two segments" } ,
    //    ......                                                                    }
    std::vector< std::pair< std::pair< StreetSegmentIndex, StreetSegmentIndex >, std::pair<std::string, std::string > > > instructionStorage;
    
    equation equ;
};

#endif /* ROUTEINSTRUCTION_H */

