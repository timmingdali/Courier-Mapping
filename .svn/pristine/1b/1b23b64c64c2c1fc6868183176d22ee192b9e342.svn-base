/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Scale_dict.h"

using namespace std;

// Hard coding the scaling dictionary
void initialize_scaling() {
    
    /* Scale level 1 */
    /* For zoom level > 0.7 */
    unordered_map<std::string, float>* lv_1_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_1_dict_ptr)["motorway"] = 1.2;
    (*lv_1_dict_ptr)["trunk"] = 1.2;
    (*lv_1_dict_ptr)["primary"] = 1;
    (*lv_1_dict_ptr)["secondary"] = 0;
    (*lv_1_dict_ptr)["tertiary"] = 0;
    (*lv_1_dict_ptr)["residential"] = 0;
    (*lv_1_dict_ptr)["other"] = 0;
    
    /* Scale level 2 */
    /* For zoom 0.7 > level > 0.4 */
    unordered_map<std::string, float>* lv_2_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_2_dict_ptr)["motorway"] = 1.2;
    (*lv_2_dict_ptr)["trunk"] = 1.2;
    (*lv_2_dict_ptr)["primary"] = 1.2;
    (*lv_2_dict_ptr)["secondary"] = 1;
    (*lv_2_dict_ptr)["tertiary"] = 0.8;
    (*lv_2_dict_ptr)["residential"] = 0;
    (*lv_2_dict_ptr)["other"] = 0;
    
    /* Scale level 3 */
    /* For zoom 0.4 > level > 0.25 */
    unordered_map<std::string, float>* lv_3_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_3_dict_ptr)["motorway"] = 1;
    (*lv_3_dict_ptr)["trunk"] = 1;
    (*lv_3_dict_ptr)["primary"] = 1;
    (*lv_3_dict_ptr)["secondary"] = 0.8;
    (*lv_3_dict_ptr)["tertiary"] = 0.6;
    (*lv_3_dict_ptr)["residential"] = 0;
    (*lv_3_dict_ptr)["other"] = 0;
    
    /* Scale level 4 */
    /* For zoom 0.25 > level > 0.15 */
    unordered_map<std::string, float>* lv_4_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_4_dict_ptr)["motorway"] = 0.7;
    (*lv_4_dict_ptr)["trunk"] = 0.7;
    (*lv_4_dict_ptr)["primary"] = 0.7;
    (*lv_4_dict_ptr)["secondary"] = 0.6;
    (*lv_4_dict_ptr)["tertiary"] = 0.5;
    (*lv_4_dict_ptr)["residential"] = 0.4;
    (*lv_4_dict_ptr)["other"] = 0;
    
    /* Scale level 5 */
    /* For zoom 0.15 > level > 0.1 */
    unordered_map<std::string, float>* lv_5_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_5_dict_ptr)["motorway"] = 0.6;
    (*lv_5_dict_ptr)["trunk"] = 0.6;
    (*lv_5_dict_ptr)["primary"] = 0.6;
    (*lv_5_dict_ptr)["secondary"] = 0.5;
    (*lv_5_dict_ptr)["tertiary"] = 0.4;
    (*lv_5_dict_ptr)["residential"] = 0.3;
    (*lv_5_dict_ptr)["other"] = 0.25;
    
    /* Scale level 6 */
    /* For zoom 0.1 > level > 0.01 */
    unordered_map<std::string, float>* lv_6_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_6_dict_ptr)["motorway"] = 0.27;
    (*lv_6_dict_ptr)["trunk"] = 0.27;
    (*lv_6_dict_ptr)["primary"] = 0.27;
    (*lv_6_dict_ptr)["secondary"] = 0.25;
    (*lv_6_dict_ptr)["tertiary"] = 0.22;
    (*lv_6_dict_ptr)["residential"] = 0.2;
    (*lv_6_dict_ptr)["other"] = 0.15;    
    
    /* Scale level 7*/
    /* For zoom <0.01*/
    unordered_map<std::string, float>* lv_7_dict_ptr = new unordered_map<std::string, float>;
    
    (*lv_7_dict_ptr)["motorway"] = 0.2;
    (*lv_7_dict_ptr)["trunk"] = 0.2;
    (*lv_7_dict_ptr)["primary"] = 0.2;
    (*lv_7_dict_ptr)["secondary"] = 0.18;
    (*lv_7_dict_ptr)["tertiary"] = 0.16;
    (*lv_7_dict_ptr)["residential"] = 0.14;
    (*lv_7_dict_ptr)["other"] = 0.1;

    
    SCALE_DICT.push_back(lv_1_dict_ptr);
    SCALE_DICT.push_back(lv_2_dict_ptr);
    SCALE_DICT.push_back(lv_3_dict_ptr);
    SCALE_DICT.push_back(lv_4_dict_ptr);
    SCALE_DICT.push_back(lv_5_dict_ptr);
    SCALE_DICT.push_back(lv_6_dict_ptr);
    SCALE_DICT.push_back(lv_7_dict_ptr);
    
}

float getScaleFactor(int ZOOM_LV, std::string segLv) {
    if ((*SCALE_DICT[ZOOM_LV]).find(segLv) == (*SCALE_DICT[ZOOM_LV]).end())
        return (*SCALE_DICT[ZOOM_LV]).at("other");
    else
        return (*SCALE_DICT[ZOOM_LV]).at(segLv);
}

void close_Scale_dict(){
    for (auto iter = SCALE_DICT.begin(); iter != SCALE_DICT.end(); iter++ )
    {
        delete (*iter);
         
    }
    SCALE_DICT.clear();
}