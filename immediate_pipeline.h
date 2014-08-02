#pragma once

// working...
#include "chain_scheduler.h"

namespace vec {
namespace task {

template <typename Context>
struct immediate_pipeline {
    Context& me() {return this;}

    template <typename Func>
    Context& exec(Func&& f) {
        Context& c = me();
        c.scheduler.push(f);
        return c;
    }

    template <typename... Args>
    Context& depends(Args... args) {
        size_t count = sizeof...(Args);
        auto done = [count]() mutable {
            return [&count](auto done) {
                --count;
                if (--count <= 0) {
                    done();
                }
            };
        };
        --arg_count;
        return _depends(arg_count, args...);
    }

private:
    template <typename F, typename... Args>
    Context& _depends(size_t arg_count, F&& f, Args... args) {
        me().push([]() {});
        return _depends(arg_count, args...);
    }
};

}
}
