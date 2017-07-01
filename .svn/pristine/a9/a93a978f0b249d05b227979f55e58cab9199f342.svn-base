/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <unittest++/UnitTest++.h>
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include "m1.h"
#include "StreetsDatabaseAPI.h"

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

SUITE(intersection_ids_from_street_names_test){
    TEST_FIXTURE(MapFixture, intersection_ids_from_street_names) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;
        
        expected = {7866};
        actual = find_intersection_ids_from_street_names("MacDonald Avenue", "Vimy Avenue");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {14812};
        actual = find_intersection_ids_from_street_names("Rivercrest Road", "Humberview Road");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {30269};
        actual = find_intersection_ids_from_street_names("To Highway 403 Collectors", "Highway 403 Collectors");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {77159};
        actual = find_intersection_ids_from_street_names("Golf Avenue", "Pine Valley Crescent");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {6584, 19326};
        actual = find_intersection_ids_from_street_names("Hartfield Court", "Hartfield Road");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {86929, 86931, 100287, 100288};
        actual = find_intersection_ids_from_street_names("Bathurst Street", "Eglinton Avenue West");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {61472, 61473};
        actual = find_intersection_ids_from_street_names("Chilmar Crescent", "Rosedale Heights Drive");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);
        
        expected = {51899, 51900};
        actual = find_intersection_ids_from_street_names("Cicerella Crescent", "Marcos Boulevard");
        sort(actual.begin(), actual.end());
        CHECK(expected == actual);

        }
        
        
    
}


    
