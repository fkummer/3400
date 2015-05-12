module dataInput(
CLOCK_50,
DATA_IN,
ENABLE_IN,
X_COORD,
Y_COORD,
VALUE,
ENABLE_OUT,
RESET
);

input			CLOCK_50;
input	[9:0] DATA_IN; //10 bit data from GPIO
input 		ENABLE_IN; //write enable from GPIO
input			RESET;

output [3:0] 	X_COORD; //coordinate for data
output [3:0] 	Y_COORD;
output [1:0] 	VALUE; //value at coordinate
output			ENABLE_OUT;

reg		state; //state to prevent continously rewriting same data
wire		ENABLE_OUT; //write enable to FPGA memory
wire		RESET;

//wire	parity;

assign X_COORD = DATA_IN[9:6];
assign Y_COORD = DATA_IN[5:2];
assign VALUE = DATA_IN[1:0];
assign ENABLE_OUT = ENABLE_IN;
//assign parity = DATA_IN[9]^DATA_IN[8]^DATA_IN[7]^DATA_IN[6]^DATA_IN[5]^DATA_IN[4]^DATA_IN[3]^DATA_IN[2]^DATA_IN[1]^DATA_IN[0]; //allows for parity detection to be added in future

endmodule
