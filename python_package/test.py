import fastprimes
import time

# --- Test the C extension ---
N = 1_000_000_000

print(f"Finding primes up to {N}...")

start_c = time.time()
primes_c = fastprimes.find_primes(N)
end_c = time.time()

print(f"--- C Extension ---")
print(f"Found {len(primes_c)} primes.")
print(f"Time: {end_c - start_c:.4f} seconds.")


# --- Test the pure Python version (for comparison) ---
def py_sieve(n):
    is_prime = [True] * (n + 1)
    p = 2
    while (p * p <= n):
        if (is_prime[p]):
            for i in range(p * p, n + 1, p):
                is_prime[i] = False
        p += 1
    
    primes = []
    for p in range(2, n + 1):
        if is_prime[p]:
            primes.append(p)
    return primes

start_py = time.time()
primes_py = py_sieve(N)
end_py = time.time()

print(f"\n--- Pure Python ---")
print(f"Found {len(primes_py)} primes.")
print(f"Time: {end_py - start_py:.4f} seconds.")

# --- Results ---
speedup = (end_py - start_py) / (end_c - start_c)
print(f"\nC extension was {speedup:.2f}x faster.")