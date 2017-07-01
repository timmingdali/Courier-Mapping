/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   routeInstruction.cpp
 * Author: liuhao22
 * 
 * Created on March 18, 2017, 2:05 AM
 */

//#include <bits/basic_string.h>

#include "routeInstruction.h"

using namespace std;

/*************************** Implementation of equation ************************/
equation::equation() {

}

inline double equation::y(double x) {
    return m*x + b;
}

inline void equation::update(double _m, double _b) {
    m = _m;
    b = _b;
}

// Return 1 for positive slope,
// -1 for negative slope
// 0 for 0 slope
inline int equation::slopeIsNegative() {
    if (m > 0)
        return 1;
    else if (m < 0)
        return -1;
    else
        return 0;
}

routeInstruction::routeInstruction() {
    
}

routeInstruction::routeInstruction(const vector<unsigned> & route, string destination) {
    size_t prevIndex = 0;
    
    instructionCount = 0;
    
    path = route;
    
    equ = equation();
    
    double distSum = segVec[route[0]].segLength;
    
    // Starting instruction
    string instruction = "Head ";
    
    // if only one segment that have to be went along
    if (route.size() == 1)
    {
        instruction += "a long the street. ";
    }
    else
    {
        auto polarDirect = polarDirection(route[0], route[1]);

        switch (polarDirect)
        {
            case NORTH:
                instruction += "north";
                break;

            case SOUTH:
                instruction += "south";
                break;

            case WEST:
                instruction += "west";
                break;

            case EAST:
                instruction += "east";
                break;

            default:
                cerr << "Not figured out the correct direction" << endl;
                exit(-1);
        }
    }
    
    auto outerSegPair = make_pair( 0, 0 );
    pair< string, string > instructionPair;
    instructionPair = make_pair( "", instruction);
    auto outerPair = make_pair( outerSegPair, instructionPair );
    instructionStorage.push_back(outerPair);
    instructionCount++;
    
    for (size_t nextIndex = 1; nextIndex < route.size(); ++nextIndex) 
    {
        // Make instruction whenever street change detects
        if (segVec[ route[prevIndex] ].streetName == segVec[ route[nextIndex] ].streetName)
        {
            distSum += segVec[route[nextIndex]].segLength;
            continue;
        }
        // Street changed, make instruction
        else
        {
            // Preparing adding a instruction into instructionStorage
            auto segPair = make_pair( route[prevIndex], route[nextIndex] );
            auto instructions = makeInstruction(prevIndex, nextIndex, distSum);
            auto instructPair = make_pair( segPair, instructions );
            instructionStorage.push_back(instructPair);
            
            instructionCount++;
            
            // iterate indexes, reset distance summation
            prevIndex = nextIndex;
            distSum = segVec[route[prevIndex]].segLength;
        }
    }
    
    string distIns = "Driving along for ";
    distIns.append(to_string( static_cast<int>(distSum) - static_cast<int>(distSum) % 1 ) + " meters. ");
    instruction = "Arriving destination " + destination;
    
    outerSegPair = make_pair( 0, 0 );
    instructionPair = make_pair( distIns, instruction);
    outerPair = make_pair( outerSegPair, instructionPair );
    instructionStorage.push_back(outerPair);
    instructionCount++;
}

void routeInstruction::printAllInstructions() {
    for (const auto &eachInstruction: instructionStorage)
    {
        cout << "From segment " << eachInstruction.first.first << " to segment " << eachInstruction.first.second <<
                "\nInstruction: " << eachInstruction.second.first << endl <<
                "\t " << eachInstruction.second.second << endl << endl;
    }
}

pair<string, string> routeInstruction::makeInstruction(unsigned prevIndex, unsigned nextIndex, float distSum) {
    
    auto direction = findDirection( path[nextIndex - 1] , path[nextIndex] );
    
    string distInfo;
    string instruction = "";
    
    string curStreetName = segVec[ path[prevIndex] ].streetName;
    
    if (distSum != 0 && segVec[ path[prevIndex] ].streetName != "<unknown>")
    {
        distInfo = "Driving along " + 
                    segVec[ path[prevIndex] ].streetName + " for " +
                    to_string( static_cast<int>(distSum) - static_cast<int>(distSum) % 1 ) + " meters. ";
    }
    else
    {
        distInfo = "Driving along current path for " +
                    to_string( static_cast<int>(distSum) - static_cast<int>(distSum) % 1 ) + " meters. ";
    }
    
    switch (direction) {
        case STRAIGHT:
            instruction += "Go straight ";
            break;
            
        case LEFT:
            instruction += "Turn left ";
            break;
            
        case RIGHT:
            instruction += "Turn right ";
            break;
            
        default:
            cerr << "Not in the right direction" << endl;
            exit(-1);
    }
    
    bool isToward = false;
    
    string nextStreetName = segVec[ path[nextIndex] ].streetName;
    
    if (nextStreetName == "<unknown>")
    {
        isToward = true;
    }
    
    // Try to find the next street having a name that is not <unknown>
    for (size_t index = nextIndex; nextStreetName == "<unknown>" && index < path.size(); ++index)
    {
        nextStreetName = segVec[ path[index] ].streetName;
    }
    
    if (isToward)
        instruction += "towards ";
    else
        instruction += "onto ";
    
    instruction += nextStreetName + ". ";
    
    auto ret = make_pair(distInfo, instruction);
    return ret;
}

