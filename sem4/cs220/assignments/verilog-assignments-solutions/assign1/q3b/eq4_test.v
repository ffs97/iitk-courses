`timescale 1ns / 1ps

module eq4_test;

	reg [3:0] in;
	reg push1;
	reg push2;
	reg clk;

	wire eq;

	// Instantiate the Unit Under Test (UUT)
	eq4 uut (
		.clk(clk), 
		.in(in), 
		.push1(push1), 
		.push2(push2), 
		.eq(eq)
	);

	initial begin
		clk = 0;
		in = 0;
		push1 = 0;
		push2 = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		$display("\nTest 1");
		in = 4'b0100;
		push1 = 1;
		#100000;
		$display("Input 1 = %b", in);
		push1 = 0;
		in = 4'b1000;
		push2 = 1;
		#100000;
		$display("Input 2 = %b", in);
		push2 = 0;
		#100000;
		$display("Output = %b", eq);
       
		#100000;
		 
		$display("\nTest 2");
		in = 4'b0110;
		push1 = 1;
		#100000;
		$display("Input 1 = %b", in);
		push1 = 0;
		in = 4'b0110;
		push2 = 1;
		#100000;
		$display("Input 2 = %b", in);
		push2 = 0;
		#100000;
		$display("Output = %b", eq);
       
		#100000;
		 
		$display("\nTest 3");
		in = 4'b0111;
		push1 = 1;
		#100000;
		$display("Input 1 = %b", in);
		push1 = 0;
		in = 4'b0011;
		push2 = 1;
		#100000;
		$display("Input 2 = %b", in);
		push2 = 0;
		#100000;
		$display("Output = %b", eq);
       
		#100000;
		 
		$display("\nTest 4");
		in = 4'b1111;
		push1 = 1;
		#100000;
		$display("Input 1 = %b", in);
		push1 = 0;
		in = 4'b1111;
		push2 = 1;
		#100000;
		$display("Input 2 = %b", in);
		push2 = 0;
		#100000;
		$display("Output = %b", eq);
		
		#100000;
		$stop;

	end
	
	always #10 clk = !clk;
      
endmodule

