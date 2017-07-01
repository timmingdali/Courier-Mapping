/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m4.h"
#include "load_map.h"
#include "TSP_supplementary.h"
#include <unordered_map>
#include "m3.h"
#include <ctime>
#include "m4_helper.h"
#include <vector>
#include <time.h> 
#include <utility>
#include <bits/stl_vector.h>
#include <queue>
#include <chrono>
#define MAX_T 29.5
#define THREE_OPT_BOUND 20
#define  itsVec (*getPtrToIntersectSet())
using namespace std;

typedef pair <double, vector <unsigned>> k_opt_inside;

struct compare_k_opt {

    bool operator()(const k_opt_inside & lhs, const k_opt_inside & rhs) {

        return lhs.first > rhs.first;

    }

};

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty) {


    using namespace std::chrono;

    steady_clock::time_point func_start = steady_clock::now();


    //#define testTime
#ifdef testTime
    clock_t begin = clock();
#endif
#ifdef heuristic 
    priority_queue<Remained_Nodes, vector<Remained_Nodes>, TSP_Com> places_to_visit;
#endif
    //unordered_map for pre computation of path findings
    unordered_map <pre_computed_path_key, pair < double, vector <unsigned> >, pre_computed_path_hash, KeyEqual > pre_computed_street_segment_paths;


    unordered_map <unsigned, bool> check_if_all_intersections_reached;

    for (auto iter = deliveries.begin(); iter != deliveries.end(); iter++) {
        check_if_all_intersections_reached.insert({iter->pickUp, true});
        check_if_all_intersections_reached.insert({iter->dropOff, true});
    }


#pragma omp parallel for
    for (int iter = 0; iter < deliveries.size(); iter++) {

        unordered_map <unsigned, bool> pre_compute_pickUp_intersects_reached = check_if_all_intersections_reached;

        unordered_map <unsigned, bool> pre_compute_dropOff_intersects_reached = check_if_all_intersections_reached;

        vector <IntersectionSet> pre_compute_pickUp_paths = TSP_preCompute_find_path_between_intersections(deliveries[iter].pickUp, pre_compute_pickUp_intersects_reached, turn_penalty);

        vector <IntersectionSet> pre_compute_dropOff_paths = TSP_preCompute_find_path_between_intersections(deliveries[iter].dropOff, pre_compute_dropOff_intersects_reached, turn_penalty);



        for (auto iter2 = deliveries.begin(); iter2 != deliveries.end(); iter2++) {

            bool check_pickUp_valid = true;
            bool check_dropOff_valid = true;

            vector <unsigned> path_pickUp_to_dropOff;
            vector <unsigned> path_pickUp_to_pickUp;

            if (pre_compute_pickUp_paths[iter2->dropOff].predecessor_intersection == -1) {
                check_dropOff_valid = false;

            }
            if (pre_compute_pickUp_paths[iter2->pickUp].predecessor_intersection == -1
                    && deliveries[iter].pickUp != iter2->pickUp) {
                check_pickUp_valid = false;

            }

            unsigned traverse_pickUp = iter2->dropOff;

            if (check_dropOff_valid == true) {

                do {

                    unsigned k = pre_compute_pickUp_paths[traverse_pickUp].connecting_street_segment;
                    int previous_intersection = pre_compute_pickUp_paths[traverse_pickUp].predecessor_intersection;

                    traverse_pickUp = previous_intersection;

                    path_pickUp_to_dropOff.push_back(k);

                } while (traverse_pickUp != deliveries[iter].pickUp);

                reverse(path_pickUp_to_dropOff.begin(), path_pickUp_to_dropOff.end());

                pre_computed_path_key pickUp_2_dropOff_struct(deliveries[iter].pickUp, iter2->dropOff);

                double travel_time = compute_path_travel_time(path_pickUp_to_dropOff, turn_penalty);
#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(pickUp_2_dropOff_struct, make_pair(travel_time, path_pickUp_to_dropOff)));

            } else {

                vector<unsigned> nothing;
                pre_computed_path_key pickUp_2_dropOff_struct(deliveries[iter].pickUp, iter2->dropOff);

#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(pickUp_2_dropOff_struct, make_pair(0, nothing)));


            }

            traverse_pickUp = iter2->pickUp;

            if (deliveries[iter].pickUp != traverse_pickUp && check_pickUp_valid == true) {

                do {

                    unsigned k = pre_compute_pickUp_paths[traverse_pickUp].connecting_street_segment;
                    int previous_intersection = pre_compute_pickUp_paths[traverse_pickUp].predecessor_intersection;

                    traverse_pickUp = previous_intersection;

                    path_pickUp_to_pickUp.push_back(k);

                } while (traverse_pickUp != deliveries[iter].pickUp);

                reverse(path_pickUp_to_pickUp.begin(), path_pickUp_to_pickUp.end());

                pre_computed_path_key pickUp_2_pickUp_struct(deliveries[iter].pickUp, iter2->pickUp);
                double travel_time = compute_path_travel_time(path_pickUp_to_pickUp, turn_penalty);

#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(pickUp_2_pickUp_struct, make_pair(travel_time, path_pickUp_to_pickUp)));

            } else {
                vector<unsigned> nothing;
                pre_computed_path_key pickUp_2_pickUp_struct(deliveries[iter].pickUp, iter2->pickUp);
#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(pickUp_2_pickUp_struct, make_pair(0, nothing)));
            }

        }


        for (auto iter2 = deliveries.begin(); iter2 != deliveries.end(); iter2++) {
            bool check_pickUp_valid = true;
            bool check_dropOff_valid = true;
            vector <unsigned> path_dropOff_to_dropOff;
            vector <unsigned> path_dropOff_to_pickUp;

            if (pre_compute_dropOff_paths[iter2->pickUp].predecessor_intersection == -1) {
                check_pickUp_valid = false;

            }
            if (pre_compute_dropOff_paths[iter2->dropOff].predecessor_intersection == -1
                    && deliveries[iter].dropOff != iter2->dropOff) {
                check_dropOff_valid = false;


            }

            unsigned traverse_dropOff = iter2->dropOff;

            if ((deliveries[iter].dropOff != traverse_dropOff) && check_dropOff_valid == true) {

                do {

                    unsigned k = pre_compute_dropOff_paths[traverse_dropOff].connecting_street_segment;
                    int previous_intersection = pre_compute_dropOff_paths[traverse_dropOff].predecessor_intersection;

                    traverse_dropOff = previous_intersection;

                    path_dropOff_to_dropOff.push_back(k);

                } while (traverse_dropOff != deliveries[iter].dropOff);

                reverse(path_dropOff_to_dropOff.begin(), path_dropOff_to_dropOff.end());

                pre_computed_path_key dropOff_2_dropOff(deliveries[iter].dropOff, iter2->dropOff);
                double travel_time = compute_path_travel_time(path_dropOff_to_dropOff, turn_penalty);
#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(dropOff_2_dropOff, make_pair(travel_time, path_dropOff_to_dropOff)));

            } else {
                vector <unsigned> nothing;
                pre_computed_path_key dropOff_2_dropOff(deliveries[iter].dropOff, iter2->dropOff);
#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(dropOff_2_dropOff, make_pair(0, nothing)));

            }

            traverse_dropOff = iter2->pickUp;

            if (check_pickUp_valid == true) {

                do {

                    unsigned k = pre_compute_dropOff_paths[traverse_dropOff].connecting_street_segment;
                    int previous_intersection = pre_compute_dropOff_paths[traverse_dropOff].predecessor_intersection;

                    traverse_dropOff = previous_intersection;

                    path_dropOff_to_pickUp.push_back(k);

                } while (traverse_dropOff != deliveries[iter].dropOff);

                reverse(path_dropOff_to_pickUp.begin(), path_dropOff_to_pickUp.end());

                pre_computed_path_key dropOff_2_pickUp(deliveries[iter].dropOff, iter2->pickUp);
                double travel_time = compute_path_travel_time(path_dropOff_to_pickUp, turn_penalty);
#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(dropOff_2_pickUp, make_pair(travel_time, path_dropOff_to_pickUp)));

            } else {

                vector <unsigned> nothing;
                pre_computed_path_key dropOff_2_pickUp(deliveries[iter].dropOff, iter2->pickUp);
#pragma omp critical
                pre_computed_street_segment_paths.insert(make_pair(dropOff_2_pickUp, make_pair(0, nothing)));

            }
        }

    }
#ifdef testTime
    clock_t end = clock();
    double elapsed_secs = double (end - begin) / CLOCKS_PER_SEC;
    std::cout << "pre compute took " << elapsed_secs << " seconds." << endl;
    begin = end;
#endif
#ifdef testTime_wall
    steady_clock::time_point now = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(now - func_start);
    cout << "pre compute took " << time_span.count() << endl;
#endif     
    /**************************    END OF COMPUTING PATHS    *************************************/
    /*************************************************************************************/




    //rtree initializations to facilitate finding closest places in generall
    boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > depos_r;

    boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > pickUps_r;

    boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > dropOffs_r;

    unordered_map <unsigned, vector <unsigned> > get_pickup_by_drop;
    vector <unsigned> not_picked, not_dropped;


    //construct the necessary infor needed to compute path
    for (auto iter = depots.begin(); iter != depots.end(); iter++) {
        point depo_to_insert = point(itsVec[*iter].cartesian_p.first, itsVec[*iter].cartesian_p.second);

        //insert the depo positions along with the ID
        depos_r.insert(make_pair(depo_to_insert, *iter));
    }

    //constructing the spatial stuff
    for (auto iter = deliveries.begin(); iter != deliveries.end(); iter++) {
        point drop_to_insert = point(itsVec[iter->dropOff].cartesian_p.first, itsVec[iter->dropOff].cartesian_p.second);
        dropOffs_r.insert(make_pair(drop_to_insert, iter->dropOff));

        point pick_to_insert = point(itsVec[iter->pickUp].cartesian_p.first, itsVec[iter->pickUp].cartesian_p.second);
        pickUps_r.insert(make_pair(pick_to_insert, iter->pickUp));

        not_picked.push_back(iter->pickUp);
        not_dropped.push_back(iter->dropOff);

        auto insert_relation = get_pickup_by_drop.find(iter->dropOff);
        if (insert_relation == get_pickup_by_drop.end()) {
            vector <unsigned> to_insert(1, iter->pickUp);
            get_pickup_by_drop.insert(make_pair(iter->dropOff, to_insert));
        }
        else {
            get_pickup_by_drop[iter->dropOff].push_back(iter->pickUp);
        }
    }

    unsigned start_p;
    coord start_p_coord;
    vector <value> result;

    unsigned i = 0;
    vector < TSPNodeInfo > all_posible_start;
    vector <unsigned> temp_path;
    vector <unsigned> not_picked_temp(not_picked);
    boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > pickUps_r_temp(pickUps_r);
    do {
        result.clear();
        int p = i % (depots.size());
        start_p = depots[p];
        start_p_coord = itsVec[start_p].cartesian_p;
        if (i < depots.size()) {
            pickUps_r_temp.query(boost::geometry::index::nearest(point(start_p_coord.first, start_p_coord.second), 1), back_inserter(result));
            temp_path = find_path_between_intersections(start_p, result[0].second, turn_penalty);
            //  double time = compute_path_travel_time(temp_path, turn_penalty);
            TSPNodeInfo info(result[0].second, temp_path, result[0]);
            all_posible_start.push_back(info);

            //     pickUps_r_temp.remove(result[0]);
        } else {
            pickUps_r_temp.query(boost::geometry::index::nearest(point(start_p_coord.first, start_p_coord.second), deliveries.size() / 2), back_inserter(result));
            unsigned ran_pick = rand() % (deliveries.size() / 2);
            temp_path = find_path_between_intersections(start_p, result[ran_pick].second, turn_penalty);
            TSPNodeInfo info(result[ran_pick].second, temp_path, result[ran_pick]);
            all_posible_start.push_back(info);
        }
        i++;
    } while (i <= 7 && i < deliveries.size());

    vector < vector <unsigned> > multi_start_path;

    for (unsigned i = 0; i < all_posible_start.size(); i++) {
        vector <unsigned> emp_vec;
        multi_start_path.push_back(emp_vec);
    }

    const unsigned loop_size = all_posible_start.size();

#ifdef testTime
    end = clock();
    elapsed_secs = double (end - begin) / CLOCKS_PER_SEC;
    std::cout << "multistart took " << elapsed_secs << " seconds." << endl;
    begin = end;
#endif


#pragma omp parallel for //num_threads(4)

    for (unsigned multi_start_iter = 0; multi_start_iter < loop_size; multi_start_iter++) {

        if (all_posible_start[multi_start_iter].path_to_it.empty()) {
            //no way to get from the depot to its closest pick up
            continue;
        } else {

            //compute all paths

            //make a copy of all the structure needed for threading
            boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > depos_r_sub(depos_r);

            boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > pickUps_r_sub(pickUps_r);

            boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> > dropOffs_r_sub(dropOffs_r);

            unordered_map <unsigned, vector <unsigned> > get_pickup_by_drop_sub = get_pickup_by_drop;
            vector <unsigned> not_picked_sub = not_picked;
            vector <unsigned> not_dropped_sub = not_dropped;
            unsigned start_p_sub;
            coord start_p_coord_sub;
            vector <value> result_sub;

            //needed to construct and modify the path
            vector <unsigned> ultimate_path, begin_bit, end_bit;
            vector <unsigned> nodes_sequence;

            //insert the nodes in a greedy way from the 
            ultimate_path = all_posible_start[multi_start_iter].path_to_it;
            begin_bit = ultimate_path;

            start_p_sub = all_posible_start[multi_start_iter].node_id;
            start_p_coord_sub = itsVec [all_posible_start[multi_start_iter].node_id].cartesian_p;

            auto temp_erase = find(not_picked_sub.begin(), not_picked_sub.end(), start_p_sub);
            not_picked_sub.erase(temp_erase);
            pickUps_r_sub.remove(all_posible_start[multi_start_iter].rtree_info);

            //got the first delivery position, now iterate greedily
            while (!not_dropped_sub.empty() || !not_picked_sub.empty()) {
                int closest_pick_id;
                double pick_up_distance;
                vector <value> closest_pick;


                if (!not_picked_sub.empty()) {

                    pickUps_r_sub.query(boost::geometry::index::nearest(point(start_p_coord_sub.first,
                            start_p_coord_sub.second), 1), back_inserter(closest_pick));
                    closest_pick_id = closest_pick[0].second;
                    pick_up_distance = find_cartesian_distance_squared(itsVec[start_p_sub].cartesian_p, itsVec[closest_pick_id].cartesian_p);

                } else {
                    closest_pick_id = -1;
                    pick_up_distance = numeric_limits<double>::max();
                }

                //find the closest legal drop off it can go
                vector <value> cloest_drop;
                int closest_drop_id;
                double drop_off_distance;
                //query 
                dropOffs_r_sub.query(boost::geometry::index::nearest(point(start_p_coord_sub.first, start_p_coord_sub.second), 1),
                        back_inserter(cloest_drop));
                //find is the drop up is picked up 
                bool all_picked_up = true;

                for (auto iter = get_pickup_by_drop_sub[cloest_drop[0].second].begin();
                        iter != get_pickup_by_drop_sub[cloest_drop[0].second].end(); iter++) {

                    auto if_picked = find(not_picked_sub.begin(), not_picked_sub.end(), *iter);
                    if (if_picked != not_picked_sub.end()) {
                        all_picked_up = false;
                        break;
                    }
                }


                //if its picked up (not in the not picked up line)
                if (all_picked_up) {
                    closest_drop_id = cloest_drop[0].second;
                    drop_off_distance = find_cartesian_distance_squared(itsVec[start_p_sub].cartesian_p, itsVec[closest_drop_id].cartesian_p);

                } else {
                    closest_drop_id = -1;
                    drop_off_distance = numeric_limits<double>::max();
                }
                //at this point, I have the closest pick up position, which is always valid when 
                //there is something to pick up
                //and the closest drop off, if there exists a legal one, now need to compare and decide where to go

                //pick up seems better
                if (drop_off_distance > pick_up_distance) {
                    pre_computed_path_key temp_key(start_p_sub, closest_pick_id);
                    vector <unsigned> temp_path2 = pre_computed_street_segment_paths[temp_key].second;
                    //vector <unsigned> temp_path2 = find_path_between_intersections(start_p, closest_pick_id, turn_penalty);
                    //    if (temp_path2.empty())
                    //       return temp_path2;

                    ultimate_path.insert(ultimate_path.end(), temp_path2.begin(), temp_path2.end());
                    nodes_sequence.push_back(start_p_sub);
                    pickUps_r_sub.remove(closest_pick[0]);

                    auto temp_erase2 = find(not_picked_sub.begin(), not_picked_sub.end(), closest_pick[0].second);
                    not_picked_sub.erase(temp_erase2);
                    //   cout<<"PICKED"<<endl;
                    start_p_sub = closest_pick_id;
                    start_p_coord_sub = itsVec[start_p_sub].cartesian_p;
                }//drop offs better
                else {
                    pre_computed_path_key temp_key(start_p_sub, closest_drop_id);
                    vector <unsigned> temp_path3 = pre_computed_street_segment_paths[temp_key].second;
                    //vector <unsigned> temp_path3 = find_path_between_intersections(start_p, closest_drop_id, turn_penalty);
                    //      if (temp_path3.empty())
                    //         return temp_path3;
                    nodes_sequence.push_back(start_p_sub);
                    ultimate_path.insert(ultimate_path.end(), temp_path3.begin(), temp_path3.end());
                    dropOffs_r_sub.remove(cloest_drop[0]);

                    auto temp_erase3 = find(not_dropped_sub.begin(), not_dropped_sub.end(), cloest_drop[0].second);
                    not_dropped_sub.erase(temp_erase3);
                    //cout<<"DROPPED"<<endl;
                    start_p_sub = closest_drop_id;
                    start_p_coord_sub = itsVec[start_p_sub].cartesian_p;

                }

            }
            nodes_sequence.push_back(start_p_sub);
            vector <unsigned> temp_path4;
            vector < vector <unsigned> > all_posible_backs;
            unsigned k = 0;

            do {
                bool legal = true;
                vector <value> final_dest_vec;
                depos_r_sub.query(boost::geometry::index::nearest(point(itsVec[start_p_sub].cartesian_p.first,
                        itsVec[start_p_sub].cartesian_p.second), 1), back_inserter(final_dest_vec));
                unsigned final_dest = final_dest_vec[0].second;

                temp_path4 = find_path_between_intersections(start_p_sub, final_dest, turn_penalty);
                depos_r_sub.remove(final_dest_vec[0]);
                all_posible_backs.push_back(temp_path4);
                k++;
            } while (k < depots.size());

            double best_cost = numeric_limits<double>::max();
            bool exist_back = false;

            for (auto iter = all_posible_backs.begin(); iter != all_posible_backs.end(); iter++) {
                if (!(iter->empty()) && best_cost > compute_path_travel_time(*iter, turn_penalty)) {
                    temp_path4 = *iter;
                    best_cost = compute_path_travel_time(*iter, turn_penalty);
                    exist_back = true;
                }
            }

            if (!exist_back) {
                continue;

            } else {
                ultimate_path.insert(ultimate_path.end(), temp_path4.begin(), temp_path4.end());
                end_bit = temp_path4;
                steady_clock::time_point time_entering_modify = steady_clock::now();
                duration<double> elapsed_before_modify = duration_cast<duration<double>>(time_entering_modify - func_start);
                double time_limit = MAX_T - elapsed_before_modify.count();
                path_modify(time_limit, ultimate_path, turn_penalty, deliveries, begin_bit, end_bit,
                        nodes_sequence, get_pickup_by_drop_sub, pre_computed_street_segment_paths);
                //               
                //                for (unsigned i = 0; i< nodes_sequence.size(); i++){
                //                    cout<< nodes_sequence[i]<<endl;
                //                }
                multi_start_path[multi_start_iter] = ultimate_path;
            }
        }



    }
#ifdef testTime
    end = clock();
    elapsed_secs = double (end - begin) / CLOCKS_PER_SEC;
    std::cout << "thread took  " << elapsed_secs << " seconds." << endl;
    begin = end;
#endif
    vector <unsigned> best_path;
    double best_time_multi = numeric_limits<double>::max();
    for (auto iter = multi_start_path.begin(); iter != multi_start_path.end(); iter++) {

        double cur_time = compute_path_travel_time(*iter, turn_penalty);

        if (cur_time < best_time_multi && !iter->empty()) {
            best_time_multi = cur_time;
            best_path = *iter;
        }
    }
#ifdef testTime
    end = clock();
    elapsed_secs = double (end - begin) / CLOCKS_PER_SEC;
    std::cout << "re turn  took " << elapsed_secs << " seconds." << endl;

#endif
   
    return best_path;

}

