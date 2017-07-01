/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   API module.h
 * Author: liuhao22
 *
 * Created on February 20, 2017, 3:04 PM
 */

#ifndef API_MODULE_H
#define API_MODULE_H

#include <python2.7/Python.h>
#include "load_map.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
    string roadName;
    string description;
    double latitude;
    double lontitude;
} StreetAPIClt;

static vector<StreetAPIClt> APIvec;

PyObject * json_dict_from_python();

void convertPythonDictToVector(PyObject* JsonDict);

void APIInitializer();

void APImerger();

vector<StreetAPIClt> * getAPI();

#endif /* API_MODULE_H */

