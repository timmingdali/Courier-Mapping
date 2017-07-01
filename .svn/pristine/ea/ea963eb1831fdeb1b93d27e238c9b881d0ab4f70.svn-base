/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "TSP_supplementary.h"

#define travelTimePtr (*getPtrToTravelTime())
using namespace std;
#define itsVec (*getPtrToIntersectSet())
typedef pair<IntersectionIndex, double> inside_queue;

struct compare {

    bool operator()(const inside_queue & lhs, const inside_queue & rhs) {
        //return lhs.travel_time_from_start > rhs.travel_time_from_start;
        return lhs.second > rhs.second;

    }

};

//For the purpose of quick travel time calculation, turn penalties are taken out as a tradeoff
double TSP_quick_travel_time (const std::vector<unsigned>& tentative_delivery_path){
    
    double time_result = 0;
    
    for (auto iter = tentative_delivery_path.begin(); iter != tentative_delivery_path.end(); iter++ )
    {
        time_result += travelTimePtr[*iter];
    }
    
    return time_result;
    
}

std:: vector<IntersectionSet> TSP_preCompute_find_path_between_intersections(const unsigned intersect_id_start,
        unordered_map <unsigned, bool>& pre_compute_intersects_reached, const double turn_penalty)
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
    //local_itsVec[intersect_id_start].approx_distance_to_end_vertex = coordinate_distance(local_itsVec[intersect_id_start], local_itsVec[intersect_id_end]);
    
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
            
        pq.pop();
        
        
        auto got = pre_compute_intersects_reached.find(intersection_index);
        if (got != pre_compute_intersects_reached.end())
        {
        pre_compute_intersects_reached.erase (intersection_index);
        }
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

                    //local_itsVec[adjacent_intersection_index].approx_distance_to_end_vertex =
                    //        coordinate_distance(local_itsVec[adjacent_intersection_index], local_itsVec[intersect_id_end]);

                    local_itsVec[adjacent_intersection_index].total_heuristics_travel_time =
                            local_itsVec[adjacent_intersection_index].travel_time_from_start;


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

                    //local_itsVec[adjacent_intersection_index].approx_distance_to_end_vertex =
                    //        coordinate_distance(local_itsVec[adjacent_intersection_index], local_itsVec[intersect_id_end]);

                    local_itsVec[adjacent_intersection_index].total_heuristics_travel_time =
                            local_itsVec[adjacent_intersection_index].travel_time_from_start;

                    local_itsVec[adjacent_intersection_index].predecessor_intersection = static_cast <int> (intersection_index);

                    local_itsVec[adjacent_intersection_index].connecting_street_segment = static_cast <int> ((*iter).segmentId);

                    local_itsVec[adjacent_intersection_index].connecting_street =static_cast <int>  ((*iter).streetId);

                    inside_queue adjacent_intersection_sub = make_pair(adjacent_intersection_index, local_itsVec[adjacent_intersection_index].total_heuristics_travel_time);

                    pq.push(adjacent_intersection_sub);

                }
            }
        }
        
        
        
        if (pre_compute_intersects_reached.empty())
            break;
        
        
        
    }

    /*
    unsigned traverse = intersect_id_end;
    
    //If the destination intersection's predecessor is still the default value "-1" (reason for -1 being the default value is because
    //once again, there is not intersection with index number -1, so nothing would be affected). Then this means there is no path
    //from the starting intersection to the destination.
    //if (local_itsVec[traverse].predecessor_intersection == -1) {
        

        //Recovers default values for all intersection set elements to default values since they will be utilized again for the next time being called.
        for (auto iter = itsVec.begin(); iter != itsVec.end(); ++iter) {
            (*iter).connecting_street_segment = -1;
            (*iter).travel_time_from_start = std::numeric_limits<double>::max();
            (*iter).predecessor_intersection = -1;
            (*iter).connecting_street = -1;
            (*iter).approx_distance_to_end_vertex = 0;
            (*iter).total_heuristics_travel_time = std::numeric_limits<double>::max();
        }
        
        //Return the path vector, in this case, an empty vector.
        //return path;
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
    */
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
    return local_itsVec;
    
    
    
    
}