void path_modify(double time_limit, vector <unsigned> & path, double turn_penalty, const std::vector<DeliveryInfo>& deliveries,
        vector <unsigned> begin_bit, vector <unsigned> end_bit, vector <unsigned> &node_sequence,
        unordered_map <unsigned, vector <unsigned> > & get_pickup_by_drop,
        unordered_map <pre_computed_path_key, pair <double, vector <unsigned> >, pre_computed_path_hash, KeyEqual > & pre_computed_street_segment_paths) {

    auto start_time = chrono::high_resolution_clock::now();
    double cur_cost = compute_path_travel_time(path, turn_penalty);
    cur_cost -= compute_path_travel_time(begin_bit, turn_penalty);
    cur_cost -= compute_path_travel_time(end_bit, turn_penalty);

    vector <unsigned> pick_ups, drop_offs;
    for (auto i = deliveries.begin(); i != deliveries.end(); ++i) {
        pick_ups.push_back(i->pickUp);
        drop_offs.push_back(i->dropOff);
    }

    vector <unsigned> potential_path(path);
    vector <unsigned> new_sequence(node_sequence);

    priority_queue<k_opt_inside, vector<k_opt_inside>, compare_k_opt> k_opt_pq;

    //-----------newly added for simulated annealing
        
        const double max_temper = 1000000;
        const double dec = 0.9999;
        double temp = max_temper;
        
    //    
    //-----------newly added ends here
    unsigned two_op_bound =  node_sequence.size() -1;
startAgain:
    //for (unsigned mode_num = )
    
    bool can_improve = false;
    for (unsigned cut_begin = 1; cut_begin < node_sequence.size() - 2; cut_begin++) {

        for (unsigned cut_end = cut_begin + 1;
                (cut_end < cut_begin + two_op_bound ) &&  (cut_end < node_sequence.size() -1) ; cut_end++) {
            // cout<<"here"<<endl;
            auto cur_time = chrono::high_resolution_clock::now();
            auto wall_time = chrono::duration_cast < chrono::duration<double> >(cur_time - start_time);
            if (wall_time.count() > time_limit ){
                cur_cost = k_opt_pq.top().first;
                
                node_sequence = k_opt_pq.top().second;
                
                path = begin_bit;
                for (unsigned i = 0; i < new_sequence.size() - 1; i++) {

                    pre_computed_path_key temp_key(new_sequence[i], new_sequence[i + 1]);
                    vector <unsigned> sub_path = pre_computed_street_segment_paths[temp_key].second;
                    //vector <unsigned> sub_path = find_path_between_intersections(new_sequence[i], new_sequence[i + 1], turn_penalty

                    if (sub_path.empty() && new_sequence[i] != new_sequence[i + 1]) {
                        continue;
                    }
                    path.insert(path.end(), sub_path.begin(), sub_path.end());

                    //------------newly added
                    //                        temp *= dec;
                    //------------ends here

                }

                path.insert(path.end(), end_bit.begin(), end_bit.end());
                
               return;
            }
                

            new_sequence = two_op_swap(node_sequence, cut_begin, cut_end);
            double new_path_time = 0;
            if (can_swap(new_sequence, deliveries, get_pickup_by_drop, cut_begin, cut_end)) {
                //        cout<<"valid swap:"<<endl;
                potential_path = begin_bit;
                for (unsigned i = 0; i < new_sequence.size() - 1; i++) {

                    pre_computed_path_key temp_key(new_sequence[i], new_sequence[i + 1]);
                    vector <unsigned> sub_path = pre_computed_street_segment_paths[temp_key].second;
                    //vector <unsigned> sub_path = find_path_between_intersections(new_sequence[i], new_sequence[i + 1], turn_penalty);

                    new_path_time += pre_computed_street_segment_paths[temp_key].first;

                    if (sub_path.empty() && new_sequence[i] != new_sequence[i + 1]) {
                        continue;
                    }
                    potential_path.insert(potential_path.end(), sub_path.begin(), sub_path.end());

                    //------------newly added
                    //                        temp *= dec;
                    //------------ends here

                }

                potential_path.insert(potential_path.end(), end_bit.begin(), end_bit.end());
                //assign potential path here

            } else {
                //    cout<<"made a bad choice"<<endl;
                continue;
            }


            //|| ( (rand() % 1) <= exp((cur_cost - new_path_time)/temp) * RAND_MAX )

            double delta = cur_cost - new_path_time;
            if (cur_cost > new_path_time || ((double) rand() / (RAND_MAX)) <= exp((delta)/temp) ) {
            
           //     cout<<"Improved: "<< cur_cost- new_path_time<<endl;
                
                if (delta < 0){
                   k_opt_inside inside_queue = make_pair(cur_cost, node_sequence);
                   k_opt_pq.push(inside_queue);
                   cur_cost = new_path_time;
                path = potential_path;
                node_sequence = new_sequence;
                
               
                
                goto startAgain;
                }else{
                cur_cost = new_path_time;
                path = potential_path;
                node_sequence = new_sequence;
                can_improve = true;
                
                //                k_opt_inside inside_queue = make_pair(new_path_time, new_sequence);
                //                k_opt_pq.push(inside_queue);

                }
                //cout << "i found something better!!"<<new_path_time << endl;



            }
            
            
            
            
            
            if (wall_time.count() >0.7* time_limit  ){
                two_op_bound = 20;
                temp = 0;
            }else {
                two_op_bound = (node_sequence.size() -1) * (time_limit - wall_time.count())/time_limit;
                temp = max_temper * (time_limit - wall_time.count())/time_limit;
            }
        }


    }
    
    if (can_improve)
    goto startAgain;

    

//
//    if (!k_opt_pq.empty()) {
//
//        cur_cost = k_opt_pq.top().first;
//        node_sequence = k_opt_pq.top().second;
//
//        potential_path.clear();
//
//        potential_path = begin_bit;
//
//        for (unsigned i = 0; i < node_sequence.size() - 1; i++) {
//
//            pre_computed_path_key temp_key(node_sequence[i], node_sequence[i + 1]);
//            vector <unsigned> sub_path = pre_computed_street_segment_paths[temp_key].second;
//
//            potential_path.insert(potential_path.end(), sub_path.begin(), sub_path.end());
//
//        }
//        potential_path.insert(potential_path.end(), end_bit.begin(), end_bit.end());
//
//        path = potential_path;
//
//        k_opt_pq = priority_queue <k_opt_inside, vector<k_opt_inside>, compare_k_opt>();
//
//        auto cur_time = chrono::high_resolution_clock::now();
//        auto wall_time = chrono::duration_cast < chrono::duration<double> >(cur_time - start_time);
//
//        if (wall_time.count() <= time_limit)
//            goto startAgain;
//
//
//
//    }
//

    if (node_sequence.size() >= 25) {

startAgainThreeOpt:


        for (unsigned cut_begin = 1; cut_begin < node_sequence.size() - 24; cut_begin++) {

            for (unsigned cut_end1 = cut_begin + 1; cut_end1 < cut_begin + 23; cut_end1++) {

                for (unsigned cut_end2 = cut_end1 + 1; cut_end2 < cut_end1 + 22; cut_end2++) {

                    auto cur_time = chrono::high_resolution_clock::now();
                    auto wall_time = chrono::duration_cast < chrono::duration<double> >(cur_time - start_time);

                    if (wall_time.count() > time_limit)
                        return;

                    new_sequence = three_opt_swap(node_sequence, cut_begin, cut_end1, cut_end2);
                    double new_path_time = 0;
                    if (can_swap(new_sequence, deliveries, get_pickup_by_drop)) {

                        potential_path = begin_bit;
                        for (unsigned i = 0; i < new_sequence.size() - 1; i++) {

                            pre_computed_path_key temp_key(new_sequence[i], new_sequence[i + 1]);
                            vector <unsigned> sub_path = pre_computed_street_segment_paths[temp_key].second;
                            new_path_time += pre_computed_street_segment_paths[temp_key].first;

                            if (sub_path.empty() && new_sequence[i] != new_sequence[i + 1]) {
                                continue;
                            }
                            potential_path.insert(potential_path.end(), sub_path.begin(), sub_path.end());

                        }

                        potential_path.insert(potential_path.end(), end_bit.begin(), end_bit.end());
                        //assign potential path here

                    } else {
                        continue;
                    }



                    if (cur_cost > new_path_time) {
                        cur_cost = new_path_time;

                        path = potential_path;

                        node_sequence = new_sequence;


                        //cout << "three_opt found something better!!"<<new_path_time << endl;


                        goto startAgainThreeOpt;
                    }
                }
            }
        }
    }


    if (node_sequence.size() >= THREE_OPT_BOUND + 5) {

startAgainThreeOptAdvanced:


        for (unsigned cut_begin = 1; cut_begin < node_sequence.size() - THREE_OPT_BOUND - 1; cut_begin++) {

            for (unsigned cut_end1 = cut_begin + 1; cut_end1 < cut_begin + THREE_OPT_BOUND - 3; cut_end1++) {

                for (unsigned cut_end2 = cut_end1 + 1; cut_end2 < cut_begin + THREE_OPT_BOUND - 2; cut_end2++) {

                    for (unsigned cut_end3 = cut_end2 + 1; cut_end3 < cut_begin + THREE_OPT_BOUND - 1; cut_end3++) {
                        auto cur_time = chrono::high_resolution_clock::now();
                        auto wall_time = chrono::duration_cast < chrono::duration<double> >(cur_time - start_time);

                        if (wall_time.count() > time_limit)
                            return;

                        for (int mode = 0; mode < 3; mode++) {

                            new_sequence = three_opt_swap_advanced(node_sequence, cut_begin, cut_end1, cut_end2, cut_end3, mode);
                            double new_path_time = 0;
                            if (can_swap(new_sequence, deliveries, get_pickup_by_drop)) {

                                potential_path = begin_bit;
                                for (unsigned i = 0; i < new_sequence.size() - 1; i++) {

                                    pre_computed_path_key temp_key(new_sequence[i], new_sequence[i + 1]);
                                    vector <unsigned> sub_path = pre_computed_street_segment_paths[temp_key].second;

                                    new_path_time += pre_computed_street_segment_paths[temp_key].first;
                                    if (sub_path.empty() && new_sequence[i] != new_sequence[i + 1]) {
                                        continue;
                                    }
                                    potential_path.insert(potential_path.end(), sub_path.begin(), sub_path.end());

                                }

                                potential_path.insert(potential_path.end(), end_bit.begin(), end_bit.end());
                                //assign potential path here

                            } else {
                                continue;
                            }



                            if (cur_cost > new_path_time) {
                                cur_cost = new_path_time;

                                path = potential_path;

                                node_sequence = new_sequence;


                                //cout << "three_opt found something better!!"<<new_path_time << endl;


                                goto startAgainThreeOptAdvanced;
                            }
                        }
                    }
                }
            }
        }
    }




}

