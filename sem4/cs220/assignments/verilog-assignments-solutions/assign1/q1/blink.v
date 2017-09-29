`timescale 1ns / 1ps

module blink(clk, ledpin);
	input clk;
	
	output ledpin;
	reg ledpin = 0;

	//Set Counter Variable
	reg [25:0] counter = 50_000_000;	// 26 bit variable

	always @(posedge clk) begin
		if (counter == 0) begin
			counter <= 50_000_000;
			ledpin <= !ledpin;
		end
		else begin
			counter <= counter - 1;
		end
	end

endmodule