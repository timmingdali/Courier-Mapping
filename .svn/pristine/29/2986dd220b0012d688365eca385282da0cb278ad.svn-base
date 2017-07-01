#include "API_module.h"
using namespace std;

/*
auto DECREF = [](PyObject **pPtr) {
for (size_t index = 0; pPtr[index]; index++)
Py_DECREF(pPtr[index]); };
*/

PyObject * json_dict_from_python() {
        
    
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pList;
    
    PyRun_SimpleString("import os, sys\n"
                       "print os.getcwd()\n");
    
    pModule = PyImport_Import(PyString_FromString("API_Script"));
    
    if (pModule == NULL)
    {
        PyErr_Print();
        std::exit(1);
    }
    pFunc = PyObject_GetAttrString(pModule, "return_json_dict");


    pArgs = PyTuple_New(0);

    pList = PyObject_CallObject(pFunc, pArgs);

    // Decrease reference count to release memory
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);

    return pList;
}

void convertPythonDictToVector(PyObject* JsonDict) {
    
	if (!PyList_Check(JsonDict))
            assert(cout << "Wrong PyObject type" << endl);

	Py_ssize_t  listSize = PyList_Size(JsonDict);

	PyObject * pDict, *pDictAttr, *pDictValue;

	// Lambda function doing conversion from Python to C++
	auto getValue = [&pDictAttr, &pDictValue, &pDict](const char* attr) {
		pDictAttr = PyString_FromString(attr);
		pDictValue = PyDict_GetItem(pDict, pDictAttr);
		string temp = PyString_AsString(pDictValue);
		//Py_DECREF(pDictAttr);
		//Py_DECREF(pDictValue);
		return temp;};

	for (Py_ssize_t index = 0; index < listSize; index++)
	{
		pDict = PyList_GetItem(JsonDict, index);

		StreetAPIClt temp;

		temp.roadName = getValue("Road");

		temp.description = getValue("Description");
		
		temp.latitude = stod(getValue("Latitude"));

		temp.lontitude = stod(getValue("Longitude"));

		APIvec.push_back(temp);

		Py_DECREF(pDict);
	}
}

void APImerger(){
    
}

void APIInitializer()
{
    setenv("PYTHONPATH", ".", 0);
    
    Py_Initialize();
    
    PyObject * theList;
    
    theList = json_dict_from_python();

    convertPythonDictToVector(theList);

    cout << APIvec.size() << endl;

    Py_DECREF(theList);

    Py_Finalize();
}

vector<StreetAPIClt> * getAPI(){
    return &(APIvec);
}