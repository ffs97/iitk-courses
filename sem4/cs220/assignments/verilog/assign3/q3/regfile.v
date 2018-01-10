`timescale 1ns / 1ps

module regfile(we, rr1, rr2, clk, wr, wd, rrd1, rrd2);
	input we, clk;
	input [3:0] rr1, rr2, wr, wd;
	
	output [3:0] rrd1, rrd2;
	reg [3:0] rrd1, rrd2;
	
	reg [3:0] regfile[0:15];
	
	always@(posedge clk)
	begin	
		if (we == 1) begin
		
			rrd1 = 0;
			rrd2 = 0;
			
			regfile[wr] = wd;
			
		end
		else begin

			rrd1 = regfile[rr1];
			rrd2 = regfile[rr2];

		end
	end

endmodule