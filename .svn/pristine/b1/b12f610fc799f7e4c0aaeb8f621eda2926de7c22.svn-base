/*
 * This header file contains the implementation of rtree - data structure for spatial access 
 * method - for two of the functions in m1.cpp: find_closest_intersection and find_closest_point_of_interest.
 * "Boost" C++ libraries are used for the implementation, refer to the directives included for the
 * specific repositories.
 * 
 * For more detailed specific function explanations, please refer to RTREE.cpp
 */

/* 
 * File:   RTREE.h
 * Author: liming24
 *
 * Created on February 3, 2017, 7:59 PM
 */

#ifndef RTREE_H
#define RTREE_H

#include "StreetsDatabaseAPI.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <vector>
#include <cmath>
#include "m1.h"

//NUM_CANDIDATES is defined for the function find_closest_intersection
#define NUM_CANDIDATES 10

//Point is defined and used representing each intersection or point of interest, with the 
//2 dimension Cartesian coordinates corresponding to the latitude and longitude.
//Value is defined as a pair of point and unsigned number; the latter is the unique intersection index number
//or point of interest index number.
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point;
typedef std::pair<point, unsigned> value;

//xy_coordinates of a point converted from latlon in the form of (lon*cos(lat), lat) 
typedef std::pair<double, double> coord;


//rtrees first initialized
void RTreeInit();
void POITreeInit();

//points to be inserted into rtrees
void insertToRTree(IntersectionIndex intersectIndex);
void insertToPOITree(POIIndex poiIndex);
void rtree (std::string treeName);
void RTreeDestc();
void POITreeDestc();

double convertLongitude (LatLon latlon);

std::vector<value> closestIntersec (coord my_position_xy );
std::vector<value> closestPOI (coord my_position_xy);

std::vector<value> closestIntersec2 (coord my_position_xy );
std::vector<value> closestPOI2 (coord my_position_xy);

struct rTreeGlobalStruct{
    //Two static pointers to rtrees are declared for point inserting and querying 
    boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> >* my_rtree_ptr;
    boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> >* my_POIrtree_ptr;
};

//Functions that return the above pointers
boost::geometry::index:: rtree<value, boost::geometry::index::quadratic<16> >* getPtrRTREE();
boost::geometry::index:: rtree<value, boost::geometry::index::quadratic<16> >* getPtrPOIRTREE();




#endif /* RTREE_H */

