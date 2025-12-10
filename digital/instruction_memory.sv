`include "parameters.sv"

module InstructionMemory(
  input   logic[`b_size-1:0] address;
  output  logic[`b_size-1:0] instruction;
)

reg[`b_size-1:0] mem[0:`mem_size-1];

initial begin
  $readmemh("rom.bin", mem);
end

assign instruction = mem[address]

endmodule
