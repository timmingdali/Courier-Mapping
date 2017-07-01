/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TSP.h
 * Author: liuhao22
 *
 * Created on March 23, 2017, 6:35 PM
 */

#ifndef TSP_H
#define TSP_H

#define itsVec (*getPtrToIntersectSet())

#include "load_map.h"
#include "m3.h"
#include "m4.h"
#include <vector>
#include <unordered_map>

class TSP_node;


struct TSP_edge{
    bool valid;
    double weight;
    
    TSP_node* desti; 
    
    TSP_edge(bool _valid, double _weight, TSP_node* _desti):
        valid(_valid), weight(_weight), desti(_desti) {};
    
} ;

class TSP_node {
public:
    TSP_node(unsigned interxId);
    
    void add_edges(TSP_edge edge);
    
private:
    unsigned selfId;
    
    std::vector<TSP_node*> edges;
};

class TSP {
public:
    TSP(DeliveryInfo &deliveries);
    
private:
    
    std::vector<TSP_node> graph;
    
    // used to store paths figured
    std::unordered_map<unsigned, TSP_node > nodes;
    
    // used to store all pick up spots
    std::vector<unsigned> pickUps;
    
    std::vector<unsigned> dropOffs;
    
    // Mapping two natural numbers into 1, for purposes of storing calculated path
    unsigned paring(unsigned first, unsigned second);
    
    
};

/*************************** Helper functions declarations *********************/


#endif /* TSP_H */

