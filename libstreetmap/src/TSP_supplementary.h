/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TSP_supplementary.h
 * Author: liming24
 *
 * Created on March 30, 2017, 11:35 PM
 */

#ifndef TSP_SUPPLEMENTARY_H
#define TSP_SUPPLEMENTARY_H

#include "load_map.h"
#include <vector>
#include "StreetGraphEdges.h"
#include <queue>
#include <functional>
#include <limits>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <utility>
#include <unordered_map>


double TSP_quick_travel_time (const std::vector<unsigned>& tentative_delivery_path);

std:: vector<IntersectionSet> TSP_preCompute_find_path_between_intersections(const unsigned intersect_id_start,
         std:: unordered_map <unsigned, bool>&  pre_compute_intersects_reached, const double turn_penalty);


#endif /* TSP_SUPPLEMENTARY_H */

