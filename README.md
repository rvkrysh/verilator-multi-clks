# Multiple clocks in verilator 
First, check these pages https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html and https://zipcpu.com/blog/2018/09/06/tbclock.html. The crux of the the idea and code are borrowed from there (Thanks!), so it's worthing also checking other works by ZipCPU.

This example has 3 basic counters running on different clocks. It generates a VCD file that can be viewed using gtkwave.
