#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#define NUM_THREADS 12 // Let's use 8 threads

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    if (n < 2) return 1;

    // --- Phase 1: Sieve up to sqrt(n) on the MAIN thread ---
    long root = (long)sqrt(n);
    char* is_prime = calloc(n + 1, sizeof(char)); // Main boolean array
    if (is_prime == NULL) return 1;

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
    if (base_primes == NULL) return 1;
    
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

    // --- Phase 4: Collect results (print) ---
    // (We also need to print the base primes themselves)
    for (long i = 2; i <= n; i++) {
        if (is_prime[i] == 0) {
            // printf("%ld ", i); // Don't print, it's too slow
        }
    }
    // printf("\n");

    free(is_prime);
    free(base_primes);
    return 0;
}