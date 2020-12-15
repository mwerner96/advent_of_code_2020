#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

struct SpokenNumber {
    int times_spoken, last_spoken, prev_last_spoken;
    SpokenNumber() : times_spoken(0), last_spoken(0), prev_last_spoken(0) {}
    inline void update_last_spoken(int idx) {
        times_spoken++;
        prev_last_spoken = last_spoken;
        last_spoken = idx;
    }
    inline int get_diff() {
        return last_spoken - prev_last_spoken;
    }
};

constexpr static int LINE_LEN = 1000;

struct RambunctiousRecitation {
    std::unordered_map<int, SpokenNumber> numbers;
    int iteration, last_number;

    RambunctiousRecitation (std::string starting_numbers) {
        std::ifstream is(starting_numbers);
        if (!is.is_open())
            throw std::invalid_argument(starting_numbers);
        char line[LINE_LEN];
        if (!is.getline(line, LINE_LEN))
            throw std::runtime_error("no line");
        std::stringstream ss(line);
        iteration = 0;
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            int idx = std::stoi(substr);
            numbers[idx].update_last_spoken(iteration++);
            last_number = idx;
        }
        is.close();
    }

    void play(int play_until) {
        for (; iteration < play_until; iteration++) {
            if (numbers[last_number].times_spoken == 1)
                last_number = 0;
            else
                last_number = numbers[last_number].get_diff();
            numbers[last_number].update_last_spoken(iteration);
        }
    }
};

int main() {
    RambunctiousRecitation rr1("input");
    rr1.play(2020);
    std::cout << rr1.last_number << std::endl;
    RambunctiousRecitation rr2("input");
    rr2.play(30000000);
    std::cout << rr2.last_number << std::endl;
}
