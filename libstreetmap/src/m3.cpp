/*
 * 
 */

#include "m3.h"
#include "m1.h"
#include "load_map.h"
#include "StreetGraphEdges.h"
#include <queue>
#include <vector>
#include <functional>
#include <limits>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <utility>
#include <chrono>
using namespace std;
#define itsVec (*getPtrToIntersectSet())

    typedef pair<IntersectionIndex, double> inside_queue;

struct compare {

    bool operator()(const inside_queue & lhs, const inside_queue & rhs) {
        //return lhs.travel_time_from_start > rhs.travel_time_from_start;
        return lhs.second > rhs.second;

    }

};

double compute_path_travel_time(const vector<unsigned>& path,
        const double turn_penalty) {
    
    if (path.empty())
        return 0;
    else {
        //add up the tiems for each segment
        double time = (*getPtrToTravelTime())[path[0]];
       // unsigned prev, cur;

    #pragma omp parallel for reduction (+:time)
        for (auto iter = path.begin()+1; (iter ) < path.end();iter++) {

          unsigned  prev = (*getPtrToStSegSet())[*(iter-1)].info.streetID;
            
          unsigned  cur = (*getPtrToStSegSet())[*iter].info.streetID;
            //turn penalty
            if (prev != cur) {
                time += turn_penalty;
            }
            
            time += (*getPtrToTravelTime())[*iter];
        }

        return time;
    }

}

vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
 {
    
    
    
    
    
    vector <IntersectionSet> local_itsVec = itsVec; 
    
    
    //This vector of boolean has as many of elements as the number there are for each intersection, this vector is used later on
    //for facilitating the Dijkstra's algorithm in which case a previously "popped" vertex wouldn't be relaxed again
    vector <bool> result_set(getNumberOfIntersections(), false);

    vector <unsigned> path;
    
    //Priority queue initialized with "inside_queue" as contained elements, refer to the top of this source file for what "inside_queue" exactly is
    priority_queue<inside_queue, vector<inside_queue>, compare> pq;
    
    //Change the starting intersection's contained values: travel time from start is no longer infinity, but zero, since the distance from the starting 
    //intersection to itself is zero. 
    local_itsVec[intersect_id_start].travel_time_from_start = 0;
    local_itsVec[intersect_id_start].approx_distance_to_end_vertex = coordinate_distance(local_itsVec[intersect_id_start], local_itsVec[intersect_id_end]);
    
    //total heuristics travel time is the combined value of each intersection's travel time from start and the approximate weight of each intersection to the ending intersection
    local_itsVec[intersect_id_start].total_heuristics_travel_time = local_itsVec[intersect_id_start].travel_time_from_start;
    result_set[intersect_id_start] = true;

    //The first element inside the queue is initialized, in which is contains the intersection's intersection index number and the value
    //of total heuristics
    inside_queue first_inside_queue = make_pair(intersect_id_start, local_itsVec[intersect_id_start].total_heuristics_travel_time);

    pq.push(first_inside_queue);

    
    
    /* The essence of Dijkstra's algorithm is, through starting at the "source" vertex, in our case, the source intersection, proppagates outwards
     * while evaluate everyone single vertex that connects to the previous intersections. Determine if the weights on the current intersection being 
     * examined has a higher weight, in our case the travel time from start, than the current case. If so, relaxes the edges and updates the intersection's
     * weight. In the end, after having visited all intersections (each intersection exactly once), each intersection contains exactly the traveling time from
     * source and its predecessor intersection, thereby enabling tracing the series of street segments taken to reach the final intersection. 
     **/
    
    while (!pq.empty()) {
        
        unsigned intersection_index = pq.top().first;

        //When the destination intersection is popped out from the priority queue, the travel time to the destination intersection
        //is obtained and to save running time, the algorithm can be stopped.
        if (intersection_index == intersect_id_end)
            break;

        pq.pop();
        
        //If the popped intersection has already been evaluated before, then this current intersection is indeed not the most ideal one
        //As such, this intersection can be skipped.
        if (intersection_index != intersect_id_start && result_set[intersection_index] == true) {
            continue;
        } else {
            result_set[intersection_index] = true;
        }

        
        //Run a for loop in which the current intersection's street segments are evaluated along side the connected 
        //intersection on the other side of the street segment.
        for (auto iter = local_itsVec[intersection_index].street_segment_edge.begin();
                iter != local_itsVec[intersection_index].street_segment_edge.end(); ++iter) {

            unsigned adjacent_intersection_index = (*iter).intersection_index;
            double adjacent_intersection_index_weight = (*iter).travel_time;
            

            //If current intersection index is the source vertex, in which case there is not connecting street segment 
            //Or if the intersection index connecting_street_segment is the same as the current edge index number
            if (intersection_index == intersect_id_start
                    || ((local_itsVec[intersection_index].connecting_street == static_cast <int> ((*iter).streetId))
                    && (local_itsVec[intersection_index].connecting_street_segment != static_cast <int>  ((*iter).segmentId)))) {
                
                //If the current intersection qualifies for relation
                if (local_itsVec[adjacent_intersection_index].travel_time_from_start >
                        local_itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight) {

                    local_itsVec[adjacent_intersection_index].travel_time_from_start =
                            local_itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight;

                    local_itsVec[adjacent_intersection_index].approx_distance_to_end_vertex =
                            coordinate_distance(local_itsVec[adjacent_intersection_index], local_itsVec[intersect_id_end]);

                    local_itsVec[adjacent_intersection_index].total_heuristics_travel_time =
                            local_itsVec[adjacent_intersection_index].approx_distance_to_end_vertex + local_itsVec[adjacent_intersection_index].travel_time_from_start;


                    local_itsVec[adjacent_intersection_index].predecessor_intersection = intersection_index;

                    local_itsVec[adjacent_intersection_index].connecting_street_segment = static_cast <int> ((*iter).segmentId);

                    local_itsVec[adjacent_intersection_index].connecting_street = static_cast <int> ((*iter).streetId);
                    
                    inside_queue adjacent_intersection_sub = make_pair(adjacent_intersection_index, local_itsVec[adjacent_intersection_index].total_heuristics_travel_time);
                    
                    pq.push(adjacent_intersection_sub);

                }
            } else if (local_itsVec[intersection_index].connecting_street != static_cast <int> ((*iter).streetId)) {

                if (local_itsVec[adjacent_intersection_index].travel_time_from_start >
                        local_itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight + turn_penalty) {

                    local_itsVec[adjacent_intersection_index].travel_time_from_start =
                            local_itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight + turn_penalty;

                    local_itsVec[adjacent_intersection_index].approx_distance_to_end_vertex =
                            coordinate_distance(local_itsVec[adjacent_intersection_index], local_itsVec[intersect_id_end]);

                    local_itsVec[adjacent_intersection_index].total_heuristics_travel_time =
                            local_itsVec[adjacent_intersection_index].approx_distance_to_end_vertex + local_itsVec[adjacent_intersection_index].travel_time_from_start;

                    local_itsVec[adjacent_intersection_index].predecessor_intersection = static_cast <int> (intersection_index);

                    local_itsVec[adjacent_intersection_index].connecting_street_segment = static_cast <int> ((*iter).segmentId);

                    local_itsVec[adjacent_intersection_index].connecting_street =static_cast <int>  ((*iter).streetId);

                    inside_queue adjacent_intersection_sub = make_pair(adjacent_intersection_index, local_itsVec[adjacent_intersection_index].total_heuristics_travel_time);

                    pq.push(adjacent_intersection_sub);

                }
            }
        }
    }

    unsigned traverse = intersect_id_end;
    
    //If the destination intersection's predecessor is still the default value "-1" (reason for -1 being the default value is because
    //once again, there is not intersection with index number -1, so nothing would be affected). Then this means there is no path
    //from the starting intersection to the destination.
    if (local_itsVec[traverse].predecessor_intersection == -1) {
        
        /*
        //Recovers default values for all intersection set elements to default values since they will be utilized again for the next time being called.
        for (auto iter = itsVec.begin(); iter != itsVec.end(); ++iter) {
            (*iter).connecting_street_segment = -1;
            (*iter).travel_time_from_start = std::numeric_limits<double>::max();
            (*iter).predecessor_intersection = -1;
            (*iter).connecting_street = -1;
            (*iter).approx_distance_to_end_vertex = 0;
            (*iter).total_heuristics_travel_time = std::numeric_limits<double>::max();
        }
        */
        //Return the path vector, in this case, an empty vector.
        return path;
    } else {
        //If there is a path that connects the starting intersection to the destination intersection:
        //Traces backwards from the destination intersection, records its predecessor intersection and the connecting 
        //street segment id, continue this recursively until the source intersection is reached. Reverse the path vector
        //so the order of street segments returned is correct. Return it!
        do {

            unsigned k = local_itsVec[traverse].connecting_street_segment;
            int previous_intersection = local_itsVec[traverse].predecessor_intersection;

            traverse = previous_intersection;

            path.push_back(k);

        } while (traverse != intersect_id_start);

    }
    reverse(path.begin(), path.end());

    /*
    for (auto iter = itsVec.begin(); iter != itsVec.end(); ++iter) {
        (*iter).connecting_street_segment = -1;
        (*iter).travel_time_from_start = std::numeric_limits<double>::max();
        (*iter).predecessor_intersection = -1;
        (*iter).connecting_street = -1;
        (*iter).approx_distance_to_end_vertex = 0;
        (*iter).total_heuristics_travel_time = std::numeric_limits<double>::max();
    }
     */
    return path;
}

