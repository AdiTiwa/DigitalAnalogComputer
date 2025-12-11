`include "parameters.sv"

module port_ram (
  input logic		            clk,
  input logic[`b_size-4:0]  addr,
  input logic               cs,
  input logic               we,
  input logic               oe,

  inout logic[`b_size-1:0]	data,
);

  reg [`b_size-1:0] 	tmp_data;
  reg [`b_size-1:0] 	mem[`ram_size];

  always @ (posedge clk) begin
    if (cs & we)
      mem[addr] <= data;
  end

  always @ (posedge clk) begin
    if (cs & !we)
    	tmp_data <= mem[addr];
  end

  assign data = cs & oe & !we ? tmp_data : 'hz;
endmodule
