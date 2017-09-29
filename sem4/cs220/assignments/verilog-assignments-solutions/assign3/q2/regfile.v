`timescale 1ns / 1ps

module regfile(in, we, rr1, rr2, wr, wd, clk, rrd1, rrd2);
	input [3:0] in;
	input we, clk;
	input rr1, rr2, wr, wd;
	
	output [3:0] rrd1, rrd2;
	
	reg [3:0] read_register_1, read_register_2;
	reg [3:0] rrd1, rrd2;
	reg [3:0] write_register, write_data;
	
	reg [15:0] regfile[0:3];
	
	always@(posedge clk)
	begin
		if (wr == 1) begin
			write_register = in;
		end
		else if (wd == 1) begin
			write_data = in;
		end
		
		if (rr1 == 1) begin
			read_register_1 = in;
		end
		else if (rr2 == 1) begin
			read_register_2 = in;
		end
		
		if (we == 1) begin
		
			rrd1 = 0;
			rrd2 = 0;
			
			regfile[write_register] = write_data;
			
		end
		else begin
	
			rrd1 = regfile[read_register_1];
			rrd2 = regfile[read_register_2];
			
		end
	end

endmodule