std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start,
        const std::string point_of_interest_name,
        const double turn_penalty) {

    //get the pois under the same name
    vector <unsigned > POIs_to_look = (*getPtrtoPOItable())[point_of_interest_name];

    //if the name is wrong
    if (POIs_to_look.empty())
        return POIs_to_look; //empty vector anyways


    //this will be the intersection we will be looking at
    vector <unsigned> intersections_to_look;



    //insert the intersection positions that are closest to each point of interest
    for (auto iter = POIs_to_look.begin(); iter != POIs_to_look.end(); iter++) {

     intersections_to_look.push_back((*getPtrToPOISet())[*iter].closestIntersect);
    }


    //a temporary path variable to store the path
    vector <unsigned> path;
    vector < vector <unsigned> > path_candidates;
    
    /****************          Dijkstra's Algorithm with multiple destinations     ************/
    
    vector <bool> result_set(getNumberOfIntersections(), false);
    vector <unsigned> remained_intersections(intersections_to_look);

    //min heap to store the elements using stl structure
    priority_queue<inside_queue, vector<inside_queue>, compare> pq;

    //from src to src is path 0
    itsVec[intersect_id_start].travel_time_from_start = 0;
    
    //from start to start huristic function is also 0
    itsVec[intersect_id_start].approx_distance_to_end_vertex = 0; //coordinate_distance(itsVec[intersect_id_start], itsVec[intersections_to_look[0]]);
    itsVec[intersect_id_start].total_heuristics_travel_time = itsVec[intersect_id_start].travel_time_from_start;
   
    //the result only neds to be set once
    result_set[intersect_id_start] = true;

    inside_queue first_inside_queue = make_pair(intersect_id_start, itsVec[intersect_id_start].total_heuristics_travel_time);

    pq.push(first_inside_queue);
    unsigned found=0;
    //loop through for breadth first traversal 
    while (!pq.empty()) {

        unsigned intersection_index = pq.top().first;

        //determine if the intersection belongs to the intersections we want to get
        auto locked_intersection = find(remained_intersections.begin(), remained_intersections.end(), intersection_index);

        //if the intersection is being looked at now, it is set to be the best path
        //so push it
        if (locked_intersection != remained_intersections.end()) {      
            found=*locked_intersection;
          break;//  remained_intersections.erase(locked_intersection);
        }
        //when all the intersections we are interested are looked at
        //and locked the best path
        if (remained_intersections.empty()) {
            break;
        }

        //otherwise
        pq.pop();

        if (intersection_index != intersect_id_start && result_set[intersection_index] == true) {
            continue;
        } else {
            result_set[intersection_index] = true;
        }


        for (auto iter = itsVec[intersection_index].street_segment_edge.begin();
                iter != itsVec[intersection_index].street_segment_edge.end(); ++iter) {

            unsigned adjacent_intersection_index = (*iter).intersection_index;
            double adjacent_intersection_index_weight = (*iter).travel_time;



            //If current intersection index is the source vertex, in which case there is not connecting street segment 
            //Or if the intersection index connecting_street_segment is the same as the current edge index number
            if (intersection_index == intersect_id_start
                    || ((itsVec[intersection_index].connecting_street == static_cast <int> ((*iter).streetId))
                    && (itsVec[intersection_index].connecting_street_segment != static_cast <int> ((*iter).segmentId)))) {

                if (itsVec[adjacent_intersection_index].travel_time_from_start >
                        itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight) {

                    itsVec[adjacent_intersection_index].travel_time_from_start =
                            itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight;
                    //no hueristic function for multiple destinations since its hard to decide what to set to
                    itsVec[adjacent_intersection_index].approx_distance_to_end_vertex = 0;

                    //set the total weight for getting the adjacent intersection
                    itsVec[adjacent_intersection_index].total_heuristics_travel_time =
                            itsVec[adjacent_intersection_index].approx_distance_to_end_vertex + itsVec[adjacent_intersection_index].travel_time_from_start;

                    //set predecessors 
                    itsVec[adjacent_intersection_index].predecessor_intersection = intersection_index;

                    itsVec[adjacent_intersection_index].connecting_street_segment = static_cast <int> ((*iter).segmentId);

                    itsVec[adjacent_intersection_index].connecting_street = static_cast <int> ((*iter).streetId);

                    inside_queue adjacent_intersection_sub = make_pair(adjacent_intersection_index, itsVec[adjacent_intersection_index].total_heuristics_travel_time);
                    //push the updated value in the queue to see if the update can cause any more relaxations
                    
                    pq.push(adjacent_intersection_sub);

                }
            } 
            //with turn penalty 
            else if (itsVec[intersection_index].connecting_street != static_cast <int> ((*iter).streetId)) {

                if (itsVec[adjacent_intersection_index].travel_time_from_start >
                        itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight + turn_penalty) {

                    itsVec[adjacent_intersection_index].travel_time_from_start =
                            itsVec[intersection_index].travel_time_from_start + adjacent_intersection_index_weight + turn_penalty;

                    itsVec[adjacent_intersection_index].approx_distance_to_end_vertex = 0;
                    //    coordinate_distance(itsVec[adjacent_intersection_index], itsVec[intersections_to_look[0]]);

                    itsVec[adjacent_intersection_index].total_heuristics_travel_time =
                            itsVec[adjacent_intersection_index].approx_distance_to_end_vertex + itsVec[adjacent_intersection_index].travel_time_from_start;

                    itsVec[adjacent_intersection_index].predecessor_intersection = intersection_index;

                    itsVec[adjacent_intersection_index].connecting_street_segment = static_cast <int> ((*iter).segmentId);

                    itsVec[adjacent_intersection_index].connecting_street = static_cast <int>  ((*iter).streetId);

                    inside_queue adjacent_intersection_sub = make_pair(adjacent_intersection_index, itsVec[adjacent_intersection_index].total_heuristics_travel_time);

                    pq.push(adjacent_intersection_sub);

                }
            }
        }
    }


        //traverse this specific intersection
        int traverse = found;

        //clear the path before back traversal 
        path.clear();

        
        if ((*getPtrToIntersectSet())[traverse].predecessor_intersection == -1){
             path_candidates.push_back(path);
        } 
        else {
       

            do {
                
                int k = itsVec[traverse].connecting_street_segment;
                int previous_intersection = itsVec[traverse].predecessor_intersection;

                traverse = previous_intersection;

                path.push_back(k);

            } while (traverse != static_cast <int> (intersect_id_start));

        }
        //reverse the path tp get the correct path 
        reverse(path.begin(), path.end());
        path_candidates.push_back(path);
 
    //clean up before exiting search 
    for (auto iter = itsVec.begin(); iter != itsVec.end(); ++iter) {
        (*iter).connecting_street_segment = -1;
        (*iter).travel_time_from_start = std::numeric_limits<double>::max();
        (*iter).predecessor_intersection = -1;
        (*iter).connecting_street = -1;
        (*iter).approx_distance_to_end_vertex = 0;
        (*iter).total_heuristics_travel_time = std::numeric_limits<double>::max();
    }
    return path;

}


