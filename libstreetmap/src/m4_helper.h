/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_helper.h
 * Author: lixin39
 *
 * Created on March 26, 2017, 9:59 PM
 */

#ifndef M4_HELPER_H
#define M4_HELPER_H
#include <chrono>
#include <vector>
#include "load_map.h"



struct pre_computed_path_key{
    
    unsigned intersection1;
    unsigned intersection2;
    pre_computed_path_key() :
    intersection1(0), intersection2(0){   
    };
    pre_computed_path_key(unsigned intersection_start, unsigned intersection_end):
    intersection1(intersection_start), intersection2(intersection_end) {
    };
    
};

struct pre_computed_path_hash{
    std:: size_t operator() (const pre_computed_path_key& k) const
    {
        return ( std::hash<unsigned int>()(k.intersection1) + std::hash<unsigned int>()(k.intersection2) );
    } 
};

struct KeyEqual{
    bool operator () (const pre_computed_path_key& lhs, const pre_computed_path_key& rhs) const
    {
        return lhs.intersection1==rhs.intersection1 && lhs.intersection2==rhs.intersection2;
    }
};

struct Remained_Nodes {
    unsigned nodeid;
    std::vector <unsigned> path;
    double huristic_weight;
    double actual_cost;

};

typedef std::pair <unsigned, unsigned> drop_pick_pair;

struct TSPNodeInfo {
    unsigned node_id;
    std::vector <unsigned> path_to_it;
    value rtree_info;
    double travel_time;

    TSPNodeInfo(unsigned _id, std::vector <unsigned> _path, value _rtree_value) : node_id(_id), path_to_it(_path), rtree_info(_rtree_value){
    };
};
        void path_modify(double time_limit, std::vector<unsigned>& path, double turn_penalty, const std::vector<DeliveryInfo>& deliveries,
       const std::vector <unsigned> begin_bit, const std::vector <unsigned> end_bit, std::vector <unsigned>& node_sequence, 
                std::unordered_map <unsigned, std::vector <unsigned> > & get_pickup_by_drop,
                std::unordered_map <pre_computed_path_key, std:: pair < double, std::vector <unsigned> >, pre_computed_path_hash, KeyEqual > & pre_computed_street_segment_paths);
      
        bool can_swap(std::vector <unsigned> & node_sequence, const std::vector<DeliveryInfo>& deliveries,
         std::unordered_map <unsigned, std::vector <unsigned> > & get_pickup_by_drop, unsigned cut_begin, unsigned cut_end);
        
         bool can_swap(std::vector <unsigned> & node_sequence, const std::vector<DeliveryInfo>& deliveries,
         std::unordered_map <unsigned, std::vector <unsigned> > & get_pickup_by_drop);
        
        
        std::vector <unsigned> two_op_swap(std::vector <unsigned> & node_sequence, unsigned cut_begin, 
                unsigned cut_end);
        std::vector <unsigned> three_opt_swap(std::vector <unsigned> node_sequence, unsigned cut_begin, unsigned cut_end1, unsigned cut_end2);
        
        std:: vector <unsigned> three_opt_swap_advanced(std::vector <unsigned> node_sequence, unsigned cut_begin, unsigned cut_end1, unsigned cut_end2, unsigned cut_end3, int mode);

#endif /* M4_HELPER_H */

