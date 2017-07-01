/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "StreetGraphEdges.h"
#include <functional>
#include<bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f

struct segment_Length_ID {
    double length;
    StreetSegmentIndex Id;

    segment_Length_ID() {
    };

    segment_Length_ID(double l, StreetSegmentIndex idx) : length(l), Id(idx) {
    };
};

GraphEdge :: GraphEdge () {}

GraphEdge :: GraphEdge (unsigned in_intersection_index, StreetSegmentIndex in_segmentId, StreetIndex in_streetId){
    
    streetId = in_streetId;
    segmentId = in_segmentId;
    intersection_index = in_intersection_index;
    streetId=in_streetId;
    travel_time = 0;
    
  
} 
