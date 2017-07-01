//provided/STL
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <list>
#include <algorithm>
#include <set>
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "FuzzySearch.h"
//added header files
#include "load_map.h"
#include "RTREE.h"
#include <ctime>
#define MAP_SIZE_REFERENCE 0.43998

//Imported Library Boost to use Rtree data structure for spatial sorting
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <utility>


//the maximum number of possible nearest node candidates to check
//when returned from the

#define segLvlookUp (*getPtrToLookUpTable())

using namespace std;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
bool isTree;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef std::pair<point, unsigned> value;

bool featureCom(FeatureSet & set1, FeatureSet & set2) {
    return set1.featureAera < set2.featureAera;
}

bool load_map(string map_name) {
    unsigned j;
    for (j = 0; j < map_name.size(); j++) {
        if (map_name[j] == '.')
            break;
    }
    map_name = map_name.substr(0, j);
    cout << map_name << endl;
    string osm_name = map_name;
    map_name.append(".streets.bin");
    osm_name.append(".osm.bin");

    if (loadStreetsDatabaseBIN(map_name) && loadOSMDatabaseBIN(osm_name)) {

        //setting max lat/long as the maximum possible number
        double lat_min = 90;
        double lat_max = -90;
        double lon_min = 180;
        double lon_max = -180;
        unsigned lat_min_index = 0, lat_max_index = 0, lon_min_index = 0, lon_max_index = 0;

        //boundary for the for loops
        unsigned numOfStreet = getNumberOfStreets();

        unsigned numOfSeg = getNumberOfStreetSegments();

        unsigned numOfIntersection = getNumberOfIntersections();

        unsigned numOfPOI = getNumberOfPointsOfInterest();

        databaseInit(numOfStreet, numOfSeg, numOfIntersection, numOfPOI);

        //dynamic allocation to the global variables
        vector<StreetSegClt> * segVec = getPtrToStSegSet();


        //iteration through all street segments
        for (size_t segIdx = 0; segIdx < numOfSeg; segIdx++) {

            // Initializing on member of segVec
            (*segVec)[segIdx] = StreetSegClt(getStreetSegmentInfo(segIdx), segIdx);

            // Initializing the vector of travel time corresponding to segment ID
            calTimeForTimeVec(segIdx);

            //push segments to street sets
            pushStSegToStSet((*segVec)[segIdx].info, segIdx);

            // For reuse
            unsigned streetID = (*segVec)[segIdx].info.streetID;

            //inserts the names to the associated streetID to the unordered map
            linkToTable(streetID);

        }

        //each iteration of the intersection does two things:
        //1. push information into custom data structure IntersectSet
        //2. insert spatial information to the Rtree data structure
        for (unsigned intersectIndex = 0; intersectIndex < getNumberOfIntersections(); intersectIndex++) {

            if (lat_min > getIntersectionPosition(intersectIndex).lat()) {
                lat_min = getIntersectionPosition(intersectIndex).lat();
                lat_min_index = intersectIndex;
            } else if (lat_max < getIntersectionPosition(intersectIndex).lat()) {
                lat_max = getIntersectionPosition(intersectIndex).lat();
                lat_max_index = intersectIndex;
            } else if (lon_min > getIntersectionPosition(intersectIndex).lon()) {
                lon_min = getIntersectionPosition(intersectIndex).lon();
                lon_min_index = intersectIndex;
            } else if (lon_max < getIntersectionPosition(intersectIndex).lon()) {
                lon_max = getIntersectionPosition(intersectIndex).lon();
                lon_max_index = intersectIndex;
            }

            insertToRTree(intersectIndex);

            auto innerSegCount = getIntersectionStreetSegmentCount(intersectIndex);

            (*getPtrToIntersectSet())[intersectIndex] = IntersectionSet(intersectIndex, innerSegCount);



            for (unsigned i = 0; i < (*getPtrToIntersectSet())[intersectIndex].street_segment_edge.size(); i++) {

                unsigned strSegId = (*getPtrToIntersectSet())[intersectIndex].street_segment_edge[i].segmentId;
                (*getPtrToIntersectSet())[intersectIndex].street_segment_edge[i].travel_time = (*getPtrToTravelTime())[strSegId];


            }

            insertToRTree(intersectIndex);
        }


        (*getPtrToIntersectSet())[getNumberOfIntersections()] = IntersectionSet(lat_min_index, 0);
        (*getPtrToIntersectSet())[getNumberOfIntersections() + 1] = IntersectionSet(lat_max_index, 0);
        (*getPtrToIntersectSet())[getNumberOfIntersections() + 2] = IntersectionSet(lon_min_index, 0); //lon min
        (*getPtrToIntersectSet())[getNumberOfIntersections() + 3] = IntersectionSet(lon_max_index, 0); //lon max


        //inserts spatial information of POIs to the POI rtree
        for (unsigned poiIndex = 0; poiIndex < getNumberOfPointsOfInterest(); poiIndex++) {
            string poiName = getPointOfInterestName(poiIndex);
            POISet curPoi = POISet(poiIndex);
            (*getPtrToPOISet())[poiIndex] = curPoi;

            insertToPOITree(poiIndex);
            unordered_map <string, vector< unsigned > >* table = getPtrtoPOItable();
            //insert depending on if table exists 
            if (table->find(poiName) == table->end()) {
                vector< unsigned> firstEle(1, poiIndex);
                table->insert(make_pair(poiName, firstEle));
                // (*table)[poiName].push_back(curPoi);
            } else
                (*table)[poiName].push_back(poiIndex);

        }


        ////////////////////
        //int stop_s2=clock();
        ////////////////////

        //cout << "poi loading time: " << (stop_s2-start_s2)/double(CLOCKS_PER_SEC)*1000 << " ms" << endl;

        ////////////////////
        //int start_s=clock();
        ////////////////////

        list < FeatureSet > unknown;
        list < FeatureSet > park;
        list < FeatureSet > beach;
        list < FeatureSet > lake;
        list < FeatureSet > river;
        list < FeatureSet > island;
        list < FeatureSet > shoreline;
        list < FeatureSet > building;
        list < FeatureSet > greenspace;
        list < FeatureSet > golfcourse;
        list < FeatureSet > stream;

        for (unsigned featureIdx = 0; featureIdx < getNumberOfFeatures(); featureIdx++) {
            if (map_name == "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin" && featureIdx == 57966)
                continue;

            switch (getFeatureType(featureIdx)) {

                case Park:
                    park.push_back(FeatureSet(featureIdx));
                    break;
                case Beach:
                    beach.push_back(FeatureSet(featureIdx));
                    break;
                case Lake:
                    lake.push_back(FeatureSet(featureIdx));
                    break;
                case River:
                    river.push_back(FeatureSet(featureIdx));
                    break;
                case Island:
                    island.push_back(FeatureSet(featureIdx));
                    break;

                case Shoreline:
                    shoreline.push_back(FeatureSet(featureIdx));
                    break;

                case Building:
                    building.push_back(FeatureSet(featureIdx));
                    break;

                case Greenspace:
                    greenspace.push_back(FeatureSet(featureIdx));
                    break;

                case Golfcourse:
                    golfcourse.push_back(FeatureSet(featureIdx));
                    break;

                case Stream:
                    stream.push_back(FeatureSet(featureIdx));
                    break;
                default:
                    unknown.push_back(FeatureSet(featureIdx));

            }

        }
        auto ptr = getPtrToFeatureSet();
        list <FeatureSet> temp_lake(lake);

        (*ptr).splice((*ptr).end(), unknown);
        (*ptr).splice((*ptr).end(), shoreline);
        (*ptr).splice((*ptr).end(), greenspace);
        (*ptr).splice((*ptr).end(), lake);
        (*ptr).splice((*ptr).end(), island);
        (*ptr).splice((*ptr).end(), beach);

        (*ptr).splice((*ptr).end(), golfcourse);
        (*ptr).splice((*ptr).end(), park);
        (*ptr).splice((*ptr).end(), river);
        (*ptr).splice((*ptr).end(), stream);
        (*ptr).splice((*ptr).end(), building);


        double ymax = (*getPtrToIntersectSet())[getNumberOfIntersections() + 1].location.lat();
        double ymin = (*getPtrToIntersectSet())[getNumberOfIntersections() ].location.lat();
        //maps that are reasonably big, taking Toronto as a a reference
        if (ymax - ymin >= MAP_SIZE_REFERENCE-0.1) {
            for (auto iter = temp_lake.begin(); iter != temp_lake.end(); iter++) {

                if (iter->featureAera < 17502100 * (ymax - ymin) / MAP_SIZE_REFERENCE) {
                    (*ptr).push_back(*iter);

                }
            }
        }
        //        vector< FeatureSet> v { std::make_move_iterator(std::begin((*ptr))), 
        //                  std::make_move_iterator(std::end((*ptr))) };
        //        
        //                  sort (v.begin(), v.end(), featureCom);
        //                  ptr->clear();
        //                  std::copy( v.begin(), v.end(), std::back_inserter( (*ptr) ) );

        ////////////////////
        //int stop_s=clock();
        ////////////////////

        //cout << "feature loading time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << " ms" << endl;

        ////////////////////
        //int start_s1=clock();
        ////////////////////

        string streetName, level, previous_name;
        rtree(map_name);
        for (unsigned i = 0; i < getNumberOfWays(); i++) {

            vector <unsigned> seg_ids, street_ids;
            const OSMWay* e = getWayByIndex(i);
            //loops through tags
            for (unsigned keys = 0; keys < getTagCount(e); ++keys) {
                string key, OSMvalue;
                key = getTagPair(e, keys).first;
                OSMvalue = getTagPair(e, keys).second;

                if (OSMvalue == previous_name) {
                    break;
                }
                //found name tag
                if (key == "name") {

                    //name is the name associated with this id
                    streetName = OSMvalue;

                    //if can't address the name from the osm data
                    if ((*getPtrToTable()).find(OSMvalue) == (*getPtrToTable()).end())
                        continue;

                    //get the ids of the same street names
                    street_ids = (*getPtrToTable())[streetName]->duplicated_street;

                } else if (key == "highway") {
                    level = OSMvalue;
                    for (auto iter = street_ids.begin(); iter != street_ids.end(); iter++) {

                        (*getPtrToStSet())[*iter].level = OSMvalue;
                        auto find_result = find(stLvLookUp.begin(), stLvLookUp.end(), OSMvalue);

                        if (find_result == stLvLookUp.end())
                            (*getPtrToStSet())[*iter].levelNum = 0;

                        else
                            (*getPtrToStSet())[*iter].levelNum = find_result - stLvLookUp.begin();
                        //       if ((*getPtrToStSet())[*iter].levelNum ==6)
                        (*getPtrToStSet())[*iter].levelNum;
                    }

                }
                else if (key == "ref") {
                    for (auto iter = street_ids.begin(); iter != street_ids.end(); iter++) {

                        (*getPtrToStSet())[*iter].ref = OSMvalue;

                    }
                }

            }
            previous_name = streetName;

        }
        initialize_segLvSortedLookUp();

        sort(segLvlookUp.begin(), segLvlookUp.end(), segLvCom);

        ////////////////////
        //int stop_s1=clock();
        ////////////////////

        //cout << "osm loading time: " << (stop_s1-start_s1)/double(CLOCKS_PER_SEC)*1000 << " ms" << endl;

        return true;
    } else
        return false;
}

