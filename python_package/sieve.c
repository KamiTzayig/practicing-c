#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    
    }

    long n;
    n = atol(argv[1]);
    char *is_prime = calloc(n+1, sizeof(char));

    for(long i = 2; i<=sqrt(n); i++){
        if(is_prime[i] == 0){
            for(long j = i*i; j<=n; j+=i){
                is_prime[j] = 1;
            }
        }
    }


    for(long i = 2; i<=n; i++){
        if(is_prime[i] == 0){
            printf("%ld\n", i);
        }
    }

    free(is_prime);
    return 0;
}