/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Help_Print_Funcs.h"

using namespace std;

#define stVec (*getPtrToStSet())
#define segVec (*getPtrToStSegSet())
#define itsVec (*getPtrToIntersectSet())

void searchStreetThenPrintAllRelevant(){
    string streetName;
    
    for (;;){
        cout << "Total size of " << stVec.size() << endl;
        getline(cin, streetName);
        auto streetIds = searchStreetId(streetName);
        std::cout << "\nResult: " << streetIds.size() << std::endl;
        if (streetIds.size() != 0){
            for (auto iter = streetIds.begin(); iter != streetIds.end(); iter++ )
            {
                auto AStreet = stVec[*iter];
                
                std::cout << "Street No." << *iter <<" with " << AStreet.StreetSegVec.size() << " segments" << std::endl;
                for (auto iter2 = AStreet.StreetSegVec.begin(); iter2 != AStreet.StreetSegVec.end(); iter2 ++)
                {
                    auto ASeg = segVec[*iter2];
                    std::cout << "  Segment" << endl;
                    std::cout << "    From: lat: " << itsVec[ASeg.info.from].location.lat() << " lon: " << itsVec[ASeg.info.from].location.lon() << endl;
                    for (auto iter3 = ASeg.curPts.begin(); iter3 != ASeg.curPts.end(); iter3 ++)
                    {
                        std::cout << "      lat: " << iter3->lat() << " lon: " << iter3->lon() << endl;
                    }
                    std::cout << "    To: lat: " << itsVec[ASeg.info.to].location.lat() << " lon: " << itsVec[ASeg.info.to].location.lon() << endl;
                }
            }
        }
        else
            std::cout << "Street does not exist" << endl;
    }
}

void printStringVector(vector<string> vec){
    for (auto iter = vec.begin(); iter != vec.end(); iter++ )
    {
        cout << (*iter) << endl;
    }
}

// Returns the original input with all words having their first letter capital
string stringParser(string input) {
    boost::to_lower(input);
    
    stringstream tempStream(input);
    
    string result, tempString;
    
    while (tempStream >> tempString)
    {
        tempString[0] = toupper(tempString[0]);
        result.append(tempString + ' ');
    }
    
    result.pop_back();
    
    return result;
}

// Separating by space, the function returns how many words are in a string
unsigned wordCounter(string input) {
    
    stringstream inputStream(input);
    
    unsigned wordCount = 0;
    
    while (inputStream >> input)
        wordCount ++;
    
    return wordCount;
}