/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "load_map.h"
#include "Scale_dict.h"
using namespace std;

static globalStruct global;


// Call this function to initialize database
void databaseInit(unsigned numberOfStreet, unsigned numOfSegments, unsigned numOfIntersection,
                    unsigned numOfPOI){
    
    global.nameTablePtr = new unordered_map<std::string, StreetSet*> ;
    
    global.streetCollectPtr = new vector<StreetSet>(numberOfStreet);

    global.streetSegCollectPtr = new vector<StreetSegClt>(numOfSegments);

    //Two extra intersection sets created for min and max 
    global.intersectionCollectPtr = new vector<IntersectionSet>(numOfIntersection + 4);
    
    global.POICollectPtr = new vector<POISet> (numOfPOI);
    
    global.FeatureCollecPtr = new list <FeatureSet> ;
    
    global.streetSegTravelTimePtr = new vector<double>(numOfSegments);
    
    global.OSMIDTablePtr = new unordered_map <OSMID, string>;
    
    global.POInameTable = new  unordered_map < string,vector <unsigned> > ;
    
    RTreeInit();
    
    POITreeInit();
    
    initialize_scaling();

}

//deletes all dynamically declared global variables
void databaseDestr() {
    delete global.nameTablePtr;
    delete global.streetCollectPtr;
    delete global.streetSegCollectPtr;
    delete global.intersectionCollectPtr;
    delete global.streetSegTravelTimePtr;
    delete global.POICollectPtr;
    delete global.FeatureCollecPtr;
    delete global.OSMIDTablePtr;
    delete global.POInameTable;
    global.segLvSortedLookUp.clear();
    
    RTreeDestc();
    POITreeDestc();
    close_Scale_dict();
}

// Calculates the travel time for each street segment
void calTimeForTimeVec(unsigned strSegId) {

    double segLength = (*global.streetSegCollectPtr)[strSegId].segLength;
    double speedLimit = ((*global.streetSegCollectPtr)[strSegId].info).speedLimit;

    (*global.streetSegTravelTimePtr)[strSegId] = 3.6 * segLength / speedLimit;
}

// To store all streets in to the "nameTable", using street name as the key, address
// of the "StreetSet" as their value
void linkToTable(unsigned streetIdx) {

    // If the StreetSet has already been linked, do nothing
    if ((*global.streetCollectPtr)[streetIdx].linked)
        return;
    
    // Get name from the global vector which stores "StreetSet"s
    string streetName = (*global.streetCollectPtr)[streetIdx].streetName;
    
    // Using "find" method to fist determine whether or not the street has been 
    // linked already
    unordered_map<std::string, StreetSet*>::iterator found = global.nameTablePtr->find(streetName);
    
    if (found == global.nameTablePtr->end())
    {
        (*global.nameTablePtr)[streetName] = &((*global.streetCollectPtr)[streetIdx]);
        
        // Adding street id to duplicated_street vector, since the vector needs
        // to have at least it self for the convenience of "m1" functions
        // Note, "(*streetCollectPtr)[streetIdx]" is already a existing key in 
        // the table
        (*global.streetCollectPtr)[streetIdx].duplicated_street.push_back(streetIdx);
    }
    else
    {
        auto objAddress = found->second;
        
        objAddress->duplicated_street.push_back(streetIdx);
    }
    (*global.streetCollectPtr)[streetIdx].linked = true;
}

// Push id of street segment to corresponding StreetSet, if the StreetSet has not
// been initialized yet, initializes it as well. Use pointer because only id is stored,
// use id to access street segment info through API function
//          "C" for collection
void pushStSegToStSet(StreetSegmentInfo & streetSeg, unsigned strSegId) {
    int streetId = streetSeg.streetID;

    // Check if the StreetSet has been initialized, in order to minimize time would
    // be wasted on duplicating operations
    if (!(*global.streetCollectPtr)[streetId].initialized) 
    {
        (*global.streetCollectPtr)[streetId].streetName = getStreetName(streetId);
        (*global.streetCollectPtr)[streetId].StreetIdx = streetId;
        (*global.streetCollectPtr)[streetId].initialized = true;
    }
    
    (*global.streetCollectPtr)[streetId].intersectVec.insert(streetSeg.from);
    (*global.streetCollectPtr)[streetId].intersectVec.insert(streetSeg.to);
    (*global.streetCollectPtr)[streetId].StreetSegVec.push_back(strSegId);
}


coord latlon_into_cartesian (LatLon latlon){
    
    return make_pair(latlon.lon() * cos(((getIntersectionPosition(0).lat())) * DEG_TO_RAD),latlon.lat());
     
}

double find_cartesian_distance_squared (coord p1, coord p2){
    
    return ((p1.first-p2.first)*(p1.first-p2.first) + 
            (p1.second - p2.second)*(p1.second - p2.second));
}
// Returns pointer pointing to "nameTable"
unordered_map<string, StreetSet*> *getPtrToTable() {
    return global.nameTablePtr;
}

