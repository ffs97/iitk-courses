`timescale 1ns / 1ps

module ad44bitTest;

	reg b1;
	reg b2;
	reg clk;
	reg [3:0] in;
	wire out;
	wire sf_e;
	wire e;
	wire rs;
	wire rw;
	wire d;
	wire c;
	wire b;
	wire a;

	add4bit uut (
		.b1(b1),
		.b2(b2),
		.clk(clk),
		.in(in),
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
		push1 = 0;
		push2 = 0;
		clk = 0;
		in = 0;

		#100;
        
		in = 4'b0010;
		#100
		push1 = 1;
		#1000000;
		push1 = 0;
		#100
		in = 4'b1111;
		#100;
		push2 = 1;
		#1000000;
		push2 = 0;
		
		#100000;
		
		in = 4'b0000;
		#100
		push1 = 1;
		#1000000;
		push1 = 0;
		#100
		in = 4'b1111;
		#100;
		push2 = 1;
		#1000000;
		push2 = 0;

		#100000;
		
		in = 4'b0011;
		push1 = 1;
		#1000000;
		push1 = 0;
		in = 4'b0011;
		#100;
		push2 = 1;
		#1000000;
		push2 = 0;

		#100000;
		
		in = 4'b0001;
		push1 = 1;
		#1000000;
		push1 = 0;
		in = 4'b0001;
		#100;
		push2 = 1;
		#1000000;
		push2 = 0;
		
		#100000;
		$stop;
	end
	
	always begin
		#10 clk = !clk;
	end
      
endmodule

