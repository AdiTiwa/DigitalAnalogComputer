`include "parameters.sv"

module ALU (
    input logic  [`b_width-1 :0] ha,
    input logic  [`b_width-1:0] la,
    input logic  [`b_width-1:0] hb,
    input logic  [`b_width-1:0] lb,
    input logic  [3:0]  op,
    output logic [`b_width-1:0] lc,
    output logic [`b_width-1:0] hc,
    output logic        zr,
    output logic        ng,
    output logic        carry,
);


always @(*) begin
  lc    = 0;
  hc    = 0;
  carry = 0;

  case (op)
    // single register operations 
    0: {carry, lc} = la + lb;
    1: lc = la - lb;
    2: lc = la & lb;
    3: lc = la | lb;
    4: lc = lb >> 1;
    5: lc = lb <<< 1;
    6: lc = lb - 1;
    7: lc = lb + 1;

    // double register operations
    8: begin
      reg ic; // intermediate carry

      {ic, lc} = la + lb;
      {carry, hc} = ha + hb + ic;
    end
    9: {hc, lc} = {ha - hb, la - lb};
    10: {hc, lc} = {ha & hb, la & lb};
    11: {hc, lc} = {ha | hb, la | lb};
    12: {hc, lc} = {hb, lb} >> 1;
    13: {hc, lc} = {hb, lb} <<< 1;
    14: {hc, lc} = {hb, lb} - 1;
    15: {hc, lc} = {hb, lb} + 1;

    default: ;
  endcase

  zr = (lc == 0);
  ng = lc[15];
end


endmodule
