/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fuzzyLib.h
 * Author: liuhao22
 *
 * Created on February 21, 2017, 5:38 PM
 */

#ifndef FUZZYLIB_H
#define FUZZYLIB_H

#define LEVENSHTEIN_DISTANCE_LIMIT 1
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include "load_map.h"
#include "Fuzzy_Search_Lib.h"
#include "Help_Print_Funcs.h"

class FuzzySearch {
public:
    FuzzySearch(std::string input);
    
    virtual ~FuzzySearch();
    
    void update(std::string input);
    
    std::vector<std::string> getCandidates();
    
    size_t getCurrentWordIndex();
    
private:
    // Members
    std::vector<std::string> candidates;
    
    std::vector<std::string> currentInput;
    
    std::vector<std::pair<size_t, size_t> > rangePairClt;
    
    std::string savedInput;
    
    size_t curWordIndex;
    
    int LevenshteinDistance(std::string str1, std::string str2);
    
    void candidateFilter();
    
    void updateCurInput(std::string input);
    
    std::vector< std::pair<size_t, size_t> > fuzzyMatching(size_t order);
};

#endif /* FUZZYLIB_H */

