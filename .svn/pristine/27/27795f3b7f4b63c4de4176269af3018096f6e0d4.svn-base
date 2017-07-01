/*
 * This header file incorporates functions that enable fuzzy search within the Milestone 2 
 * graphical user interface:
 * "searchEngineInitializer()" is used to initialize a static vector of street names and their 
 * namess slices: i.e. "Yonge Street" & "Yonge" and "Street"
 * 
 */

/* 
 * File:   Fuzzy_Search.h
 * Author: liuhao22
 *
 * Created on February 21, 2017, 5:18 PM
 */

#ifndef FUZZY_SEARCH_H
#define FUZZY_SEARCH_H

#include "load_map.h"
#include "RTREE.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>

struct POI_search{
    coord POIcoord;
    POIIndex index;
    
    POI_search(coord coord_, POIIndex _index):
    POIcoord(coord_), index(_index) {}
    
};

static std::vector< std::pair<std::string, std::vector<std::string> > > streetNameCol;

static std::vector< std::pair<std::string, std::vector<std::string> > > POINameCol;

static std::unordered_map< std::string, std::vector<POI_search> > POI_lookUp;

bool compare(std::pair<std::string, std::vector<std::string> > former, std::pair<std::string, std::vector<std::string> > latter);

void searchEngineInitializer();

std::vector< std::pair<std::string, std::vector<std::string> > >* getPtrToSearchLib();

std::vector< std::pair<std::string, std::vector<std::string> > >* getPtrToSearchLibPOI();

std::unordered_map< std::string, std::vector<POI_search> >* getPtrToSearchLibPOI_map();


#endif /* FUZZY_SEARCH_H */

