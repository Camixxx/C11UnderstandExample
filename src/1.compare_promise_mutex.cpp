#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

// for promise
#include <future>

std::mutex mtx;
int shared_data = 0;

void thread_task_mutex(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++shared_data;
    }
}

void test_mutex(int num_threads, int iterations) {
    std::vector<std::thread> threads;
    shared_data = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_task_mutex, iterations);
    }

    for (auto &t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "[Mutex] Final shared_data: " << shared_data 
              << ", Time: " << elapsed.count() << " seconds\n";
}

#include <future>

int thread_task_promise(int iterations) {
    int local_data = 0;
    for (int i = 0; i < iterations; ++i) {
        ++local_data;
    }
    return local_data;
}

void test_future(int num_threads, int iterations) {
    std::vector<std::future<int>> futures;
    int total_result = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        std::promise<int> promise;
        futures.emplace_back(promise.get_future());
        std::thread([iterations, p = std::move(promise)]() mutable {
            p.set_value(thread_task_promise(iterations));
        }).detach();
    }

    for (auto &f : futures) {
        total_result += f.get();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "[Future] Final shared_data: " << total_result 
              << ", Time: " << elapsed.count() << " seconds\n";
}


int main() {
    const int num_threads = 4;
    const int iterations = 1000000;

    std::cout << "Running Mutex Test 1000000 iter...\n";
    test_mutex(num_threads, iterations);

    std::cout << "\nRunning Future Test 1000000 iter...\n";
    test_future(num_threads, iterations);

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
