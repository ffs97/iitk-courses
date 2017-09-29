`timescale 1ns / 1ps

module printLCD(clk, sf_e, e, rs, rw, d, c, b, a);

	(* LOC = "C9" *)  input clk; // pin C9 is the 50-MHz on-board clock

	(* LOC = "D16" *) output reg sf_e; // 1 LCD access (0 strataFlash access)
	(* LOC = "M18" *) output reg e; // enable (1)
	(* LOC = "L18" *) output reg rs;	// Register Select (1 data bits for R/W)
	(* LOC = "L17" *) output reg rw;	// Read/Write 1/0
	
	(* LOC = "M15" *) output reg d;	// 4th data bits (to form a nibble)
	(* LOC = "P17" *) output reg c;	// 3rd data bits (to form a nibble)
	(* LOC = "R16" *) output reg b;	// 2nd data bits (to form a nibble)
	(* LOC = "R15" *) output reg a;	// 1st data bits (to form a nibble)

	reg [26:0] count = 0;	// 27-bit count
	reg [5:0] code;	// 6-bits different signals to give out
	reg refresh;	// refresh LCD rate @ about 25Hz


	always @ (posedge clk) begin
		count <= count + 1;
		

		case (count [26:21])	// as top 6 bits change

			// Initialization of the LCD
				0: code <= 6'h03;	// power-on init sequence
				1: code <= 6'h03;
				2: code <= 6'h03;	// when LCD's powered on
				3: code <= 6'h02;	// it flickers existing char dislay

			// Function Set
				4: code <= 6'h02; // upper nibble 0010 
				5: code <= 6'h08; // lower nibble 1000 (10xx)

			// Entry Mode Set
				6: code <= 6'h00; // upper nibble 0000 
				7: code <= 6'h06; // lower nibble 0110: Incr, Shift disabled

			// Display On/Off
				// D: 1, show char represented by code in DDR, 0 don't, but code remains 
				// C: 1, show cursor, 0 don't 
				// B: 1, cursor blinks (if shown), 0 don't blink (if shown) 
				8: code <= 6'h00; // upper nibble 0000 
				9: code <= 6'h0C; // lower nibble 1100 (1 D C B)

			// Clear Display 
				10: code <= 6'h00; //upper nibble 0000 
				11: code <= 6'h01; //lower nibble 0001

			// Write Data to RAM (first line)
				//C
					12: code <= 6'h24; // upper nibble 0100
					13: code <= 6'h23; // lower nibble 0011
				//o
					14: code <= 6'h26; // upper nibble 0110
					15: code <= 6'h2F; // lower nibble 1111
				//m
					16: code <= 6'h26; // upper nibble 0110
					17: code <= 6'h2D; // lower nibble 1101
				//p
					18: code <= 6'h27; // upper nibble 0111
					19: code <= 6'h20; // lower nibble 0000
				//u
					20: code <= 6'h27; // upper nibble 0111
					21: code <= 6'h25; // lower nibble 0101
				//t
					22: code <= 6'h27; // upper nibble 0111
					23: code <= 6'h24; // lower nibble 0100
				//e
					24: code <= 6'h26; // upper nibble 0110
					25: code <= 6'h25; // lower nibble 0101
				//r
					26: code <= 6'h27; // upper nibble 0111
					27: code <= 6'h22; // lower nibble 0010

			//Set DD RAM (DDR) Address to end line
				28: code <= 6'h0C; // upper nibble 1100
				29: code <= 6'h00; // lower nibble 0000

			// Write Data to RAM (first line)
				//O
					30: code <= 6'h24; // upper nibble 0100
					31: code <= 6'h2F; // lower nibble 1111
				//r
					32: code <= 6'h27; // upper nibble 0111
					33: code <= 6'h22; // lower nibble 0010
				//g
					34: code <= 6'h26; // upper nibble 0110
					35: code <= 6'h27; // lower nibble 0111
				//a
					36: code <= 6'h26; // upper nibble 0110
					37: code <= 6'h21; // lower nibble 0001
				//n
					38: code <= 6'h26; // upper nibble 0110
					39: code <= 6'h2E; // lower nibble 1110
				//i
					40: code <= 6'h26; // upper nibble 0110
					41: code <= 6'h29; // lower nibble 1001
				//z
					42: code <= 6'h27; // upper nibble 0111
					43: code <= 6'h2A; // lower nibble 1010
				//a
					44: code <= 6'h26; // upper nibble 0110
					45: code <= 6'h21; // lower nibble 0001
				//t
					46: code <= 6'h27; // upper nibble 0111
					47: code <= 6'h24; // lower nibble 0100
				//i
					48: code <= 6'h26; // upper nibble 0110
					49: code <= 6'h29; // lower nibble 1001
				//o
					50: code <= 6'h26; // upper nibble 0110
					51: code <= 6'h2F; // lower nibble 1111
				//n
					52: code <= 6'h26; // upper nibble 0110
					53: code <= 6'h2E; // lower nibble 1110

			// Read Busy Flag and Address  (set busy to 1)
				default: code <= 6'h10;
		endcase 

		// flip refresh (enable) the LCD when bit 20 of the count is changed
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
