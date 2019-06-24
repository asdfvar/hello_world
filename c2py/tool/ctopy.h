#ifndef CTOPY_H
#define CTOPY_H

#include <Python.h>
#include <string>
#include <vector>

class C_TO_PY {

   public:

      // constructor name: C_TO_PY
      C_TO_PY (const std::string py_path,
               const std::string module_name,
               const std::string function_name,
               const int num_args)
      {

         // set the python path to the requested location
         setenv ("PYTHONPATH", py_path, 1);

         // initialize the python interpreter
         Py_Initialize ();

         PyObject *pName = PyString_FromString (module_name.c_str ());
         pyModule         = PyImport_Import (pName);
         Py_DECREF (pName);

         if (pyModule != NULL)
         {
            pyFunc = PyObject_GetAttrString (pyModule, function_name);
         }

         pyArgs = PyTuple_New (num_args);
         argument = 0;
      }

      // destructor name: C_TO_PY
      ~C_TO_PY (void)
      {
         // dereference the python function
         Py_DECREF (pyFunc);

         // dereference the python module
         Py_DECREF (pyModule);

         // dereference the python function
         Py_DECREF (pyFunc);

         // dereference the python arguments
         Py_DECREF (pyArgs);

         // close the python interpreter
         Py_Finalize ();
      }

      // function name: load from C_TO_PY
      template <typename type>
         void set_arg (type src)
         {
            PyObject *pyValue;
            if (std::is_same <type, int>::value)
            {
               pyValue = PyInt_FromLong (src);
            }

            PyTuple_SetItem (pyArgs, argument++, pyValue);

            // dereference the python value
            if (pyValue != nullptr) Py_DECREF (pyValue);
         }

      // function name: load from C_TO_PY
      template <typename type>
         void set_arg (type *src, int num_el)
         {
            // instantiate a new python list
            PyObject *pyList = PyList_New (num_el);

            if (std::is_same <type, int>::value)
            {
               for (int ind = 0; ind < num_el; ind++)
               {
                  // set each integer value to the python list
                  PyList_SET_ITEM (pyList, ind, PyInt_FromLong (src[ind]));
               }
            }

            // set the argument value
            PyTuple_SetItem (pyArgs, argument++, pyList);

            // dereference the python list
            if (pyList != nullptr) Py_DECREF (pyList);

         }

      // function name: execute from C_TO_PY
      void execute (void)
      {
         if (pyFunc & PyCallable_Check (pyFunc))
         {
            // call the python function
            PyObject_CallObject (pyFunc, pyArgs);
         }
      }

   private:

      PyObject *pyModule;
      PyObject *pyFunc;
      PyObject *pyArgs;
      int argument;
};

#endif
