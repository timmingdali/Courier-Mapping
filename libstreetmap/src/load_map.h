/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   load_map.h
 * Author: liuhao22
 *
 * Created on January 25, 2017, 2:37 PM
 */

#ifndef LOAD_MAP_H
#define LOAD_MAP_H

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <array>
#include <vector>
#include <set>
#include <utility>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "RTREE.h"
#include "OSMDatabaseAPI.h"

#include "StreetGraphEdges.h"
#include <limits>

#define VISIBLE_WORLD_TOLERANCE 0.01
/*custom data structures are defined as structs below and will be initialized in to different global variables
these structures allow linkage between different data members, which does not exit in layer one API*/

const std::vector<std::string> stLvLookUp = {"other", "residential", "tertiary", "secondary", "primary", "trunk", "motorway"};

bool ascendingSort(GraphEdge & lhs, GraphEdge & rhs);

// StreetSet is custom structure used to link street to its own street segments 
// along with vector containing IDs of duplicated name street as well as id of 
// intersection though the street, also some other attributes

typedef struct {
    std::string streetName;
    StreetIndex StreetIdx;
    std::vector<StreetSegmentIndex> StreetSegVec;
    bool linked = false;
    bool initialized = false;
    // Link to address of a duplicated name street 
    std::vector<unsigned> duplicated_street;
    std::set<unsigned> intersectVec;
    unsigned levelNum;
    std::string level;
    std::string ref;

} StreetSet;


// Been placed ahead for the usage of "StreetSegClt"
double calc_length(unsigned street_segment_id);

// Been placed ahead for the usage of "IntersectionClt"
coord latlon_into_cartesian(LatLon latlon);



// StreetSegClt stores some basic attributes of a street segment, specially when
// a object is initialized, the constructor reads all its corresponding LatLon and
// stores in a vector

struct StreetSegClt {
    bool restricted;
    StreetSegmentInfo info;
    StreetSegmentIndex idx;
    std::vector<LatLon> curPts;
    double segLength;
    std::vector <coord> ptsOnStreet;
    std::string streetName;
    unsigned levelNum;
    std::string level;
    std::string ref;
    bool drawn = false;

    // Constructor

    StreetSegClt() {
    };

    StreetSegClt(StreetSegmentInfo theInfo, unsigned index) :
    restricted(false), info(theInfo), idx(index), segLength(calc_length(index)),
    streetName(getStreetName(theInfo.streetID)) {
        ptsOnStreet.push_back(latlon_into_cartesian(getIntersectionPosition(info.from)));
        for (size_t i = 0; i < info.curvePointCount; i++) {
            ptsOnStreet.push_back(latlon_into_cartesian(getStreetSegmentCurvePoint(idx, i)));
            curPts.push_back(getStreetSegmentCurvePoint(idx, i));
        }

        ptsOnStreet.push_back(latlon_into_cartesian(getIntersectionPosition(info.to)));
    };

};

// IntersectionSet specially stores IDs of all street segments' id associated 
//with it

struct IntersectionSet {
    bool isLoaded;
    unsigned segCount;
    IntersectionIndex itsId;
    LatLon location;
    int predecessor_intersection;
    int connecting_street_segment;
    int connecting_street;
    double travel_time_from_start;
    double approx_distance_to_end_vertex;
    double total_heuristics_travel_time;



    //Currently only the index number of street segment is recorded
    std::vector <unsigned > segPairVec;

    //This vector stores GraphEdges that this current intersection points to with
    std::vector <GraphEdge> street_segment_edge;

    coord cartesian_p;
    // Constructors

    IntersectionSet() :
    isLoaded(false), segCount(0), itsId(0) {
    };