bool can_swap(vector <unsigned> & node_sequence, const std::vector<DeliveryInfo>& deliveries,
        unordered_map <unsigned, vector <unsigned> > & get_pickup_by_drop) {

    vector <unsigned> pick, drop;
    for (auto iter = deliveries.begin(); iter != deliveries.end(); iter++) {
        pick.push_back(iter->pickUp);
        drop.push_back(iter->dropOff);
    }

    //#pragma omp parallel for
    for (auto node = node_sequence.begin(); node < node_sequence.end(); node++) {

        auto drop_node = find(drop.begin(), drop.end(), *node);
        //interested in dropoff
        if (drop_node != drop.end()) {

            vector <unsigned> should_picked = get_pickup_by_drop[*drop_node];
            //loops though to find misplacement in the pick up and drop off order

            for (auto iter = should_picked.begin(); iter < should_picked.end(); iter++) {

                if (find(node_sequence.begin(), node_sequence.end(), *drop_node) <
                        find(node_sequence.begin(), node_sequence.end(), *iter)) {
                    //#pragma omp critical 
                    return false;
                    //legal =false;
                    //  break;
                }

            }

        }
    }

    return true;
}

bool can_swap(vector <unsigned> & node_sequence, const std::vector<DeliveryInfo>& deliveries,
        unordered_map <unsigned, vector <unsigned> > & get_pickup_by_drop, unsigned cut_begin, unsigned cut_end) {

    vector <unsigned> pick, drop;
    for (auto iter = deliveries.begin(); iter != deliveries.end(); iter++) {
        pick.push_back(iter->pickUp);
        drop.push_back(iter->dropOff);
    }

    //#pragma omp parallel for
    for (unsigned node = cut_begin; node < cut_end + 1; node++) {

        auto drop_node = find(drop.begin(), drop.end(), node_sequence[node]);
        //interested in dropoff
        if (drop_node != drop.end()) {

            vector <unsigned> should_picked = get_pickup_by_drop[*drop_node];
            //loops though to find misplacement in the pick up and drop off order

            for (auto iter = should_picked.begin(); iter < should_picked.end(); iter++) {

                if (find(node_sequence.begin(), node_sequence.end(), *drop_node) <
                        find(node_sequence.begin(), node_sequence.end(), *iter)) {
                    //#pragma omp critical 
                    return false;
                    //legal =false;
                    //  break;
                }

            }

        }
    }

    return true;
}

