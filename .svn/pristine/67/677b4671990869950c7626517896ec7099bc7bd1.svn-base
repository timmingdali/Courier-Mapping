/*
 * Refer to Fuzzy_Search_Lib.h for general explanations about the usage of this file
 */

#include "Fuzzy_Search_Lib.h"
#include <boost/algorithm/string.hpp>
#define streetVec (*getPtrToStSet())

using namespace std;

void searchEngineInitializer(){
    
    // Clear out the street name collect
    streetNameCol.clear();
    POINameCol.clear();
    POI_lookUp.clear();
    
    // Initialize street, intersection searching database
    for (auto iter = (*getPtrToTable()).begin(); iter != (*getPtrToTable()).end(); iter++ )
    {   
        string streetName = boost::to_upper_copy(iter->first);
        
        stringstream temp(streetName);
        
        string slice;
        
        vector<string> temp_vec;
        
        while (temp >> slice)
            temp_vec.push_back(slice);
        
        pair<string, vector<string> > temp_pair(streetName, temp_vec);
        
        streetNameCol.push_back(temp_pair);
    }
    
    sort(streetNameCol.begin(), streetNameCol.end(), compare);
    
    // Initialize POI searching database
    for (auto eachPOI: (*getPtrToPOISet())) {
        
        vector<POI_search> pairVector;
        
        auto inserted = POI_lookUp.emplace( make_pair(eachPOI.name , pairVector));

        // If insertion takes place, the iterator is at the newly inserted element,
        // therefor push the POI_search to its value vector, and then add an slice
        // to POINameClt
        if (inserted.second)
        {
            inserted.first->second.push_back(POI_search(eachPOI.cartesian_p, eachPOI.index));
            
            string POIName = boost::to_upper_copy(eachPOI.name);
        
            stringstream temp(POIName);

            string slice;

            vector<string> temp_vec;

            while (temp >> slice)
                temp_vec.push_back(slice);

            pair<string, vector<string> > temp_pair(POIName, temp_vec);

            POINameCol.push_back(temp_pair);
        }
        // If insertion doesn't take place, the iterator is at the already existed
        // element, push the POI_search to it as well
        else
        {
            inserted.first->second.push_back(POI_search(eachPOI.cartesian_p, eachPOI.index));
        }
    }
    
    sort(POINameCol.begin(), POINameCol.end(), compare);
}

bool compare(std::pair<std::string, std::vector<std::string> > former, std::pair<std::string, std::vector<std::string> > latter){
    
    unsigned min;
    
    if (former.second.size() < latter.second.size())
        min = former.second.size();
    else
        min = latter.second.size();
    
    for (unsigned index = 0; index < min; index++)
    {
        if (former.second[index] < latter.second[index])
            return true;
        else if (former.second[index] > latter.second [index])
            return false;
    }
    
    if (min == former.second.size())
        return true;
    else
        return false;
}

vector< pair<string, vector<string> > >*  getPtrToSearchLib(){
    return &streetNameCol;
}

vector< pair<string, vector<string> > >* getPtrToSearchLibPOI(){
    return &POINameCol;
}

unordered_map< string, vector<POI_search> >* getPtrToSearchLibPOI_map() {
    return &POI_lookUp;
}


