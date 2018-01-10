module eq1(in1, in2, eq);
	input in1, in2;
	output eq;

	assign eq = (!in1 && !in2) || (in1 && in2);
endmodule