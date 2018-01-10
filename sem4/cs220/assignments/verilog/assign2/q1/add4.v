`timescale 1ns / 1ps

module add4 (
	push1, push2, clk, in, out,
	sf_e, e, rs, rw, d, c, b, a);

	output reg sf_e;
	output reg e;
	output reg rs;
	output reg rw;
	output reg d;
	output reg c;
	output reg b;
	output reg a;

	input wire push1, push2, clk;
	input [3:0] in;
	output wire out;

	reg [26:0] count = 0;
	reg [5:0] code;
	reg refresh;
	wire carry, temp1, temp2, temp3;
	reg [3:0] x,y;
	wire [3:0] sum;
	reg [15:0] out;

	always @(posedge clk)
		begin
			if (push1 == 1)
				x = in;
			if (push2 == 1)
				y = in;
		end

	add1 bit1 (.Cin(0), .a(x[0]), .b(y[0]), .Cout(temp1), .sum(sum[0]));
	add1 bit2 (.Cin(temp1), .a(x[1]), .b(y[1]), .Cout(temp2), .sum(sum[1]));
	add1 bit3 (.Cin(temp2), .a(x[2]), .b(y[2]), .Cout(temp3), .sum(sum[2]));
	add1 bit4 (.Cin(temp3), .a(x[3]), .b(y[3]), .Cout(carry), .sum(sum[3]));

	assign out = temp;

	always @ (posedge clk) begin
	count <= count + 1;

		case (sum [3:0])
			0: out = 16'h2320;
			1: out = 16'h2321;
			2: out = 16'h2322;
			3: out = 16'h2323;
			4: out = 16'h2324;
			5: out = 16'h2325;
			6: out = 16'h2326;
			7: out = 16'h2327;
			8: out = 16'h2328;
			9: out = 16'h2329;
			10: out = 16'h2421;
			11: out = 16'h2422;
			12: out = 16'h2423;
			13: out = 16'h2424;
			14:out = 16'h2425;
			15:out = 16'h2426;
			default: out = 0;
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

			12: code <= out[13:8];
			13: code <= out[5:0];
			 
			default: code <= 6'h10; 
		endcase

		refresh <= count[ 20 ];
		sf_e <= 1;
		e <= refresh; 
		rs <= code[5];
		rw <= code[4]; 
		d <= code[3];
		c <= code[2]; 
		b <= code[1];
		a <= code[0]; 
	end

endmodule
