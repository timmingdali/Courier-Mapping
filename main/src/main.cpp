#include <iostream>
#include <string>
#include "Help_Print_Funcs.h"
#include "API_module.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include <vector>
#include <set>
#include <sstream>
#include <ctime>

#include "load_map.h"
#include "graphics.h"
#include "Fuzzy_Search_Lib.h"
#include "FuzzySearch.h"
#include "Scale_dict.h"
#include "routeInstruction.h"
// For testing
#include <sstream>
#include "StreetGraphEdges.cpp"
#include "unit_test_util.h"
#include "courier_verify.h"

#define stVec (*getPtrToStSet())
#define segVec (*getPtrToStSegSet())
#define itsVec (*getPtrToIntersectSet())

bool loadAPI = false;

int main(int argc, char** argv) {
    while (!cin.eof()) {
        cin.clear();
        string stringRead, city, country;
        cout << "Please enter the name of the city, followed by the country name in lower case: ";

        getline(cin, stringRead);
        std::string map_path = "/cad2/ece297s/public/maps/";

        if (argc == 1) {

            stringstream lineRead(stringRead);
            lineRead >>city;
            lineRead>>country;
            if (country == "") {
                map_path.append(city);
            } else {
                map_path.append(city);
                map_path.append("_");
                map_path.append(country);
            }

        } else if (argc == 2) {
            //Get the map from the command line
            map_path = argv[1];
        } else {
            //Invalid arguments
            std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
            std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
            return 1;
        }
        //Load the map and related data structures
        bool load_success = load_map(map_path);
        if (!load_success) {
            std::cout << "Invalid map name" << std::endl;

            std::cerr << "Failed to load map '" << map_path << "'\n";
            continue;
        }

        std::cout << "Successfully loaded map '" << map_path << "'\n";


        searchEngineInitializer();

        draw_map();
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        std::vector<unsigned> result_path;
        float turn_penalty;
        using ece297test::relative_error;
        using ece297test::courier_path_is_legal;
        deliveries = {DeliveryInfo(0, 1), DeliveryInfo(1, 0)};
        depots = {1364};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        std::cout << courier_path_is_legal(deliveries, depots, result_path) <<"\n"<<result_path.size() << std::endl;

        //Clean-up the map related data structures


        std::cout << "Closing map\n";

        close_map();
    }
}

