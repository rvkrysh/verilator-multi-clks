`default_nettype none

module counter_naive (
  input wire       clk,
  output reg [7:0] cnt = 0
);

always @(posedge clk) begin
  cnt <= cnt + 1;
end

endmodule

`default_nettype wire
