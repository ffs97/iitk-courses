`timescale 1ns / 1ps

module alu1(in1, in2, cin, opcode, out, cout);
	 
	input wire in1, in2, cin;
	input [2:0] opcode;
	output wire out, cout;
	reg out_t , cout_t;
	
	always@*
	begin
		case(opcode[1:0])
			0: begin
				out_t <= in1 & in2;
				cout_t <= 0;
			end
			1: begin
				out_t <= in1 | in2;
				cout_t <= 0;
			end
			2: begin
				out_t <= (!in1 & !in2 & cin) | (in1 & !in2 & !cin) | (!in1 & in2 & !cin) | (in1 & in2 & cin);
				cout_t <= (!in1 & in2 & cin) | (in1 & !in2 & cin) | (in1 & in2 & !cin) | (in1 & in2 & cin);
			end	
			3: begin
				out_t <= (!in1 & in2 & cin) | (in1 & in2 & !cin) | (!in1 & !in2 & !cin) | (in1 & !in2 & cin);
				cout_t <= (!in1 & !in2 & cin) | (in1 & in2 & cin) | (in1 & !in2 & !cin) | (in1 & !in2 & cin);
			end
			default: out_t <= 0;
		endcase
	end
	
	assign out = out_t && !opcode[2];
	assign cout = cout_t && !opcode[2];

endmodule