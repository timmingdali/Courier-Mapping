/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <algorithm>
#include <set>
#include <string>
#include "m1.h"
#include "unit_test_util.h"
#include "StreetsDatabaseAPI.h"
#include <unittest++/UnitTest++.h>

using ece297test::relative_error;

struct MapFixture {
    MapFixture() {
        //Load the map
        load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");

        //Initialize random number generators
        rng = std::minstd_rand(3);     
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
      
    }

    ~MapFixture() {
        //Clean-up
        close_map();
    }
      std::minstd_rand rng;
      std::uniform_int_distribution<unsigned> rand_street;
};

SUITE(perf_find_streets_ids_from_name_func_public_toronto_canada) {
        //veryfy functionality before going to peformance
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
        
        //speed test starts from here
        //generate random ids within the range of all possible streetIDs
        std::vector<unsigned> street_ids;
        for(size_t i = 0; i < 1000000; i++) {
            street_ids.push_back(rand_street(rng));
        }
        
        //generate random (but valid) street names based on the ids
        std::vector <std::string> street_names;
       
        for(size_t i = 0; i < 1000000; i++) 
        {
            street_names.push_back(getStreetName(street_ids[i]));
        }
       
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(250);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 1000000; i++) {
                result = find_street_ids_from_name(street_names[i]);
               
            }
        }
        
        
        
        
    }
}