/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fuzzyLib.cpp
 * Author: liuhao22
 * 
 * Created on February 21, 2017, 5:38 PM
 */

#include "FuzzySearch.h"

#define searchLib (*getPtrToSearchLib())
#define stVec (*getPtrToStSet())
#define segVec (*getPtrToStSegSet())
#define itsVec (*getPtrToIntersectSet())
using namespace std;

FuzzySearch::FuzzySearch(string input):
    savedInput(input), curWordIndex(0)
{   

    this->rangePairClt.push_back( make_pair(size_t(0), searchLib.size()) );
    
    updateCurInput(input);
    
    this->candidateFilter();
}

FuzzySearch::~FuzzySearch() {
}

std::vector<std::string> FuzzySearch::getCandidates(){
    return this->candidates;
}

void FuzzySearch::updateCurInput(std::string input){
    
    string word;
    
    boost::to_upper(input);
    
    stringstream inputStream(input);
    
    while (inputStream >> word){
        this->currentInput.push_back(word);
    }
}

void FuzzySearch::update(string input){
    if (input == this->savedInput){
        return ;
    }
    
    boost::to_upper(input);
    
    this->currentInput.clear();
    
    updateCurInput(input);
    
    candidateFilter();

}

void FuzzySearch::candidateFilter(){
    
    std::vector<std::pair<size_t, size_t> > tempRangeContainer;

    // A bit complex to explain the algorithm here, just ask me about this
    for (size_t order = curWordIndex; order < this->currentInput.size(); order ++)
    {
        bool found = false;
        
        for (auto begin_end = this->rangePairClt.begin(); begin_end != this->rangePairClt.end(); begin_end++ )
        {
            for (size_t index = begin_end->first; index < begin_end->second; index++ )
            {
                if (searchLib[index].second.size() < order + 1)
                {
                    begin_end->first = index + 1;
                    continue;
                }

                if (!found && (this->currentInput[order] == searchLib[index].second[order]) )
                {
                    found = true;
                    begin_end->first = index;
                }

                if (found && (this->currentInput[order] != searchLib[index].second[order]) )
                {
                    begin_end->second = index;
                    break;
                }
            }
            
            // If no exact result is found, process fuzzy search
            if (!found)
            {
                auto fuzzySearchResult = this->fuzzyMatching(order);

                // If fuzzy search found multiple results

                if (!fuzzySearchResult.empty())
                {
                    for (auto iter = fuzzySearchResult.begin(); iter != fuzzySearchResult.end(); iter++ )
                    {
                        tempRangeContainer.push_back(*iter);
                    }
                }
                else
                {
                    // Clear out candidates and current input vector if no result found
                    return;
                }
            }
            
            // Updates current dealing word's index
            this->curWordIndex++;
        }
        
        if (!tempRangeContainer.empty())
        {
            this->rangePairClt.clear();

            // replace self container with temp container
            this->rangePairClt.insert(rangePairClt.end(), tempRangeContainer.begin(), tempRangeContainer.end());
        }
    }
    // Updates candidates vector
    this->candidates.clear();
    
    // Add candidates to candidates vector
    for (auto begin_end = this->rangePairClt.begin(); begin_end != this->rangePairClt.end(); begin_end++ )
    {
        for (size_t index = begin_end->first; index < begin_end->second; index++ )
        {
            this->candidates.push_back(searchLib[index].first);
        }
    }
}

size_t FuzzySearch::getCurrentWordIndex() {
    return this->curWordIndex;
}

vector< pair<size_t, size_t> > FuzzySearch::fuzzyMatching(size_t order){
    
    vector< pair<string, pair<size_t, size_t> > > wordSliceClt;
    
    vector< pair<size_t, size_t> > retVec;
    
    string inputWord = this->currentInput[order];
    
    // Construct vector for use of LevenshteinDistance function:
    // Group up all street with current word slice into 1 group, take index of the
    // start and the end of the "group", and then seal them into a pair with the
    // word slice as the "first", pair of "first" and "second" as the "second"
    for (pair<size_t, size_t> &eachPair: rangePairClt)
    {
        unsigned rbegin = eachPair.first;
        unsigned end = eachPair.second;
        for (size_t index = rbegin; index < end; index++)
        {
            if (searchLib[index].second.size() <= order) 
            {
                index++;
                rbegin = index;
                continue;
            }
            
            if (searchLib[rbegin].second[order] != searchLib[index].second[order])
            {
                // Constructing content to be added into wordSliceClt
                pair<size_t, size_t> tempIntPair(rbegin, index);
                pair<string, pair<size_t, size_t> > tempPair(searchLib[rbegin].second[order], tempIntPair);

                wordSliceClt.push_back(tempPair);

                rbegin = index;
            }
        }
    }
    
    // Processing word slices collection
    // By calculating levenshtein distance of the "first" of each pair, keep data of pairs
    // with their distance equal or less than LEVENSHTEIN_DISTANCE_LIMIT
    for (auto iter = wordSliceClt.begin(); iter != wordSliceClt.end(); iter ++)
    {
        int LevVal = LevenshteinDistance(inputWord, iter->first);
        
        if (LevVal <= LEVENSHTEIN_DISTANCE_LIMIT){
            pair<size_t, size_t> tempIndexPair(iter->second);
            retVec.push_back(tempIndexPair);
        }
        else if (inputWord == string(iter->first.begin(), iter->first.begin() + inputWord.size()) )
        {
            pair<size_t, size_t> tempIndexPair(iter->second);
            retVec.push_back(tempIndexPair);
        }
    }
    
    return retVec;
}

// Levenshtein Distance: Returns a integer that tells the minimum step to convert
// from one string to another
int FuzzySearch::LevenshteinDistance(string str1, string str2){
    int n = str1.size();
    
    int m = str2.size();
    
    if (n == 0)
        return m;
    if (m == 0)
        return n;
    
    vector<int> vec1(m+1);
    
    vector<int> vec2(m+1);
    
    for(int i=0;i<=m;i++)
        vec1[i] = i;
    
    int cost = 0;

    for(int i=1;i<=n;i++)
    {
        vec2[0] = i;
        for(int j=1;j<=m;j++)
        {
            if( str1[i-1] == str2[j-1] )
                cost = 0;
            else
                cost = 1;
            vec2[j] = vec2[j-1]+1 < vec1[j]+1 ? vec2[j-1]+1 : vec1[j]+1;
            vec2[j] = vec2[j] < vec1[j-1]+cost ? vec2[j] : vec1[j-1]+cost;
        }
        vec1 = vec2;
    }
    return vec2.back();
}

