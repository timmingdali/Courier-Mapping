/*
 * This source file contains functions implementations of rtree that is declared in "rtree.h"
 */

#include "RTREE.h"
#include "load_map.h"

//Point and Value are defined for rtree insertion and searching, refer to "rtree.h" for 
//detailed explanations of their usage.
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point;
typedef std::pair<point, unsigned> value;

static rTreeGlobalStruct rTreeGlobal;

void POITreeInit(){
    
    //A rtree that takes in "value" with 2 dimensions in Cartesian system is dynamically called
    //with the first "node" inserted. The "node"'s latitude and longitude are the x and y axis for the
    //Cartesian system.
    unsigned Zero = 0;
    rTreeGlobal.my_POIrtree_ptr = new boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> >;
    
    point firstIntersection = point(getPointOfInterestPosition(Zero).lon(),getPointOfInterestPosition(Zero).lat());
    rTreeGlobal.my_POIrtree_ptr->insert(std::make_pair(firstIntersection,Zero));
    
    
}

void RTreeInit(){
    
    //Same as POITreeInit
    
    unsigned Zero = 0;
    rTreeGlobal.my_rtree_ptr = new boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> >;
    
    point firstIntersection = point(getIntersectionPosition(Zero).lon()*cos(((getIntersectionPosition(0).lat())) * DEG_TO_RAD),getIntersectionPosition(Zero).lat());
    rTreeGlobal.my_rtree_ptr->insert(std::make_pair(firstIntersection,Zero));   
    
}

void insertToRTree(IntersectionIndex intersectIndex){
    
    //my_rtree_ptr calls the insertion function that takes in a point composed of given intersection
    //index's longitude and latitude, pairs the point with its unique intersection index number
    //and completes the insertion.
    coord xy_position = (*(getPtrToIntersectSet()))[intersectIndex].cartesian_p;
   
    point intersection_to_be_inserted = point (xy_position.first, xy_position.second);
    
    rTreeGlobal.my_rtree_ptr->insert(std::make_pair(intersection_to_be_inserted,intersectIndex));
    
}

void insertToPOITree(POIIndex poiIndex){
    
    //my_POIrtree_ptr calls the insertion function that takes in a point composed of given point of interest's
    //longitude and latitude, pairs the point with its unique point of interest index number
    //and completes the insertion.
    
    coord xy_position = (*(getPtrToPOISet()))[poiIndex].cartesian_p;
   
    point intersection_to_be_inserted = point (xy_position.first, xy_position.second);
    
    rTreeGlobal.my_POIrtree_ptr->insert(std::make_pair(intersection_to_be_inserted,poiIndex));
   
    
    
    
}

std::vector<value> closestIntersec (coord my_position_xy){
    
    //A vector with type "value" is declared to take in the queried NUM_CANDIDATES number of closest nodes to the "latlon"
    //and returns this vector.
    std::vector<value> result;
    rTreeGlobal.my_rtree_ptr->query(boost::geometry::index::nearest(point(my_position_xy.first, my_position_xy.second),1), std::back_inserter(result)); 
    return result;
}

std::vector<value> closestPOI (coord my_position_xy){
    
    //Similar to closestIntersec
    std::vector<value> result;
    rTreeGlobal.my_POIrtree_ptr->query(boost::geometry::index::nearest(point(my_position_xy.first, my_position_xy.second),1), std::back_inserter(result)); 
    return result;
}
std::vector<value> closestIntersec2 (coord my_position_xy){
    
    //A vector with type "value" is declared to take in the queried NUM_CANDIDATES number of closest nodes to the "latlon"
    //and returns this vector.
    std::vector<value> result;
    rTreeGlobal.my_rtree_ptr->query(boost::geometry::index::nearest(point(my_position_xy.first, my_position_xy.second),NUM_CANDIDATES), std::back_inserter(result)); 
    return result;
}

std::vector<value> closestPOI2 (coord my_position_xy){
    
    //Similar to closestIntersec
    std::vector<value> result;
    rTreeGlobal.my_POIrtree_ptr->query(boost::geometry::index::nearest(point(my_position_xy.first, my_position_xy.second),NUM_CANDIDATES), std::back_inserter(result)); 
    return result;
}

void RTreeDestc(){
    delete rTreeGlobal.my_rtree_ptr;
}

void POITreeDestc(){
    delete rTreeGlobal.my_POIrtree_ptr;
    
}

double convertLongitude (LatLon latlon){
    
    return latlon.lon() * cos(((getIntersectionPosition(0).lat())) * DEG_TO_RAD);
    
}

boost::geometry::index:: rtree<value, boost::geometry::index::quadratic<16> >* getPtrRTREE(){
    return rTreeGlobal.my_rtree_ptr;
}

boost::geometry::index:: rtree<value, boost::geometry::index::quadratic<16> >* getPtrPOIRTREE(){
    return rTreeGlobal.my_POIrtree_ptr;
}