void close_map() {
    //Clean-up your map related data structures here
    databaseDestr();

    closeStreetDatabase();
    closeOSMDatabase();

}

//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.

vector<unsigned> find_street_ids_from_name(string street_name) {
    auto retVec = searchStreetId(street_name);
    return retVec;
    //O(1)
}

//Returns the street segments for the given intersection

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {
    auto result = (*getPtrToIntersectSet())[intersection_id].segPairVec;
    return result;
    //O(1)
}

//Returns the street names at the given intersection (includes duplicate street names in returned vector)

std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
    //Returns the street names at the given intersection (includes duplicate street names in returned vector)

    std::vector<unsigned> temp = (*getPtrToIntersectSet())[intersection_id].segPairVec;
    //temp is for first storing given intersection's street segment index number

    std::vector<std::string> intersectionStreetNameS;
    //Final vector of strings to be returned

    for (size_t index = 0; index < temp.size(); index++) {

        StreetIndex streetIndeX = getStreetSegmentInfo(temp[index]).streetID;
        //Now temp vector contains all street segments corresponding streets index numbers

        intersectionStreetNameS.push_back((*getPtrToStSet())[streetIndeX].streetName);
        //Pass in the corresponding street names and store them into intersectionStreetNameS vector
        //through using the global pointer to the street sets

    }

    return (intersectionStreetNameS);
}

