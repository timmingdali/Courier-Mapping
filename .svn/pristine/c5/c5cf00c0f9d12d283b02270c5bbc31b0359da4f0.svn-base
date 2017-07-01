/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuzzySearch_POI.h
 * Author: liuhao22
 *
 * Created on March 16, 2017, 7:12 PM
 */

#ifndef FUZZYSEARCH_POI_H
#define FUZZYSEARCH_POI_H

#include "Fuzzy_Search_Lib.h"
#include <vector>

#define POILib (*getPtrToSearchLibPOI())
#define POIMap (*getPtrToSearchLibPOI_map())
class FuzzySearch_POI {
public:
    
    FuzzySearch_POI(std::string);
    
    void updateCurInput(std::string);
    
    std::vector<std::string> getCandidates();
    
    void update(std::string);
    
private:
    std::pair<size_t, size_t> rangePair;
    
    std::vector<std::string> candidates;
    
    std::vector<std::string> currentInput;
    
    std::string savedInput;
    
    size_t curWordIndex;
    
    void candidateFilter();

};

#endif /* FUZZYSEARCH_POI_H */