//vector <unsigned> two_op_swap(vector <unsigned> &node_sequence, unsigned cut_begin, unsigned cut_end,
//        unsigned mode) {
//
//    vector <unsigned> part1, part2, part3;
//
//
//    for (unsigned i = 0; i < node_sequence.size(); i++) {
//        if (i < cut_begin)
//            part1.push_back(node_sequence[i]);
//        else if (i >= cut_begin && i <= cut_end)
//            part2.push_back(node_sequence[i]);
//        else
//            part3.push_back(node_sequence[i]);
//    }
//
//    if (mode == 0) {
//        reverse(part2.begin(), part2.end());
//
//        part1.insert(part1.end(), part2.begin(), part2.end());
//        part1.insert(part1.end(), part3.begin(), part3.end());
//
//        return part1;
//    } else {
//
//        reverse(part1.begin(), part1.end());
//        reverse(part3.begin(), part3.end());
//        part1.insert(part1.end(), part2.begin(), part2.end());
//        part1.insert(part1.end(), part3.begin(), part3.end());
//        return part1;
//        //        if (mode == 1 && (cut_end - cut_begin) > 4) {
//        //            unsigned further_divide = (cut_end - cut_begin) / 2;
//        //
//        //            vector <unsigned> part2_sub1, part2_sub2;
//        //            for (unsigned part2_ele = 0; part2_ele < part2.size(); part2_ele++) {
//        //                if (part2_ele < further_divide)
//        //                    part2_sub1.push_back(part2[part2_ele]);
//        //                else
//        //                    part2_sub2.push_back(part2[part2_ele]);
//        //            }
//        //            reverse(part2_sub1.begin(), part2_sub1.end());
//        //            reverse(part2_sub2.begin(), part2_sub2.end());
//        //
//        //            part1.insert(part1.end(), part2_sub1.begin(), part2_sub1.end());
//        //            part1.insert(part1.end(), part2_sub2.begin(), part2_sub2.end());
//        //            part1.insert(part1.end(), part3.begin(), part3.end());
//        //            return part1;
//        //        } else {
//        //            unsigned further_divide = (cut_end - cut_begin) / 2;
//        //
//        //            vector <unsigned> part2_sub1, part2_sub2;
//        //            for (unsigned part2_ele = 0; part2_ele < part2.size(); part2_ele++) {
//        //                if (part2_ele < further_divide)
//        //                    part2_sub1.push_back(part2[part2_ele]);
//        //                else
//        //                    part2_sub2.push_back(part2[part2_ele]);
//        //            }
//        //            reverse(part2_sub1.begin(), part2_sub1.end());
//        //            reverse(part2_sub2.begin(), part2_sub2.end());
//        //
//        //            part1.insert(part1.end(), part2_sub2.begin(), part2_sub2.end());
//        //            part1.insert(part1.end(), part2_sub1.begin(), part2_sub1.end());
//        //            part1.insert(part1.end(), part3.begin(), part3.end());
//        //
//        //        }
//
//    }
//
//}
//
//vector <unsigned> three_opt_swap(vector <unsigned> node_sequence, unsigned cut_begin, unsigned cut_end1, unsigned cut_end2) {
//    
//    vector <unsigned> part1, part2, part3, part4;
//    
//    for (unsigned i = 0; i < node_sequence.size(); i++) {
//        if (i < cut_begin)
//            part1.push_back(node_sequence[i]);
//        else if (i >= cut_begin && i < cut_end1)
//            part2.push_back(node_sequence[i]);
//        else if (i >= cut_end1 && i < cut_end2)
//            part3.push_back(node_sequence[i]);
//        else
//            part4.push_back(node_sequence[i]);
//    }
//
//    part1.insert(part1.end(), part3.begin(), part3.end());
//    part1.insert(part1.end(), part2.begin(), part2.end());
//    part1.insert(part1.end(), part4.begin(), part4.end());
//
//    return part1;
//    
//}

