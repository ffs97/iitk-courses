`timescale 1ns / 1ps

module alu4_test;

	reg [3:0] in;
	reg rr1;
	reg rr2;
	reg op;
	reg clk;

	wire [2:0] out;
	wire sf_e;
	wire e;
	wire rs;
	wire rw;
	wire d;
	wire c;
	wire b;
	wire a;

	alu4 uut (
		.in(in), 
		.rr1(rr1), 
		.rr2(rr2), 
		.op(op), 
		.clk(clk), 
		.out(out), 
		.sf_e(sf_e), 
		.e(e), 
		.rs(rs), 
		.rw(rw), 
		.d(d), 
		.c(c), 
		.b(b), 
		.a(a)
	);

	initial begin
		in = 0;
		rr1 = 0;
		rr2 = 0;
		op = 0;
		clk = 0;

		#100;
		
      op = 1;
		in = 4'b0000;
		#100000;
		op = 0;
		#100
		in = 4'b1101;
		rr1 = 1;
		#100000;
		rr1 = 0;
		#100;
		in = 4'b0101;
		rr2 = 1;
		#100000;
		rr2 = 0;
		
		#100000;
		
		$stop;
	end
	
	always #10 clk = !clk;
      
endmodule