    /*Total heuristics travel time, approx distance to end vertex, travel time from start, connecting street, connecting street segments, and 
     *predecessor intersection are all elements of data for the shortest path search in m3. Those that are set to -1 in the beginning because -1
     * is a safe value in which there is no negative index number. 
     * 
     * A vector of street segment is constructed for every intersection set, with the order of the vector arranged in an ascending order of the 
     * street segments traveling time to further facilitate the intersection search function.
     */
    IntersectionSet(IntersectionIndex intersectId, unsigned numOfSeg) :
    isLoaded(true), segCount(numOfSeg), itsId(intersectId), location(getIntersectionPosition(intersectId))
    , predecessor_intersection(-1), connecting_street_segment(-1), connecting_street(-1), travel_time_from_start(std::numeric_limits<double>::max()),
    approx_distance_to_end_vertex(0), total_heuristics_travel_time(std::numeric_limits<double>::max()) {
        for (size_t index = 0; index < segCount; index++) {
            StreetSegmentIndex currSegId = getIntersectionStreetSegment(itsId, index);
            segPairVec.push_back(currSegId);
            StreetIndex streetId = getStreetSegmentInfo(currSegId).streetID;

            if ((intersectId == getStreetSegmentInfo(currSegId).from)) {


                GraphEdge graphEdge(getStreetSegmentInfo(currSegId).to, currSegId, streetId);


                street_segment_edge.push_back(graphEdge);

            }
            if ((intersectId == getStreetSegmentInfo(currSegId).to && getStreetSegmentInfo(currSegId).oneWay == false)) {

                GraphEdge graphEdge(getStreetSegmentInfo(currSegId).from, currSegId, streetId);


                street_segment_edge.push_back(graphEdge);


            }


        }

        std::sort(street_segment_edge.begin(), street_segment_edge.end(), ascendingSort);

        cartesian_p = latlon_into_cartesian(location);
    };
    //The overload assignment operator is constructed below for copying the intersection set data into priority queue.
    /*
    IntersectionSet & operator=(const IntersectionSet & rhs) {
        isLoaded = rhs.isLoaded;
        segCount = rhs.segCount;
        itsId = rhs.itsId;

        predecessor_intersection = rhs.predecessor_intersection;
        travel_time_from_start = rhs.travel_time_from_start;
        total_heuristics_travel_time = rhs.total_heuristics_travel_time;
        approx_distance_to_end_vertex = rhs.approx_distance_to_end_vertex;

        location = rhs.location;

        //Currently only the index number of street segment is recorded
        segPairVec = rhs.segPairVec;

        //This vector stores GraphEdges that this current intersection points to with
        street_segment_edge = rhs.street_segment_edge;

        cartesian_p = rhs.cartesian_p;

        

        return *this;
    }
     */
};


//POISets is a custom structure that combines all information related to a 
//Point of Interest

struct POISet {
    POIIndex index;
    std::string type;
    std::string name;
    LatLon latlon_p;
    OSMID POI_OSM;
    coord cartesian_p;
    bool draw = false;
    IntersectionIndex closestIntersect;
    coord closest_int_position;

    //constructors

    POISet() {
    };

    POISet(POIIndex poiIdx) : index(poiIdx) {
        type = getPointOfInterestType(poiIdx);
        name = getPointOfInterestName(poiIdx);
        latlon_p = getPointOfInterestPosition(poiIdx);
        POI_OSM = getPointOfInterestOSMNodeID(poiIdx);
        cartesian_p = latlon_into_cartesian(getPointOfInterestPosition(poiIdx));

        std::vector<value> result = closestIntersec2(cartesian_p);
       
        double closest_int_d = find_distance_between_two_points(latlon_p,
                                                      getIntersectionPosition(result[0].second));
        unsigned closest_id = result[0].second;
       
        for (unsigned i = 1; i < 10; i++) {
            double cur_d = find_distance_between_two_points(latlon_p,
                                                            getIntersectionPosition(result[i].second) );
      
            if (closest_int_d > cur_d) {
                closest_int_d = cur_d;
                closest_id =result[i].second;
            }
        }
        closestIntersect = closest_id;
        closest_int_position = latlon_into_cartesian(getIntersectionPosition(closest_id));

    }

};

struct FeatureSet {
    FeatureIndex index;
    std::string name;
    FeatureType type;
    TypedOSMID featureOSM;
    unsigned pointCount;
    std::vector <coord> pts_on_feature;
    bool highlight;
    double featureAera;

    //constructors 

    FeatureSet() {
    };

    FeatureSet(FeatureIndex idx) : index(idx) {
        name = getFeatureName(idx);
        type = getFeatureType(idx);
        featureOSM = getFeatureOSMID(idx);
        pointCount = getFeaturePointCount(idx);

        for (unsigned i = 0; i < pointCount; i++) {
            pts_on_feature.push_back(latlon_into_cartesian(getFeaturePoint(idx, i)));
        }
        double area = 0;
        for (auto iter = pts_on_feature.begin(); (iter + 1) != pts_on_feature.end(); iter++) {
            area += (iter->first * (iter + 1)->second + iter->second * (iter + 1)->first);
        }
        featureAera = 0.5 * fabs(area);
    };
};