//Returns true if you can get from intersection1 to intersection2 using a single
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {
    // a intersection is connected to itself
    if (intersection_id1 == intersection_id2)
        return true;

    unsigned intersectSeg = getIntersectionStreetSegmentCount(intersection_id1);

    for (size_t index = 0; index < intersectSeg; index++) {
        StreetSegmentIndex segIdx = getIntersectionStreetSegment(intersection_id1, index);

        StreetSegClt* segCltPtr = &((*getPtrToStSegSet())[segIdx]);

        if (!segCltPtr->info.oneWay) {
            // In this case, no need to check id1 since it must be either "to" or
            // "from", therefore by checking only id2 the program will give back
            // a correct result
            if (segCltPtr->info.from == intersection_id2) {
                return true;
            } else if (segCltPtr->info.to == intersection_id2) {
                return true;
            }
        } else {
            if (segCltPtr->info.to == intersection_id2) {
                return true;
            }
        }
    }
    return false;
}

//Returns all intersections reachable by traveling down one street segment
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections

vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    unsigned segCount = getIntersectionStreetSegmentCount(intersection_id);

    // Using container "set" to prevent duplication
    set<unsigned> retSet; // retSet: returning set

    for (size_t index = 0; index < segCount; index++) {
        StreetSegmentIndex segIdx = getIntersectionStreetSegment(intersection_id, index);

        StreetSegmentInfo segInfo = (*getPtrToStSegSet())[segIdx].info;

        if (!segInfo.oneWay) {
            if (segInfo.from == intersection_id)
                retSet.insert(segInfo.to);
            else
                retSet.insert(segInfo.from);
        } else {
            if (segInfo.from == intersection_id)
                retSet.insert(segInfo.to);
        }
    }

    // Copy data to vector as a return object
    vector<unsigned> retVec(retSet.begin(), retSet.end());

    return retVec;
}

