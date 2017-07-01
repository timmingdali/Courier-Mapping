/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuzzySearch_POI.cpp
 * Author: liuhao22
 * 
 * Created on March 16, 2017, 7:12 PM
 */

#include "FuzzySearch_POI.h"

using namespace std;

FuzzySearch_POI::FuzzySearch_POI(string input):
savedInput(input), curWordIndex(0)
{
    updateCurInput(input);
    
    rangePair = make_pair(0, POILib.size());
    
    candidateFilter();
}

void FuzzySearch_POI::updateCurInput(std::string input){
    
    string word;
    
    boost::to_upper(input);
    
    stringstream inputStream(input);
    
    while (inputStream >> word){
        this->currentInput.push_back(word);
    }
}

vector<string> FuzzySearch_POI::getCandidates() {
    return candidates;
}

void FuzzySearch_POI::candidateFilter(){
    
    for (size_t order = curWordIndex; order < this->currentInput.size(); order++ )
    {
        bool found = false;
        
        for (size_t index = rangePair.first; index < rangePair.second; index++ )
        {
            if (POILib[index].second.size() < order + 1)
            {
                rangePair.first = index + 1;
                continue;
            }
            
            if (!found && (currentInput[order] == POILib[index].second[order]))
            {
                found = true;
                rangePair.first = index;
            }
            
            if (found && (currentInput[order] != POILib[index].second[order]))
            {
                rangePair.second = index;
                break;
            }
        }
        
        if (!found)
        {
            
        }
    }
    
    candidates.clear();
    
    // Add candidates to candidates vector
    for (size_t index = rangePair.first; index < rangePair.second; index++ )
    {
        this->candidates.push_back(POILib[index].first);
    }
}

void FuzzySearch_POI::update(string input){
    if (input == this->savedInput){
        return ;
    }
    
    boost::to_upper(input);
    
    this->currentInput.clear();
    
    updateCurInput(input);
    
    candidateFilter();

}
