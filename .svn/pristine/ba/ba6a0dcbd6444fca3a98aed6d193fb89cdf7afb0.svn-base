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
        load_map("/cad2/ece297s/public/maps/saint-helena.streets.bin");
    }

    ~MapFixture() {
        //Clean-up
        close_map();
    }
};

SUITE(find_streets_ids_from_name_func_public_saint_helena) {
        
    TEST_FIXTURE(MapFixture, street_ids_from_name) {

        std::vector<unsigned> expected;
        std::vector<unsigned> actual;         
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