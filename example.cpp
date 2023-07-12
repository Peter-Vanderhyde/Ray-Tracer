#include <thread>
#include <random>
#include <vector>
#include <iostream>
#include <chrono>

double random_double() {
    static thread_local std::random_device rd;
    static thread_local std::default_random_engine engine(rd());
    static thread_local std::uniform_real_distribution<double> distribution{0, 1};
    return distribution(engine);
}

void trace(double& result) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int sum{0};
    for (int i = 0; i < 1000000000; ++i) {
        sum += i;
    }
        result = random_double();
}

int main() {
    int N = 4;
    std::vector<std::thread> threads;
    std::vector<double> results(N);
    for (int i = 1; i < N; ++i) {
        std::thread t{trace, std::ref(results.at(i))};
        threads.push_back(std::move(t));
    }

    // compute on main thread
    trace(results.at(0));

    // clean up / gather results
    for (auto& thread : threads) {
        thread.join();
    }

    for (double result : results) {
        std::cout << result << '\n';
    }
}