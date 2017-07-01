/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Scale_dict.h
 * Author: liuhao22
 *
 * Created on February 24, 2017, 3:47 PM
 */

#ifndef SCALE_DICT_H
#define SCALE_DICT_H

#include "interface.h"
#include <vector>
#include <unordered_map>

static std::vector< std::unordered_map<std::string, float>* > SCALE_DICT;

void initialize_scaling();

float getScaleFactor(int ZOOM_LV, std::string segLv);

void close_Scale_dict();

#endif /* SCALE_DICT_H */

