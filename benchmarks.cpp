#include "benchmarks.h"

double measureMemoryReadSpeed(size_t num_elements) {
    std::vector<int> vec(num_elements, 1);

    auto start = std::chrono::high_resolution_clock::now();

    volatile int sink = 0; // To prevent the loop from being optimized away
    for (size_t i = 0; i < num_elements; ++i) {
        sink += vec[i];
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    // Calculate read speed
    double read_speed = (num_elements * sizeof(int)) / elapsed.count();
    return read_speed; // Bytes per second
}
double measureMemoryWriteSpeed(size_t num_elements) {
    std::vector<int> vec(num_elements, 1);

    auto start = std::chrono::high_resolution_clock::now();

    std::fill(vec.begin(), vec.end(), 42);  // Fill the array with the number 42

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    // Calculate write speed
    double write_speed = (num_elements * sizeof(int)) / elapsed.count();
    return write_speed; // Bytes per second
}
void mem_read() {
    size_t num_elements = 100000000; // 100 million elements

    // Measure read speed
    double read_speed = measureMemoryReadSpeed(num_elements);
    double read_speed_gb = (read_speed * 8) / (1024.0 * 1024.0 * 1024.0);
    std::cout << "Memory read speed: " << read_speed_gb << " GB/s" << std::endl;

    // Measure write speed
    double write_speed = measureMemoryWriteSpeed(num_elements);
    double write_speed_gbps = (write_speed * 8) / (1024.0 * 1024.0 * 1024.0);
    std::cout << "Memory write speed: " << write_speed_gbps << " Gb/s" << std::endl;
}
std::vector<bool> sieve_of_eratosthenes(int n) {
    std::vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return is_prime;
}
void prime_sive(int n) {
    std::vector<bool> is_prime = sieve_of_eratosthenes(n);
    for (int i = 2; i <= n; ++i) {  // Print primes
        if (is_prime[i]) {
            std::cout << i << ' ';
        }
    }
    std::cout << std::endl;
}

namespace primes {
    void sieveOfEratosthenes(int n) {
        // Create a boolean array "isPrime[0..n]" and initialize all entries to true
        std::vector<bool> isPrime(n + 1, true);

        for (int p = 2; p * p <= n; p++) {
            // If isPrime[p] is true, then it's a prime number
            if (isPrime[p]) {
                // Update all multiples of p
                for (int i = p * p; i <= n; i += p) {
                    isPrime[i] = false;
                }
            }
        }

        // Print prime numbers
        for (int p = 2; p <= n; p++) {
            if (isPrime[p]) {
                std::cout << p << " ";
            }
        }
    }
    int main(int n) {
        std::cout << "Prime numbers up to " << n << " are:" << std::endl;
        sieveOfEratosthenes(n);
        return 0;
    }
}
