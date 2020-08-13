`default_nettype none

module top (
  input wire       clk0,
  input wire       clk1,
  input wire       clk2,
  output reg [7:0] cnt0,
  output reg [7:0] cnt1,
  output reg [7:0] cnt2
);

// module instantiations
counter_naive counter_naive_inst0 (
  .clk(clk0),
  .cnt(cnt0)
);

counter_naive counter_naive_inst1 (
  .clk(clk1),
  .cnt(cnt1)
);

counter_naive counter_naive_inst2 (
  .clk(clk2),
  .cnt(cnt2)
);

endmodule

`default_nettype wire