vector <unsigned> two_op_swap(vector <unsigned> & node_sequence, unsigned cut_begin, unsigned cut_end) {

    vector <unsigned> part1, part2, part3;
    for (unsigned i = 0; i < node_sequence.size(); i++) {
        if (i < cut_begin)
            part1.push_back(node_sequence[i]);
        else if (i >= cut_begin && i <= cut_end)
            part2.push_back(node_sequence[i]);
        else
            part3.push_back(node_sequence[i]);
    }
    reverse(part2.begin(), part2.end());

    part1.insert(part1.end(), part2.begin(), part2.end());
    part1.insert(part1.end(), part3.begin(), part3.end());

    return part1;
}

vector <unsigned> three_opt_swap(vector <unsigned> node_sequence, unsigned cut_begin, unsigned cut_end1, unsigned cut_end2) {

    vector <unsigned> part1, part2, part3, part4;

    for (unsigned i = 0; i < node_sequence.size(); i++) {
        if (i < cut_begin)
            part1.push_back(node_sequence[i]);
        else if (i >= cut_begin && i < cut_end1)
            part2.push_back(node_sequence[i]);
        else if (i >= cut_end1 && i < cut_end2)
            part3.push_back(node_sequence[i]);
        else
            part4.push_back(node_sequence[i]);
    }

    part1.insert(part1.end(), part3.begin(), part3.end());
    part1.insert(part1.end(), part2.begin(), part2.end());
    part1.insert(part1.end(), part4.begin(), part4.end());

    return part1;

}

