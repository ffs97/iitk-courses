`timescale 1ns / 1ps
module alu1(in1, in2, cin, opcode, out, cout);
	 
	input in1, in2, cin;
	input [1:0] opcode;
	
	output out, cout;
	reg out, cout;
	
	always @*
	begin
		case(opcode)
			0: begin
				out <= in1 & in2;
				cout <= 0;
			end
			1: begin
				out <= in1 | in2;
				cout <= 0;
			end
			2: begin
				out <= (~in1 & ~in2 & cin) | (in1 & ~in2 & ~cin) | (~in1 & in2 & ~cin) | (in1 & in2 & cin);
				cout <= (~in1 & in2 & cin) | (in1 & ~in2 & cin) | (in1 & in2 & ~cin) | (in1 & in2 & cin);
			end	
			3: begin
				out <= (~in1 & in2 & cin) | (in1 & in2 & ~cin) | (~in1 & ~in2 & ~cin) | (in1 & ~in2 & cin);
				cout <= (~in1 & ~in2 & cin) | (in1 & in2 & cin) | (in1 & ~in2 & ~cin) | (in1 & ~in2 & cin);
			end
			default: begin
				out <= 0;
				cout <= 0;
			end
		endcase
		
	end

endmodule