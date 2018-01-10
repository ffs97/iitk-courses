`timescale 1ns / 1ps

module alu4(in1, in2, opcode, ans, cf, zf, sf);
		
	input [3:0] in1, in2;
	input [2:0] opcode;

	output cf, zf, sf;
	output [3:0] ans;
		
	reg [26:0] count = 0;
	reg [5:0] code;
	reg refresh;
	reg [15:0] hexout;

	wire [3:0] carry;
	wire cin;
	assign cin = (opcode == 3);
	
	alu1 bit0 (.in1(in1[0]) , .in2(in2[0]) , .cin(cin) , .cout(carry[0]) , .opcode(opcode) , .out(ans[0]));
	alu1 bit1 (.in1(in1[1]) , .in2(in2[1]) , .cin(carry[0]) , .cout(carry[1]) , .opcode(opcode) , .out(ans[1]));
	alu1 bit2 (.in1(in1[2]) , .in2(in2[2]) , .cin(carry[1]) , .cout(carry[2]) , .opcode(opcode) , .out(ans[2]));
	alu1 bit3 (.in1(in1[3]) , .in2(in2[3]) , .cin(carry[2]) , .cout(carry[3]) , .opcode(opcode) , .out(ans[3]));

	assign cf = carry[3];
	assign zf = !(ans[0] || ans[1] || ans[2] || ans[3]);
	assign sf = ans[3];

endmodule