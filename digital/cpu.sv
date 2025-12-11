`include "parameters.sv"
`include "instruction_memory.sv"

module cpu (
  input   logic[`b_size-1:0]  M;
  input   logic[`b_size-1:0]  instruction;
  input   logic               reset;

  output  logic[`b_size-1:0]  oM;
  output  logic               wrM;
  output  logic[`b_size-4:0]  A;
  output  logic[`b_size-1:0]  pc;
)

  

endmodule
