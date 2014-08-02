#include "task.h"
#include "chain_scheduler.h"
#include <set>

struct task1 {
    template <typename C, typename N>
    static void operator () (C& c, N&& next) const {
        printf("hello from task1\n");
        next();
    }
};


int main() {
    struct Context {//: immediate_pipeline<Context> {
        typedef std::allocator<void> allocator_type;
        vec::task::chain_scheduler<Context> scheduler;
        std::set<void*> done;

        Context() : scheduler(*this) {}
    };

    using namespace vec::task;

    Context c;
    //c.depends().
    c << exec(task1()) << exec([](auto& c, auto&& next) {
        printf("hello 123\n");
        next();
    });

        printf("s\n");
    c.scheduler.step();
        printf("s2\n");

    return 0;
}
