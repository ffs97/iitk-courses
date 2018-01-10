module eq2 (in1, in2, eq);
	input [1:0] in1, in2;
	output eq;
	
	wire [1:0] eq_bit;
	eq1 eq_bit_instance0 (.in1(in1[0]), .in2(in2[0]), .eq(eq_bit[0]));
	eq1 eq_bit_instance1 (.in1(in1[1]), .in2(in2[1]), .eq(eq_bit[1]));
	
	assign eq = eq_bit[0] && eq_bit[1];
endmodule