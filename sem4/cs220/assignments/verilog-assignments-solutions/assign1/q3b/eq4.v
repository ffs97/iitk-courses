`timescale 1ns / 1ps

module eq4(clk, in, push1, push2, eq);
input clk;
input [3:0] in;
input push1, push2;

output eq;

reg [3:0] in1, in2;

always@(posedge clk)
begin
	if (push1) begin
		in1 = in;
	end
	if (push2) begin
		in2 = in;
	end
end

wire eq0, eq1;
eq2 eq_bit01_unit (.in1(in1[1:0]), .in2(in2[1:0]), .eq(eq0));
eq2 eq_bit23_unit (.in1(in1[3:2]), .in2(in2[3:2]), .eq(eq1));
assign eq = eq0 & eq1;

endmodule
