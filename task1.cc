#include "timer.h"
#include <iostream>
#include <array>

struct task1 {
    template <typename Chain>
    static auto def(Chain& chain) {
        return chain.next([](auto& c) {
                for (auto& i: c.task1) {
                    ++i;
                }
            });
    }
};

struct task2 {
    template <typename Chain>
    static auto def(Chain& chain) {
        return chain.template depends<task1>().
            next([](auto& c) {
                for (size_t i = 0; i < c.task1.size(); ++i) {
                    c.task2[i] = c.task1[i] * 2;
                }
            });
    }
};

template <typename Fn>
struct callback_chain {
    Fn fn;
    callback_chain(Fn f) : fn(f) {}

    template <typename Task>
    auto depends() const {
        return Task::def(*this);
    }

    template <typename NextFn>
    auto next(NextFn&& next_fn) const {
        auto new_fn = [=](auto& c) {
            this->fn(c);
            next_fn(c);
        };
        return callback_chain<decltype(new_fn)>(new_fn);
    }

    template <typename Context>
    auto run(Context& c) const {
        return fn(c);
    }
};

template <>
struct callback_chain<void> {
    template <typename Task>
    auto def() const {
        return Task::def(*this);
    }

    template <typename Task>
    auto depends() const {
        return Task::def(*this);
    }

    template <typename Fn>
    auto next(Fn&& fn) const {
        callback_chain<Fn> chain(fn);
        return chain;
    }

};

int main() {
    auto compiled_task = callback_chain<void>().def<task2>();

    struct {
        std::array<int, 100> task1;
        std::array<int, 100> task2;
    } c;

    for (auto&& i: c.task1) {
        i = 0;
    }

    std::cout << "c.task1\n";
    for (auto&& i: c.task1) {
        std::cout << i << ", ";
    }
    std::cout << "c.task2\n";
    for (auto&& i: c.task2) {
        std::cout << i << ", ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "run\n";
    compiled_task.run(c);

    std::cout << "c.task1\n";
    for (auto&& i: c.task1) {
        std::cout << i << ", ";
    }
    std::cout << "c.task2\n";
    for (auto&& i: c.task2) {
        std::cout << i << ", ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "run\n";
    compiled_task.run(c);

    std::cout << "c.task1\n";
    for (auto&& i: c.task1) {
        std::cout << i << ", ";
    }
    std::cout << "c.task2\n";
    for (auto&& i: c.task2) {
        std::cout << i << ", ";
    }
    std::cout << std::endl << std::endl;

    return 0;
}
