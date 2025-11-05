from distutils.core import setup, Extension

module1 = Extension(
    'fastprimes',
    sources=['fastprimes.c'],
    extra_link_args=['-lpthread']  # <-- ADD THIS LINE
)

setup(
    name='FastPrimes',
    version='2.0', # Updated version
    description='A C extension for finding primes, now multi-threaded',
    ext_modules=[module1]
)