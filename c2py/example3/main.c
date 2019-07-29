/*
** a simplified example derived from:
** https://docs.python.org/release/2.6.5/extending/embedding.html#pure-embedding
*/

#include <Python.h>
#include <stdio.h>

int main ( int argc, char *argv[])
{
    // Set PYTHONPATH to working directory
    setenv ("PYTHONPATH", ".", 1);

    // start the python interpreter
    if (!Py_IsInitialized ()) Py_Initialize ();
    // Py_SetPath (...)

    // import the python module to use
    PyObject *pName   = PyString_FromString ((char*)"arrays");
    PyObject *pModule = PyImport_Import (pName);
    Py_DECREF (pName);

    if (pModule != NULL)
    {
       // get the attribute (function) of the python module to be used
       PyObject *pFunc = PyObject_GetAttrString (pModule, "multiply_sum");

       if (pFunc && PyCallable_Check (pFunc))
       {
          PyObject *pArgs = PyTuple_New (1);

          PyObject *array = PyList_New (4);

          PyObject *value;

          value = PyFloat_FromDouble (3.14159);
          PyList_SetItem (array, 0, value);
          value = PyFloat_FromDouble (2.71828);
          PyList_SetItem (array, 1, value);
          value = PyFloat_FromDouble (11.83482);
          PyList_SetItem (array, 2, value);
          value = PyFloat_FromDouble (5.334212);
          PyList_SetItem (array, 3, value);

          PyTuple_SetItem (pArgs, 0, array);

          // call the python function
          value = PyObject_CallObject (pFunc, pArgs);
          Py_DECREF (pArgs);

          if (value != NULL)
          {
             printf ("Result of call: %f\n", PyFloat_AsDouble (value));
             Py_DECREF (value);
          }
          else
          {
             Py_DECREF (pModule);
             Py_DECREF (pFunc);
             PyErr_Print ();
             fprintf (stderr,"Call failed\n");
             return 1;
          }
       }
       else
       {
          if (PyErr_Occurred ())
          {
             PyErr_Print ();
          }
       }
       Py_XDECREF (pFunc);
       Py_DECREF (pModule);
    }
    else
    {
        PyErr_Print ();
        return 1;
    }

    // stop the python interpreter
    Py_Finalize ();

    return 0;
}
