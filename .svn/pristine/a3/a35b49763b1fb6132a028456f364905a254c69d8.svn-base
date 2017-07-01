/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
//#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry/index/rtree.hpp>
#include <utility>
//just for output
//#include <boost/foreach.hpp>
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef std::pair<point, unsigned> value; 

void Boost (){

    double d1 = 10.2;
    double d2 = 10.5;
    
    bgi::rtree<value, bgi::quadratic<16>> my_rtree; 
    
    
    my_rtree.insert(std::make_pair( point(d1,d2) , 3));
    
    cout<<"hi";
    
}