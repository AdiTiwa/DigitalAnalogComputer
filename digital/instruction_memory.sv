`include "parameters.sv"

module InstructionMemory(
  input   logic[`b_size-1:0] address;
  output  logic[`i_size-1:0] instruction;
)

reg[`i_size-1:0] mem[0:`mem_size-1];

initial begin
  $readmemh("rom.bin", mem);
end

assign instruction = mem[address]

endmodule
