ifneq ($(words $(CURDIR)),1)
  $(error Unsupported: GNU Make cannot build in directories containing spaces: '$(CURDIR)')
endif

CFLAGS := "-Wall -Werror -Wextra --std=c++17 -O3 -g"
SIMFLAGS := -O2 --cc --autoflush --assert -sv -Mdir bin -CFLAGS $(CFLAGS)
TFLAG := --trace

.PHONY: Vtop run clean info

default: Vtop

Vtop:
	verilator $(SIMFLAGS) top.sv --exe sim_main.cpp $(TFLAG)
	make -j -C bin -f Vtop.mk Vtop

run:
	./bin/Vtop

clean:
	rm -rf bin

info:
	verilator -V
