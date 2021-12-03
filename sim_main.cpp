#include <iostream>
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vtop.h"

#define NCLKS 3
#define PC0 5000 // ps
#define PC1 8000 // ps
#define PC2 9000 // ps

Vtop *top_;  // module instantiation
vluint64_t main_time = 0; // simulation time

class Trace {
public:
    Trace(Vtop* top, const char* filename) {
        Verilated::traceEverOn(true);
        tp_ = new VerilatedVcdC;
        top->trace(tp_, 99);  // trace 99 levels of hierarchy
        tp_->spTrace()->set_time_resolution("ps");
        tp_->spTrace()->set_time_unit("ps");
        tp_->open(filename);
    }

    virtual ~Trace() {
        tp_->close();
        delete tp_;
    }

    void dump() {
        tp_->dump(main_time);
    }

    void exit() {
        tp_->flush();
    }

private:
    VerilatedVcdC *tp_;
};

class Clock {
public:
    Clock(vluint32_t period, bool val = false) {
        period_ = period;
        val_ = val;
        now_ = 0;
    }

    vluint32_t next_edge(void) {
        return abs(period_/2 - now_);
    }

    void advance(vluint32_t step = 0) {
        now_ += step;
        if (now_ == period_/2) {
            val_ = !val_;
            now_ = 0;
        }
    }

    bool state(void) {
        return val_;
    }

private:
    vluint32_t period_ = 0;
    vluint32_t now_ = 0;
    bool val_ = false;
};

Clock *clk0_, *clk1_, *clk2_;
Trace *trace_;

vluint32_t min_time(vluint32_t *t) {
    vluint32_t tmp = t[0];
    for (int i = 0; i < NCLKS; i++) {
        if (t[i] < tmp)
            tmp = t[i];
    }
    return tmp;
}

void init() {
    clk0_ = new Clock(PC0);
    clk1_ = new Clock(PC1);
    clk2_ = new Clock(PC2);

    top_->clk0 = clk0_->state();
    top_->clk1 = clk1_->state();
    top_->clk2 = clk2_->state();
}

void tick() {
    vluint32_t step = 0;
    vluint32_t t[NCLKS];

    // min time to next edge
    t[0] = clk0_->next_edge();
    t[1] = clk1_->next_edge();
    t[2] = clk2_->next_edge();
    step = min_time(t);

    // advance the clocks
    clk0_->advance(step);
    clk1_->advance(step);
    clk2_->advance(step);

    // drive clks
    top_->clk0 = clk0_->state();
    top_->clk1 = clk1_->state();
    top_->clk2 = clk2_->state();

    // eval and dump waveform
    top_->eval();
    main_time += step;
    trace_->dump();
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv); // remember args

    top_ = new Vtop;
    trace_ = new Trace(top_, "wave.vcd");

    init();
    for (int i = 0; i < 1000; i++) {
        tick();
    }

    top_->final(); // done simulating
    delete top_; // destroy model
    trace_->exit();

    exit(0);
}
