/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StreetGraphEdges.h
 * Author: lixin39
 *
 * Created on March 10, 2017, 10:19 PM
 */

#ifndef STREETGRAPHEDGES_H
#define STREETGRAPHEDGES_H
#include "StreetsDatabaseAPI.h"
class GraphEdge {
public:
   
    StreetIndex streetId;
    StreetSegmentIndex segmentId;
    unsigned intersection_index;
    double travel_time;

    
    GraphEdge ();
    GraphEdge (unsigned in_intersection_index, StreetSegmentIndex in_segmentId, StreetIndex in_streetId);

};


#endif /* STREETGRAPHEDGES_H */