vector <unsigned> three_opt_swap_advanced(vector <unsigned> node_sequence, unsigned cut_begin, unsigned cut_end1, unsigned cut_end2, unsigned cut_end3, int mode) {

    vector <unsigned> part1, part2, part3, part4, part5;





    for (unsigned i = 0; i < node_sequence.size(); i++) {
        if (i < cut_begin)
            part1.push_back(node_sequence[i]);
        else if (i >= cut_begin && i < cut_end1) {
            part2.push_back(node_sequence[i]);
        } else if (i >= cut_end1 && i < cut_end2)
            part3.push_back(node_sequence[i]);
        else if (i >= cut_end2 && i < cut_end3) {
            part4.push_back(node_sequence[i]);
        } else {
            part5.push_back(node_sequence[i]);
        }

    }

    if (mode == 0) {
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 1) {
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part5.begin(), part5.end());

    } else if (mode == 4) {
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 3) {
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 2) {
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 5) {
        reverse(part2.begin(), part2.end());
        reverse(part3.begin(), part3.end());
        reverse(part4.begin(), part4.end());

        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 6) {
        reverse(part2.begin(), part2.end());
        reverse(part3.begin(), part3.end());
        reverse(part4.begin(), part4.end());

        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 7) {
        reverse(part2.begin(), part2.end());
        reverse(part3.begin(), part3.end());
        reverse(part4.begin(), part4.end());

        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    } else if (mode == 8) {
        reverse(part2.begin(), part2.end());
        reverse(part3.begin(), part3.end());
        reverse(part4.begin(), part4.end());

        part1.insert(part1.end(), part4.begin(), part4.end());
        part1.insert(part1.end(), part2.begin(), part2.end());
        part1.insert(part1.end(), part3.begin(), part3.end());
        part1.insert(part1.end(), part5.begin(), part5.end());
    }
    return part1;

}