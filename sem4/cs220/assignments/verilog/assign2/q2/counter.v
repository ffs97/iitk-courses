`timescale 1ns / 1ps

module counter (
		input clk,
		input wire, set, reset,
		input [3:0] in,
		output reg [3:0] counter
    );
	 
	reg [25:0] count = 50000000;

	always @(posedge clk)
		if (count == 0)
			begin
				count <= 50000000;
				counter <= counter + 1;
			end
		else
			begin
				if ( set == 1 )
					begin
						counter <= in;
						count <= 50000001;
					end
				
				if (reset == 1)
					begin
						counter <= 0;
						count <= 50000001;
					end

				count <= count - 1;
			end

endmodule
