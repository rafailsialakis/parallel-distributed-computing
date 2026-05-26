import sys
import time
import math

def main():
    # Check command-line arguments
    if len(sys.argv) != 2:
        print("Usage: python sieve.py <size>")
        sys.exit(1)
    
    try:
        size = int(sys.argv[1])
    except ValueError:
        print("Integer argument expected")
        sys.exit(1)
    
    if size <= 0:
        print("size should be positive integer")
        sys.exit(1)

    # Initialize array with True values (assuming all numbers are prime initially)
    prime = [True for _ in range(size+1)]
    # 0 and 1 are not prime
    if size > 0:
        prime[0] = prime[1] = False
    
    # Get current time
    start = time.time()
    
    # Apply Sieve of Eratosthenes algorithm
    limit = int(math.sqrt(size)) + 1
    for p in range(2, limit + 1):
        # If prime[p] is True, then it is a prime
        if prime[p]:
            # Update all multiples of p
            for i in range(p*p, size + 1, p):
                prime[i] = False
    
    # Calculate elapsed time
    elapsed_time_ms = (time.time() - start) * 1000
    
    # Count primes
    count = sum(1 for i in range(2, size + 1) if prime[i])
    
    print(f"number of primes {count}")
    print(f"time in ms = {elapsed_time_ms:.2f}")

if __name__ == "__main__":
    main()