//Returns all street segments for the given street

std::vector<unsigned> find_street_street_segments(unsigned street_id) {
    auto result = (*getPtrToStSet())[street_id].StreetSegVec;
    return result;
}

//Returns all intersections along the a given street

vector<unsigned> find_all_street_intersections(unsigned street_id) {
    // Using attribute from "StreetSet" as the result
    auto retSet = (*getPtrToStSet())[street_id].intersectVec;

    vector<unsigned> retVec(retSet.begin(), retSet.end());

    return retVec;
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique, so more than 1 intersection id may exist

vector<unsigned> find_intersection_ids_from_street_names(string street_name1, string street_name2) {

    auto streetVec_1 = searchStreetId(street_name1);
    auto streetVec_2 = searchStreetId(street_name2);


    set<unsigned> retSet;
    //loop through all streetIDs with the same name1
    for (auto iter_1 = streetVec_1.begin(); iter_1 != streetVec_1.end(); iter_1++) {
        auto ptrTo1stStreet = &(*getPtrToStSet())[*iter_1];

        set<unsigned> intersectSet_1 = ptrTo1stStreet->intersectVec;

        // Pre-load end iterators to save time
        auto endIterForIntersect_1 = intersectSet_1.end();

        for (auto iter_2 = streetVec_2.begin(); iter_2 != streetVec_2.end(); iter_2++) {
            set<unsigned> intersectSet_2 = (*getPtrToStSet())[*iter_2].intersectVec;
            //within the Street, loop through all intersection in that street
            // Pre-load end iterators to save time
            auto endIterForIntersect_2 = intersectSet_2.end();

            for (auto intersectIter = intersectSet_1.begin(); intersectIter != endIterForIntersect_1; intersectIter++) { //find if the intersection in name1 exits in those in name2
                if (intersectSet_2.find(*intersectIter) != endIterForIntersect_2) {
                    retSet.insert(*intersectIter);
                }
            }
        }
    }
    //convert to the right data type
    vector<unsigned> retVec(retSet.begin(), retSet.end());

    return retVec;
}

// Returns the distance between two coordinates in meters
// used the equation (x,y)= (long * cos (lat_avg), lat) and d=R* |p1-p2|

double find_distance_between_two_points(LatLon point1, LatLon point2) {

    //calculates all lat/lon points in to x-y coordinates
    double xcoorP1 = point1.lon() * cos(((point1.lat() + point2.lat()) / 2) * DEG_TO_RAD);
    double ycoorP1 = point1.lat();
    double xcoorP2 = point2.lon() * cos(((point1.lat() + point2.lat()) / 2) * DEG_TO_RAD);
    double ycoorP2 = point2.lat();

    // d=R* ||p1-p2||
    return (EARTH_RADIUS_IN_METERS * DEG_TO_RAD * sqrt(pow((xcoorP1 - xcoorP2), 2.0) + pow((ycoorP1 - ycoorP2), 2.0)));

}


//Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id) {

    //this length information in included in the global variable streetSegCollectPtr
    //which is a point to custom street segments

    return (*getPtrToStSegSet())[street_segment_id].segLength;
    //time complexity O(1)
}

