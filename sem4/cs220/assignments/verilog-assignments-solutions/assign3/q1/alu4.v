`timescale 1ns / 1ps

module alu4(in, rr1, rr2, op, clk, out,
	sf_e, e, rs, rw, d, c, b, a);

	output reg sf_e;
	output reg e;
	output reg rs;
	output reg rw; 
	output reg d;
	output reg c;
	output reg b;
	output reg a;
	
	reg [26:0] count = 0;
	reg [5:0] code;
	reg refresh;

	input [3:0] in;
	input clk;
	input rr1, rr2, op;
	
	output [2:0] out;
	reg [2:0] out;
	wire [3:0] ans;
	wire [3:0] carry;
	reg [15:0] hexout;
	
	reg [1:0] opcode;
	reg [3:0] in1, in2;
	reg cin;
	
	alu1 bit0 (.in1(in1[0]), .in2(in2[0]), .cin(cin), .opcode(opcode), .out(ans[0]), .cout(carry[0]));
	alu1 bit1 (.in1(in1[1]), .in2(in2[1]), .cin(carry[0]), .opcode(opcode), .out(ans[1]), .cout(carry[1]));
	alu1 bit2 (.in1(in1[2]), .in2(in2[2]), .cin(carry[1]), .opcode(opcode), .out(ans[2]), .cout(carry[2]));
	alu1 bit3 (.in1(in1[3]), .in2(in2[3]), .cin(carry[2]), .opcode(opcode), .out(ans[3]), .cout(carry[3]));
	
	always@(posedge clk)
	begin
		count <= count + 1;
	
		if (rr1 == 1) begin
			in1 = in;
		end
		else if (rr2 == 1) begin
			in2 = in;
		end
		else if (op == 1) begin
			opcode = in[1:0];
			if (opcode == 3) begin
				cin <= 1;
			end
			else begin
				cin <= 0;
			end
		end
		
		case (ans[3:0])
			0: hexout = 16'h2320;
			1: hexout = 16'h2321;
			2: hexout = 16'h2322;
			3: hexout = 16'h2323;
			4: hexout = 16'h2324;
			5: hexout = 16'h2325;
			6: hexout = 16'h2326;
			7: hexout = 16'h2327;
			8: hexout = 16'h2328;
			9: hexout = 16'h2329;
			10: hexout = 16'h2421;
			11: hexout = 16'h2422;
			12: hexout = 16'h2423;
			13: hexout = 16'h2424;
			14: hexout = 16'h2425;
			15: hexout = 16'h2426;
			default: hexout = 0;
		endcase

		case (count [26:21])
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
		sf_e <= 1;
		e <= refresh; 
		rs <= code[5];
		rw <= code[4]; 
		d <= code[3];
		c <= code[2]; 
		b <= code[1];
		a <= code[0]; 
	end
	
	always@*
	begin
		out[0] <= carry[3];
		out[1] <= !(ans[0] || ans[1] || ans[2] || ans[3]);
		out[2] <= ans[3];
	end

endmodule