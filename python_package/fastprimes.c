// Python.h must be included first
#include <python3.12/Python.h> 

// Standard C Libraries
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h> // Good for debugging, though not used in main path

// Threading Library
#include <pthread.h>

#define NUM_THREADS 8 // Use 8 threads as it was efficient

//####################################################################
//##
//## PART 1: MULTI-THREADED C LOGIC
//##
//####################################################################

// We need a struct to pass arguments to each thread
typedef struct {
    long start;
    long end;
    long* base_primes;
    long base_primes_count;
    char* is_prime; // The main array all threads share
} ThreadArgs;


// This is the function each thread will run
void* sieve_worker(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    char* is_prime = args->is_prime;

    // Loop through all base primes
    for (long i = 0; i < args->base_primes_count; i++) {
        long p = args->base_primes[i];
        
        // Find the first multiple of p within this thread's segment
        // This avoids unnecessary checks before the 'start' boundary
        long start_multiple;
        if (args->start % p == 0) {
            start_multiple = args->start;
        } else {
            start_multiple = args->start + (p - (args->start % p));
        }
        
        // Mark all multiples of p in the segment
        for (long j = start_multiple; j <= args->end; j += p) {
            is_prime[j] = 1; // Mark as not prime
        }
    }
    return NULL;
}

// This is the main C function that performs the threaded sieve.
// It no longer prints; it returns an array of primes.
long* sieve(long n, long *count) {
    if (n < 2) {
        *count = 0;
        return NULL;
    }

    // --- Phase 1: Sieve up to sqrt(n) on the MAIN thread ---
    long root = (long)sqrt(n);
    char* is_prime = calloc(n + 1, sizeof(char));
    if (is_prime == NULL) { *count = 0; return NULL; }

    for (long p = 2; p * p <= root; p++) {
        if (is_prime[p] == 0) {
            for (long i = p * p; i <= root; i += p)
                is_prime[i] = 1;
        }
    }

    // --- Phase 1.5: Collect all base primes ---
    long base_primes_count = 0;
    for (long p = 2; p <= root; p++) {
        if (is_prime[p] == 0) {
            base_primes_count++;
        }
    }
    
    long* base_primes = malloc(base_primes_count * sizeof(long));
    if (base_primes == NULL) { 
        free(is_prime); 
        *count = 0; 
        return NULL; 
    }
    
    long j = 0;
    for (long p = 2; p <= root; p++) {
        if (is_prime[p] == 0) {
            base_primes[j++] = p;
        }
    }

    // --- Phase 2: Create and run worker threads ---
    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    
    long segment_size = (n - root) / NUM_THREADS;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].start = root + 1 + (i * segment_size);
        if (i == NUM_THREADS - 1) {
            args[i].end = n; // Last thread takes the remainder
        } else {
            args[i].end = root + ((i + 1) * segment_size);
        }
        
        args[i].base_primes = base_primes;
        args[i].base_primes_count = base_primes_count;
        args[i].is_prime = is_prime;

        pthread_create(&threads[i], NULL, sieve_worker, &args[i]);
    }

    // --- Phase 3: Wait for all threads to finish ---
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // --- Phase 4: Collect results (Count, Malloc, and Fill) ---
    
    // 1. Count the primes (includes base primes and primes in segments)
    *count = 0;
    for (long p = 2; p <= n; p++) {
        if (is_prime[p] == 0) {
            (*count)++;
        }
    }

    // 2. Malloc space for the result array
    long *primes = malloc(*count * sizeof(long));
    if (primes == NULL) {
        free(is_prime);
        free(base_primes);
        *count = 0;
        return NULL;
    }

    // 3. Fill the result array
    j = 0;
    for (long p = 2; p <= n; p++) {
        if (is_prime[p] == 0) {
            primes[j++] = p;
        }
    }

    // --- Phase 5: Cleanup C-side memory ---
    free(is_prime);
    free(base_primes);
    
    // Return the final C array to the wrapper
    return primes;
}


//####################################################################
//##
//## PART 2: PYTHON C-API WRAPPER BOILERPLATE
//##
//####################################################################

// This is the "translator" function that Python calls.
// It unpacks Python args, calls the C logic, and packs the C result.
static PyObject* fastprimes_find_primes(PyObject *self, PyObject *args) {
    long n;
    
    // 1. Parse the input from Python (expect one long int "l")
    if (!PyArg_ParseTuple(args, "l", &n)) {
        return NULL; // Failed to parse
    }
    
    long count = 0;
    
    // 2. Call your PURE C function (now multi-threaded!)
    long *primes = sieve(n, &count);
    
    if (primes == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory in C.");
        return NULL;
    }

    // 3. Convert C array back to a Python List
    PyObject *py_list = PyList_New(count);
    if (py_list == NULL) { // Always check for allocation failures
        free(primes);
        PyErr_SetString(PyExc_MemoryError, "Failed to create Python list.");
        return NULL;
    }

    for (long i = 0; i < count; i++) {
        PyObject *py_long = PyLong_FromLong(primes[i]);
        if (py_long == NULL) { // Check for failure
            free(primes);
            Py_DECREF(py_list); // Free the list we just made
            PyErr_SetString(PyExc_MemoryError, "Failed to create Python integer.");
            return NULL;
        }
        PyList_SetItem(py_list, i, py_long); // Steals reference, no DECREF needed
    }
    
    // 4. Free C array and return Python list
    free(primes);
    return py_list;
}

// --- Method Definition Table ---
// Tells Python the function names in this module
static PyMethodDef FastPrimesMethods[] = {
    {"find_primes", fastprimes_find_primes, METH_VARARGS, "Find all primes up to n using a multi-threaded C Sieve."},
    {NULL, NULL, 0, NULL} // Sentinel
};

// --- Module Definition Structure ---
static struct PyModuleDef fastprimes_module = {
    PyModuleDef_HEAD_INIT,
    "fastprimes",  // Module name
    "A module to find primes, written in C.", // Docstring
    -1,
    FastPrimesMethods
};

// --- Module Initialization Function ---
// This is called when Python does "import fastprimes"
PyMODINIT_FUNC PyInit_fastprimes(void) {
    return PyModule_Create(&fastprimes_module);
}