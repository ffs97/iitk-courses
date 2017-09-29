`timescale 1ns / 1ps

module eq2_test;

	reg [1:0] in1;
	reg [1:0] in2;

	wire eq;

	eq2 uut (
		.in1(in1), 
		.in2(in2), 
		.eq(eq)
	);

	initial begin
		in1 = 0;
		in2 = 0;

		#100;

      in1 = 2'b00;
      in2 = 2'b00;
		#1000;
		$display("Input1 = %b, Input2 = %b, Output = %b", in1, in2, eq);

      #100000;

      in1 = 2'b01;
      in2 = 2'b00;
		#1000;
		$display("Input1 = %b, Input2 = %b, Output = %b", in1, in2, eq);

      #100000;

      in1 = 2'b11;
      in2 = 2'b11;
		#1000;
		$display("Input1 = %b, Input2 = %b, Output = %b", in1, in2, eq);

      #100000;

      in1 = 2'b01;
      in2 = 2'b11;
		#1000;
		$display("Input1 = %b, Input2 = %b, Output = %b", in1, in2, eq);

      #100000;
		$stop;

	end
      
endmodule

