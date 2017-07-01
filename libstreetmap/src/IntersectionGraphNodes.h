/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IntersectionGraphNodes.h
 * Author: lixin39
 *
 * Created on March 10, 2017, 10:15 PM
 */

#ifndef INTERSECTIONGRAPHNODES_H
#define INTERSECTIONGRAPHNODES_H
#include "StreetGraphEdges.h"
#include "StreetsDatabaseAPI.h"
class GraphEdge;
class GraphNode {
    
private:
    int predecessor_intersection;
    int connecting_street_segment;
    double trave_time_from_start;
    int connecting_street;
    std::vector <GraphEdge > inEdge;
    std::vector <GraphEdge > outEdge;

    
public:
    GraphNode ();
    GraphNode (IntersectionIndex itsId);
    
};



#endif /* INTERSECTIONGRAPHNODES_H */

