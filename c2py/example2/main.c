/*
** a simplified example derived from:
** https://docs.python.org/release/2.6.5/extending/embedding.html#pure-embedding
*/

#include <Python.h>
#include <stdio.h>

int main ( int argc, char *argv[])
{
    // define integers in C from which a * b will be computed in python:
    int a = 7;
    int b = 5;

    // Set PYTHONPATH to working directory
    setenv ("PYTHONPATH", ".", 1);

    // start the python interpreter
    Py_Initialize ();

    // import the python module to use
    PyObject* pName   = PyString_FromString ((char*)"multiply");
    PyObject* pModule = PyImport_Import (pName);
    Py_DECREF (pName);

    if (pModule != NULL)
    {
       // get the attribute (function) of the python module to be used
       PyObject* pFunc = PyObject_GetAttrString (pModule, "multiply");

       if (pFunc && PyCallable_Check (pFunc))
       {
          PyObject* pArgs = PyTuple_New (2);

          PyObject* pValue = PyInt_FromLong (a);
          PyTuple_SetItem (pArgs, 0, pValue);

          pValue = PyInt_FromLong (b);
          PyTuple_SetItem (pArgs, 1, pValue);

          // call the python function
          pValue = PyObject_CallObject (pFunc, pArgs);
          Py_DECREF (pArgs);

          if (pValue != NULL)
          {
             printf ("Result of call: %ld\n", PyInt_AsLong(pValue));
             Py_DECREF (pValue);
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
