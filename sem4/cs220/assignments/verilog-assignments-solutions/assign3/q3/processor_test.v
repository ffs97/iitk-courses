`timescale 1ns / 1ps

module processor_test;

	reg [3:0] in;
	reg op;
	reg rr1_wr;
	reg rr2_wd;
	reg we;
	reg clk;
	
	wire cf;
	wire zf;
	wire sf;
	wire sf_e;
	wire e;
	wire rs;
	wire rw;
	wire d;
	wire c;
	wire b;
	wire a;

	processor uut (
		.in(in), 
		.op(op), 
		.rr1_wr(rr1_wr), 
		.rr2_wd(rr2_wd), 
		.we(we), 
		.cf(cf), 
		.zf(zf), 
		.sf(sf), 
		.clk(clk), 
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
		op = 0;
		rr1_wr = 0;
		rr2_wd = 0;
		we = 0;
		clk = 0;

		#100;
        
		op = 1;
		in = 4'b0100;
		#100000;
		op = 0;
		#100;
		rr1_wr = 1;
		in = 4'b0000;
		#100000;
		rr1_wr = 0;
		#100;
		rr2_wd = 1;
		in = 4'b0101;
		#100000;
		rr2_wd = 0;
		#100;
		we = 1;
		#100000;
		we = 0;

		#100;
        
		op = 1;
		in = 4'b0100;
		#100000;
		op = 0;
		#100;
		rr1_wr = 1;
		in = 4'b0001;
		#100000;
		rr1_wr = 0;
		#100;
		rr2_wd = 1;
		in = 4'b1001;
		#100000;
		rr2_wd = 0;
		#100;
		we = 1;
		#100000;
		we = 0;
		
		#100;
        
		op = 1;
		in = 4'b0011;
		#100000;
		op = 0;
		#100;
		rr1_wr = 1;
		in = 4'b0001;
		#100000;
		rr1_wr = 0;
		#100;
		rr2_wd = 1;
		in = 4'b0000;
		#100000;
		rr2_wd = 0;
		
		#100;
		
		#100000;
		$stop;
	end
	
	always begin
		#10 clk = !clk;
	end
      
endmodule

