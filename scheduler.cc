#include "task.h"
#include <set>

int main() {
    struct Context : immediate_pipeline<Context> {
        typedef std::allocator<void> allocator_type;
        vec::task::chain_scheduler<Context> scheduler;
        std::set<void*> done;

        Context() : immediate_pipeline<Context>(*this) {}
    };

    using namespace vec::task;

    Context c;
    c.depends().
    c << task([](auto& c, auto&& next) {
        printf("hello world\n");
        next();
    }) << task([](auto& c, auto&& next) {
        printf("hello 123\n");
        next();
    });

        printf("s\n");
    c.scheduler.step();
        printf("s2\n");

    return 0;
}
