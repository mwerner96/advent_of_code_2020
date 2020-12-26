#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <vector>

static constexpr int MAX_NUM = 1000;
static constexpr int UDOSTRING = 200;
static constexpr int LINE_LEN = UDOSTRING;

static inline bool number_in_range(int num) {
    return (num >= 0 && num < MAX_NUM);
}

struct Ticket {
    std::vector<int> numbers;

    Ticket(std::stringstream & ss) {
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            int num = std::stoi(substr);
            if (!number_in_range(num))
                throw std::invalid_argument("num too large");
            numbers.emplace_back(num);
        }
    }
};

struct TicketConstraint {
    std::string name;
    bool valid_nums[MAX_NUM] = {false};
    unsigned int possible_idx;
    unsigned int idx;

    TicketConstraint(std::smatch & constraint_match) : possible_idx(0) {
        name = constraint_match[1].str();

        int constraints[4];

        for (size_t i = 2; i < constraint_match.size(); ++i) {
            auto sub_match = constraint_match[i];
            constraints[i-2] = std::stoi(sub_match.str());
            if (!number_in_range(constraints[i-2]))
                throw std::runtime_error("constraint not ok");
        }

        for (int i = 0; i < 4; i += 2) {
            for (int j = constraints[i]; j <= constraints[i+1]; j++) {
                valid_nums[j] = true;
            }
        }
    }

    bool check_ticketlist_at_idx(std::vector<Ticket> & tickets, int idx) {
        for (auto ticket : tickets) {
            if (!valid_nums[ticket.numbers[idx]])
                return false;
        }
        return true;
    }
};

struct TicketChecker {
    std::vector<TicketConstraint> constraints;
    Ticket * my_ticket;
    std::vector<Ticket> nearby_tickets;
    std::vector<Ticket> valid_tickets;

    TicketChecker(std::string notes) {
        std::ifstream is(notes);
        if (!is.is_open())
            throw std::invalid_argument(notes);
        const std::regex constraint_regex("(.*): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");
        std::smatch constraint_match;
        char line[LINE_LEN];
        // constraints
        while (is.getline(line, LINE_LEN)) {
            std::string s(line);
            if (std::regex_match(s, constraint_match, constraint_regex))
                constraints.emplace_back(constraint_match);
            else
                break;
        }
        // my ticket
        {
            is.getline(line, LINE_LEN);
            std::string s(line);
            if (s.compare("your ticket:"))
                throw std::invalid_argument("expected to read >your ticket:<");
            is.getline(line, LINE_LEN);
            std::stringstream ss(line);
            my_ticket = new Ticket(ss);
            is.getline(line, LINE_LEN);
        }
        // nearby tickets
        {
            is.getline(line, LINE_LEN);
            std::string s(line);
            if (s.compare("nearby tickets:"))
                throw std::invalid_argument("expected to read >nearby tickets:<");
            while (is.getline(line, LINE_LEN)) {
                std::stringstream ss(line);
                nearby_tickets.emplace_back(ss);
            }
        }
        is.close();
    }

    ~TicketChecker() {
        delete my_ticket;
    }

    int filter_valid_tickets_error_rate() {
        valid_tickets.clear();
        int rate = 0;
        bool valid_nums[MAX_NUM] = {false};
        for (auto & constraint : constraints) {
            for (int i = 0; i < MAX_NUM; i++) {
                if (constraint.valid_nums[i])
                    valid_nums[i] = true;
            }
        }
        for (auto ticket : nearby_tickets) {
            bool ticket_valid = true;
            for (int number : ticket.numbers) {
                if (!valid_nums[number]) {
                    rate += number;
                    ticket_valid = false;
                }
            }
            if (ticket_valid)
                valid_tickets.push_back(ticket);
        }
        return rate;
    }

    uint64_t part_2() {
        for (auto & constraint : constraints) {
            for (int i = 0; i < constraints.size(); i++) {
                if (constraint.check_ticketlist_at_idx(valid_tickets, i))
                    constraint.possible_idx |= 1 << i;
            }
        }
        int done = 0;
        while (true) {
            for (auto & constraint : constraints) {
                if (__builtin_popcount(constraint.possible_idx) == 1) {
                    unsigned int possible_idx = constraint.possible_idx;
                    constraint.idx = __builtin_ctz(possible_idx);
                    for (auto & inner : constraints)
                        inner.possible_idx &= ~possible_idx;
                    done++;
                }
            }
            if (done == constraints.size())
                break;
        }
        uint64_t product = 1;
        for (auto & constraint : constraints) {
            if (!constraint.name.compare(0, 9, "departure"))
                product *= my_ticket->numbers[constraint.idx];
        }
        return product;
    }
};

int main() {
    TicketChecker c("input");
    std::cout << c.filter_valid_tickets_error_rate() << std::endl;
    std::cout << c.part_2() << std::endl;
}
