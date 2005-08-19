/**
 * @file pycantera.cpp
 *
 * This is the main file for the Python interface module.
 *
 */

// turn off warnings about long names under Windows
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "../../src/config.h"

#include "Python.h"

#ifdef HAS_NUMERIC
#include "Numeric/arrayobject.h"
#else
#include "numarray/arrayobject.h"
#endif

#include "ct.h"
#include "ctxml.h"
#include "ctsurf.h"
#include "ctbdry.h"
#include "ctrpath.h"
#include "ctreactor.h" 
#include "ctfunc.h"
#include "ctonedim.h"
#include "ctmultiphase.h"

#include <iostream>
using namespace std;

//  constants defined in the module
static PyObject *ErrorObject;

// local includes
#include "pyutils.h"

#include "ctphase_methods.cpp"
#include "ctthermo_methods.cpp"
#include "ctkinetics_methods.cpp"  
#include "cttransport_methods.cpp"
#include "ctxml_methods.cpp"
#include "ctfuncs.cpp"
#include "ctsurf_methods.cpp"
//#include "ctbndry_methods.cpp"
#include "ctrpath_methods.cpp"
#include "ctreactor_methods.cpp"
#include "ctfunc_methods.cpp"
#include "ctonedim_methods.cpp"
#include "ctmultiphase_methods.cpp"

#ifdef INCL_USER_PYTHON
#include "ctuser.h"
#include "ctuser_methods.cpp"
#endif


#include "methods.h"
#include "pylogger.h"
//#include "../../src/global.h"
extern "C" {

    /* Initialization function for the module */

    DL_EXPORT(void) init_cantera(void)
    {
        PyObject *m, *d;

        /* Initialize the type of the new type object here; doing it here
         * is required for portability to Windows without requiring C++. */
 
        /* Create the module and add the functions */
        m = Py_InitModule("_cantera", ct_methods);
        import_array();
        Cantera::Logger* pylog = new Cantera::Py_Logger;
        setLogWriter(pylog);
 
        /* Add some symbolic constants to the module */
        d = PyModule_GetDict(m);
        ErrorObject = PyErr_NewException("cantera.error", NULL, NULL);
        PyDict_SetItemString(d, "error", ErrorObject);
#ifdef HAS_NUMERIC
        PyDict_SetItemString(d, "nummod",PyString_FromString("Numeric"));
#else
        PyDict_SetItemString(d, "nummod",PyString_FromString("numarray"));
#endif
    }

}