unsigned routeInstruction::polarDirection(unsigned curSegIdx, unsigned nextSegIdx) {
    coord startPt;
    coord headPt;
    
    if (segVec[curSegIdx].info.to == segVec[nextSegIdx].info.from ||
        segVec[curSegIdx].info.to == segVec[nextSegIdx].info.to )
    {
        startPt = segVec[curSegIdx].ptsOnStreet.front();
        headPt = segVec[curSegIdx].ptsOnStreet[1];
    }
    else if (segVec[curSegIdx].info.to == segVec[nextSegIdx].info.to ||
             segVec[curSegIdx].info.to == segVec[nextSegIdx].info.from)
    {
        startPt = segVec[curSegIdx].ptsOnStreet.back();
        headPt = segVec[curSegIdx].ptsOnStreet[segVec[curSegIdx].curPts.size() - 2];
    }
    
    float result_y = headPt.second - startPt.second;
    float result_x = headPt.first - startPt.first;
//deciding the direction from conditionals 
    if (result_y > 0 )
    {
        if ( fabs (result_x) < fabs(result_y)/2)
            return NORTH;
        else if (result_x < 0)
            return WEST;
        else 
            return EAST;
            
    }
    else 
        {
        if ( fabs (result_x) < fabs(result_y)/2)
            return SOUTH;
                else if (result_x < 0)
            return WEST;
        else 
            return EAST;
        }
   
  
}
unsigned routeInstruction::findDirection(unsigned curSegIdx, unsigned nextSegIdx) {
    coord startPt;
    coord relayPt;
    coord endPt;
    
    // Assuming two segments connected having their "to" or "from" connected as well
    // Case:
    //      to/from ------- to
    //       |
    //       |
    //      from
    if (segVec[curSegIdx].info.to == segVec[nextSegIdx].info.from)
        relayPt = itsVec[segVec[curSegIdx].info.to].cartesian_p;

    // Case:
    //      to/to ------- from
    //       |
    //       |
    //      from
    else if (segVec[curSegIdx].info.to == segVec[nextSegIdx].info.to)
        relayPt = itsVec[segVec[curSegIdx].info.to].cartesian_p;

    // Case:
    //      from/from ------- to
    //       |
    //       |
    //      to
    else if (segVec[curSegIdx].info.from == segVec[nextSegIdx].info.from)
        relayPt = itsVec[segVec[curSegIdx].info.from].cartesian_p;

    // Case:
    //      from/to ------- from
    //       |
    //       |
    //      to
    else if (segVec[curSegIdx].info.from == segVec[nextSegIdx].info.to)
        relayPt = itsVec[segVec[curSegIdx].info.from].cartesian_p;
    
    else {
        cerr << "Assumption failed, segments to or from are not directly connected, exiting" << endl;
        exit(-1);
    }
    
    // Determining starting point
    if (segVec[curSegIdx].ptsOnStreet.front() == relayPt)
        startPt = segVec[curSegIdx].ptsOnStreet[1];
    else
        startPt = segVec[curSegIdx].ptsOnStreet[ segVec[curSegIdx].ptsOnStreet.size() - 2];

    // Determining ending point
    if (segVec[nextSegIdx].ptsOnStreet.front() == relayPt)
        endPt = segVec[nextSegIdx].ptsOnStreet[1];
    else
        endPt = segVec[nextSegIdx].ptsOnStreet[ segVec[nextSegIdx].ptsOnStreet.size() - 2];
    
    // constructing equation model
    // slope
    double m = (endPt.second - startPt.second)/(endPt.first - startPt.first);
    // constant shift
    double b = endPt.second - m*endPt.first;
    
    equ.update(m, b);
    
    double result = equ.y(relayPt.first);
    
    if (equ.slopeIsNegative() == -1 || equ.slopeIsNegative() == 0)
    {
        if (relayPt.second > result)
            return RIGHT;
        else if (relayPt.second < result)
            return LEFT;
        else
            return STRAIGHT;
    }
    else if (equ.slopeIsNegative() == 1)
    {
        if (relayPt.second < result)
            return RIGHT;
        else if (relayPt.second > result)
            return LEFT;
        else
            return STRAIGHT;
    }
    
    return STRAIGHT;
}

unsigned routeInstruction::getNumOfInstructions() {
    return instructionCount;
}


pair< pair<StreetSegmentIndex, StreetSegmentIndex>, pair<string, string> > routeInstruction::getOneInstructPair(unsigned index) {
    if (index >= instructionCount)
    {
        cerr << "Instruction out of range" << endl;
        exit(-1);
    }
    return instructionStorage[index];
}

