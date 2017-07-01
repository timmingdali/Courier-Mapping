/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <algorithm>
#include <set>
#include "m1.h"
#include "unit_test_util.h"
#include "StreetsDatabaseAPI.h"
#include <unittest++/UnitTest++.h>

using ece297test::relative_error;

struct MapFixture {
    MapFixture() {
        //Load the map
        load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
    }

    ~MapFixture() {
        //Clean-up
        close_map();
    }
};

SUITE(find_streets_ids_from_name_func_public_toronto_canada) {
        
    TEST_FIXTURE(MapFixture, street_ids_from_name) {
         
        std::vector <unsigned> expected;
        std::vector <unsigned> actual;
        
        
        expected = {1808, 6481, 16023};
        actual = find_street_ids_from_name("Main Street");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {35};
        actual = find_street_ids_from_name("Yonge Street");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {5789};
        actual = find_street_ids_from_name("Bloor Street");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {2,215};
        actual = find_street_ids_from_name("Highway 401 Eastbound Express");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {0};
        actual = find_street_ids_from_name("<unknown>");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {254, 16537};
        actual = find_street_ids_from_name("Bay Street");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {11357};
        actual = find_street_ids_from_name("Highways 27, 401, 427");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {11564};
        actual = find_street_ids_from_name("Fawndale Crescent");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {19003, 19004, 19068, 19340, 19343, 19871};
        actual = find_street_ids_from_name("Dundas Street");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
        expected = {};
        actual = find_street_ids_from_name("no street called this");
        std::sort(actual.begin(),actual.end());
        CHECK_EQUAL(expected,actual);
        
                
        std::vector <std::string>  name;
        for (unsigned i = 0; i< getNumberOfStreets() ; i++){
   
        name.push_back(getStreetName(i));
        }

        for (auto iter = name.begin() ; iter != name.end() ; iter++){
               expected= {};
              actual = find_street_ids_from_name (*iter);
             
              for (unsigned i = 0; i< getNumberOfStreets() ; i++){
              if (getStreetName(i)== *iter)
                  expected.push_back(i);
              }
           std::sort(actual.begin(),actual.end());
           CHECK_EQUAL(expected,actual);
}
        
    }
    
}