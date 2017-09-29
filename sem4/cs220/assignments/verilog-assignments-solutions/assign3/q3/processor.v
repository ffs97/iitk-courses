`timescale 1ns / 1ps

module processor(in, op, rr1_wr, rr2_wd, we, 
	cf, zf, sf, inv, clk, 
	sf_e, e, rs, rw, d, c, b, a);
	input [3:0] in;
	input clk;
	input op, rr1_wr, rr2_wd, we;
	
	output cf, zf, sf, inv;
	reg inv;
	
	wire [3:0] ans;
	reg [2:0] opcode;
	reg [3:0] read_reg_1, read_reg_2;
	reg [3:0] write_reg, write_data;
	wire [3:0] rrd1, rrd2;
	
	regfile regfile_instance ( 
		.we(we), 
		.clk(clk), 
		.rr1(read_reg_1), 
		.rr2(read_reg_2), 
		.wr(write_reg), 
		.wd(write_data), 
		.rrd1(rrd1), 
		.rrd2(rrd2)
	);

	alu4 alu4_instance (
		.in1(rrd1), 
		.in2(rrd2), 
		.opcode(opcode), 
		.ans(ans), 
		.cf(cf), 
		.zf(zf), 
		.sf(sf) 
	);
	
	always@(posedge clk)
	begin
		if (op == 1) begin
			opcode = in;
		end
		else begin
			
			if (opcode == 3'b100) begin
				if (rr1_wr == 1) begin
					write_reg = in;
				end
				if (rr2_wd == 1) begin
					write_data = in;
				end
			end
			else if (opcode[2] == 0) begin
				if (rr1_wr == 1) begin
					read_reg_1 = in;
				end
				if (rr2_wd == 1) begin
					read_reg_2 = in;
				end
			end
			else begin
				inv = 1;
			end
			
		end
	end

	reg [26:0] count = 0;
	reg [15:0] hexout;
	reg [5:0] code;
	reg refresh;

	output reg sf_e; output reg e;
	output reg rs;
	output reg rw;
	output reg d; output reg c; output reg b; output reg a;

	always@(posedge clk)
	begin
		count <= count + 1;

		case (ans)
			00: hexout = 16'h2320;
			01: hexout = 16'h2321;
			02: hexout = 16'h2322;
			03: hexout = 16'h2323;
			04: hexout = 16'h2324;
			05: hexout = 16'h2325;
			06: hexout = 16'h2326;
			07: hexout = 16'h2327;
			08: hexout = 16'h2328;
			09: hexout = 16'h2329;
			10: hexout = 16'h2421;
			11: hexout = 16'h2422;
			12: hexout = 16'h2423;
			13: hexout = 16'h2424;
			14: hexout = 16'h2425;
			15: hexout = 16'h2426;
			default: hexout = 0;
		endcase

		case (count[26:21])
			0: code <= 6'h03;
			1: code <= 6'h03;
			2: code <= 6'h03;
			3: code <= 6'h02;
			4: code <= 6'h02;
			5: code <= 6'h08;
			6: code <= 6'h00;
			7: code <= 6'h06;
			8: code <= 6'h00;
			9: code <= 6'h0C;
			10: code <= 6'h00;
			11: code <= 6'h01;

			12: code <= hexout[13:8];
			13: code <= hexout[5:0];
			 
			default: code <= 6'h10;
		endcase 

		refresh <= count[20];
		
		sf_e <= 1; e <= refresh;

		rs <= code[5]; rw <= code[4];
		d <= code[3]; c <= code[2]; b <= code[1]; a <= code[0];

	end

endmodule
