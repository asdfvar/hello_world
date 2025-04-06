// theExtension.cpp
//
/////////////////////////////////////////////////////////////////
//
// Primary module for extension.
// 

/////////////////////////////////////////////////////////////////

// Numpy uses a variable PyArray_API to define API methods; this is initialised by calling import_array.
// By default this initialises to a static variable, so is only available in the current module. 
// Defining PY_ARRAY_UNIQUE_SYMBOL indicates that the variable should be not be declared
// as static, so it can be linked to from other .cpp files. Other .cpp files must define NO_IMPORT_ARRAY
// which means PyArray_API is declared as extern, hence it is not defined multiple times.
//

#define PY_ARRAY_UNIQUE_SYMBOL VectorAdd_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_17_API_VERSION

#include <arrayobject.h>

/////////////////////////////////////////////////////////////////

const char* g_szBasicAdd = ""
"basic_add(output, a, b)                                           \n"
"                                                                  \n"
"Adds two numpy arrays and writes result to output `output = a + b`\n"
"                                                                  \n"
"Returns:                                                          \n"
"- None                                                            \n";
PyObject* basic_add(PyObject*, PyObject* args)
{
    PyObject* pPyA;
    PyObject* pPyB;
    PyObject* pPyO;
    if(!PyArg_ParseTuple(args, "OOO", &pPyO, &pPyA, &pPyB)) // object references are borrowed so do not decrement
    {
        PyErr_SetString(PyExc_ValueError, "Failed to parse input parameters.");
        return nullptr;
    }

    PyArrayObject* pPyArrayA = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OTF(pPyA, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY));
    if(pPyArrayA == nullptr)
    {
        PyErr_SetString(PyExc_ValueError, "Failed to cast a.");
        return nullptr;
    }

    size_t iSizeA = static_cast<size_t>(PyArray_SIZE(pPyArrayA));
    const double* pcdA = static_cast<const double*>(PyArray_DATA(pPyArrayA));

    PyArrayObject* pPyArrayB = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OTF(pPyB, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY));
    if(pPyArrayB == nullptr)
    {
        PyErr_SetString(PyExc_ValueError, "Failed to cast b.");
        return nullptr;
    }

    size_t iSizeB = static_cast<size_t>(PyArray_SIZE(pPyArrayB));
    const double* pcdB = static_cast<const double*>(PyArray_DATA(pPyArrayB));

    PyArrayObject* pPyArrayO = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OTF(pPyO, NPY_DOUBLE, NPY_ARRAY_OUT_ARRAY));
    if(pPyArrayO == nullptr)
    {
        PyErr_SetString(PyExc_ValueError, "Failed to cast o.");
        return nullptr;
    }

    size_t iSizeO = static_cast<size_t>(PyArray_SIZE(pPyArrayO));
    double* pdO = static_cast<double*>(PyArray_DATA(pPyArrayO));

    if(iSizeO != iSizeA || iSizeO != iSizeB)
    {
        PyErr_SetString(PyExc_ValueError, "Failed input arrays different sizes.");
        return nullptr;
    }

    for(size_t i = 0; i < iSizeO; ++i)
    {
        pdO[i] = pcdA[i] + pcdB[i];
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/////////////////////////////////////////////////////////////////

static PyMethodDef VectorAdd_Methods[] = {
    // The first property is the name exposed to Python, the second is the C++
    // function name that contains the implementation.
    { "basic_add",   static_cast<PyCFunction>(basic_add), METH_VARARGS, g_szBasicAdd},
    // Terminate the array with an object containing nulls.
    { nullptr, nullptr, 0, nullptr }
};
    
/////////////////////////////////////////////////////////////////

static PyModuleDef VectorAdd_Module = {
    PyModuleDef_HEAD_INIT,
    "VectorAdd",                      // Module name to use with Python import statements
    "C++ module demonstrating C Api", // Module description
    0,                                //
    VectorAdd_Methods                 // Structure that defines the methods of the module
};

/////////////////////////////////////////////////////////////////

static bool m_bInitNumpyAPI = false;

void* _initNumpyAPI()
{
    if(!m_bInitNumpyAPI)
    {
        import_array();
        m_bInitNumpyAPI = true;
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////

PyMODINIT_FUNC PyInit_VectorAdd()
{
    _initNumpyAPI();
    return PyModule_Create(&VectorAdd_Module);
}

////////////////////////////////////////////////////////////////
