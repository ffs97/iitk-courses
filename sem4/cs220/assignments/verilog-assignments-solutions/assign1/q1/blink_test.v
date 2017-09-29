`timescale 1ns / 1ps

module blinkTest;
	reg clk;
	reg ledpin;

	blink uut (
		.clk(clk),
		.ledpin(ledpin)
	);

	initial begin
		clk = 0;

		#100;
		
	end

	always begin
		#10 clk = !clk;
	end

endmodule