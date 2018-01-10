`timescale 1ns / 1ps

module add1
(
	input wire Cin, a, b,
	output wire Cout, sum);

	assign Cout = (~a & b & Cin) | (a & ~b & Cin) | (a & b & ~Cin) | (a & b & Cin);
	assign sum = (~a & ~b & Cin) | (a & ~b & ~Cin) | (~a & b & ~Cin) | (a & b & Cin);

endmodule