// Returns pointer pointing to database of streetCollectPtr
vector<StreetSet>* getPtrToStSet() {
    return global.streetCollectPtr;
}

// Returns pointer pointing to database of street segment
vector<StreetSegClt>* getPtrToStSegSet() {
    return global.streetSegCollectPtr;
}

vector<IntersectionSet>* getPtrToIntersectSet(){
    return global.intersectionCollectPtr;
}


std::vector <POISet>* getPtrToPOISet(){
    
    return global.POICollectPtr;
}


std::list <FeatureSet>* getPtrToFeatureSet(){
    
    return global.FeatureCollecPtr;
}

// Returns pointer pointing to database of travel time
vector<double>* getPtrToTravelTime() {
    return global.streetSegTravelTimePtr;
}

unordered_map <OSMID, string>* getPtrtoOSMTable (){
    return global.OSMIDTablePtr;
}


std::vector< std::pair<unsigned, StreetSegmentIndex> >* getPtrToLookUpTable(){
    return &global.segLvSortedLookUp;
}
std::unordered_map <std::string, std::vector< unsigned >>* getPtrtoPOItable (){
    return global.POInameTable;
}

// Returns vector containing all street with that name, if not found, returns an
// empty vector
vector<unsigned> searchStreetId(string streetName) {
    vector<unsigned> result;
    
    // Using "find" method to check if the street exists in the nameTable
    auto rawResult = getPtrToTable()->find(streetName);
   
    if (rawResult != getPtrToTable()->end())
    {
        // rawResult as a iterator, has its "first" attribute storing the 
        // street name, and its "second" storing address to the corresponding
        // "StreetSet"
        result = (rawResult->second)->duplicated_street;
        return result;
    } 
    else 
    {
        return result;
    }
}

// Check if a street segment is the segment of a street
bool isSubStreetSegment(unsigned streetId, unsigned streetSegId) {
    if (getStreetSegmentInfo(streetSegId).streetID == streetId)
        return true;
    else
        return false;
}

//calculates the total length of a street segment in meters
double calc_length(unsigned street_segment_id) {
  
    unsigned i;
    StreetSegmentInfo current_segment = getStreetSegmentInfo(street_segment_id);
    
    //no curve points
    if (current_segment.curvePointCount < 1)

        return find_distance_between_two_points(getIntersectionPosition(current_segment.from),
                                                getIntersectionPosition(current_segment.to));

    else {
        //calculates distance from start to first point
        double total_distance = find_distance_between_two_points(getIntersectionPosition(current_segment.from),
                                                                 getStreetSegmentCurvePoint(street_segment_id, 0));
       
        //sum up connecting distances
        for (i = 0; i < current_segment.curvePointCount - 1; i++) {

            total_distance += find_distance_between_two_points(getStreetSegmentCurvePoint(street_segment_id, i),
                                                               getStreetSegmentCurvePoint(street_segment_id, i + 1));
            }
              
        return total_distance +
                find_distance_between_two_points(getStreetSegmentCurvePoint(street_segment_id, i),
                                                 getIntersectionPosition(current_segment.to));
        }   
    return 0;
}

void initialize_segLvSortedLookUp(){
    for (unsigned index = 0; index < getNumberOfStreets(); index++ )
    {   
        StreetSet curSet= (*getPtrToStSet())[index];
        vector <StreetSegmentIndex> segments = curSet.StreetSegVec;
        
        for (auto iter = segments.begin(); iter != segments.end(); iter++){

            pair<unsigned, StreetSegmentIndex> tempPair(curSet.levelNum, *iter); 
            global.segLvSortedLookUp.push_back(tempPair);
            (*getPtrToStSegSet())[*iter].level = curSet.level;
            (*getPtrToStSegSet())[*iter].levelNum = curSet.levelNum;
            (*getPtrToStSegSet())[*iter].ref = curSet.ref;           

        }

    }
}

bool segLvCom (pair<unsigned, StreetSegmentIndex> set1, pair<unsigned, StreetSegmentIndex> set2){
    return set1.first < set2.first;
}

bool ascendingSort (GraphEdge & lhs, GraphEdge & rhs) {
    return lhs.travel_time < rhs.travel_time;
}

double coordinate_distance (const IntersectionSet & current_vertex, const IntersectionSet & end_vertex) {
    
    double xdiff = (end_vertex.cartesian_p.first - current_vertex.cartesian_p.first)*(end_vertex.cartesian_p.first - current_vertex.cartesian_p.first);
    double ydiff = (end_vertex.cartesian_p.second - current_vertex.cartesian_p.second)*(end_vertex.cartesian_p.second - current_vertex.cartesian_p.second);
    double answer = sqrt(xdiff + ydiff);
    return answer*10;
    
    
}