/********************************* GLOBALS ************************************/
// Structure that group up all data structure types

struct globalStruct {
    // An unordered map that has a string as “key”, and StreetSet* as it “value”. 
    // This structure allows program to find address to StreetSet in streetCollectPtr 
    // through street name.
    std::unordered_map<std::string, StreetSet*> * nameTablePtr;

    // A vector with equivalent size to total number of streets storing structure 
    // StreetSet. By simply indexing *streetCollectPtr using a street ID, program 
    // can access to the corresponding StreetSet in O(1) complexity.
    std::vector<StreetSet> *streetCollectPtr;

    // A vector with equivalent size to total number of street segments storing 
    // structure StreetSegClt. Again, by simply indexing *streetSegCollectPtr using 
    // a street segment ID, program can access to the corresponding StreetSegClt in 
    // O(1) complexity as above.
    std::vector<StreetSegClt> *streetSegCollectPtr;

    //a vector of all custom "IntersectionSet" structs,
    std::vector<IntersectionSet> *intersectionCollectPtr;

    //a vector of all custom "POISet" structs,
    std::vector<POISet> *POICollectPtr;

    //a vector of all custom "FeatureSet" structs,
    std::list <FeatureSet> *FeatureCollecPtr;

    // A vector with equivalent size to total number of street segments storing 
    // “travel time” for each street segment. For example, to know time travel in 
    // one street segment, by indexing *streetSegTravelTimePtr with the segment ID 
    // will give back the result. In other words, this vector is a add-up to 
    // *streetSegCollectPtr. The vector was created to fasten accessing time to “travel time”.
    std::vector<double> * streetSegTravelTimePtr;

    std::unordered_map < OSMID, std::string>* OSMIDTablePtr;

    std::vector< std::pair<unsigned, StreetSegmentIndex> > segLvSortedLookUp;

    //a hash table to store index names of the poi
    std::unordered_map < std::string, std::vector <unsigned>> *POInameTable;

};

/******************************************************************************/

double find_cartesian_distance_squared(coord p1, coord p2);

void databaseInit(unsigned numberOfStreet, unsigned numOfSegments, unsigned numOfIntersection,
        unsigned numOfPOI);

//cleans up the dynamic allocation
void databaseDestr();

// This function link keys of the map to StreetSet by adding pointer to the set
void linkToTable(unsigned streetId);

//calculates the travel time in the streetSegTravelTimePtr
void calTimeForTimeVec(unsigned strSegId);

//as street segments are looped through in load_map, this function pushed the 
//segments in the global vector streetCollectPtr
void pushStSegToStSet(StreetSegmentInfo & streetSeg, unsigned strSegId);

// Check if a street segment is the segment of a street
bool isSubStreetSegment(unsigned streetId, unsigned streetSegId);

// Returns vector containing all street with that name, if not found, returns an
// empty vector
std::vector<unsigned> searchStreetId(std::string streetName);

// Returns pointer pointing to "nameTable"
std::unordered_map<std::string, StreetSet*> *getPtrToTable();

// Returns pointer pointing to database of streetCollectPtr
std::vector<StreetSet>* getPtrToStSet();

// Returns pointer pointing to database of street segment
std::vector<StreetSegClt>* getPtrToStSegSet();

// Returns pointer pointing to database of street sets
std::vector<IntersectionSet>* getPtrToIntersectSet();

//returns pointer pointing to database of POIs

std::vector <POISet>* getPtrToPOISet();

std::list <FeatureSet>* getPtrToFeatureSet();

std::vector< std::pair<unsigned, StreetSegmentIndex> >* getPtrToLookUpTable();

// Returns pointer pointing to database of intersections
std::vector<double>* getPtrToTravelTime();

std::unordered_map <OSMID, std::string>* getPtrtoOSMTable();

std::unordered_map < std::string, std::vector<unsigned>> *getPtrtoPOItable();

bool featureCom (FeatureSet & set1, FeatureSet & set2);

void initialize_segLvSortedLookUp();

bool segLvCom(std::pair<unsigned, StreetSegmentIndex> set1, std::pair<unsigned, StreetSegmentIndex> set2);

double coordinate_distance(const IntersectionSet & current_vertex, const IntersectionSet & end_vertex);


#endif /* LOAD_MAP_H */