double find_street_segment_travel_time(unsigned street_segment_id) {

    //the travel time is stored in a vector of doubles, which are indexed by their id's

    return (*getPtrToTravelTime())[street_segment_id];
    //time complexity O(logn)
}

void rtree(string map_name) {
    if (map_name == "/cad2/ece297s/public/maps/iceland.streets.bin")
        isTree = true;
    else isTree = false;
}

double find_street_length(unsigned street_id) {

    //get a vector of all street segment IDs in street "street_id"
    vector<unsigned> * segment_collection = &((*getPtrToStSet())[street_id].StreetSegVec);

    double length = 0;

    //iterator that traverses all segments
    for (auto seg_it = (*segment_collection).begin(); seg_it != (*segment_collection).end(); seg_it++) {
        length += find_street_segment_length(*seg_it);
    }

    return length;
    //time complexity O(number of segments)
}

//Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position) {
    //closest POI returns "NUM_CANDIDATES" number of possible nodes that are closest

    coord my_position_xy = latlon_into_cartesian(my_position);

    std::vector<value> result = closestPOI(my_position_xy);


    return result[0].second;

}

//Returns the the nearest intersection to the given position
//uses the same algorithm in find_closest_POI, same time complexity as well

unsigned find_closest_intersection(LatLon my_position) {


    coord my_position_xy = latlon_into_cartesian(my_position);

    std::vector<value> result = closestIntersec(my_position_xy);

    return result[0].second;


}

