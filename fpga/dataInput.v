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
input	[9:0] DATA_IN; //8 bit data from GPIO
input 		ENABLE_IN; //write enable from GPIO
input			RESET;

output [3:0] 	X_COORD; //coordinate for data
output [3:0] 	Y_COORD;
output [1:0] 	VALUE; //value at coordinate
output			ENABLE_OUT;

reg		state; //state to prevent continously rewriting same data
reg		ENABLE_OUT; //write enable to FPGA memory
wire		RESET;

//wire	parity;

assign X_COORD = DATA_IN[9:6];
assign Y_COORD = DATA_IN[5:2];
assign VALUE = DATA_IN[1:0];
//assign parity = DATA_IN[9]^DATA_IN[8]^DATA_IN[7]^DATA_IN[6]^DATA_IN[5]^DATA_IN[4]^DATA_IN[3]^DATA_IN[2]^DATA_IN[1]^DATA_IN[0]; //allows for parity detection to be added in future

always @(posedge CLOCK_50)begin
	if (RESET) begin
		ENABLE_OUT<=1'b0;
		state<=1'b1;
	end else if(state==1'b1) begin
		if(ENABLE_OUT==1'b1) begin
			ENABLE_OUT <= 1'b1; //only enable write to memory if this is first clock cycle that ENABLE_IN is high
			state <= 1'b0;
	end
	end else begin
		state <= ENABLE_IN==1'b1 ? 1'b0 : 1'b1;
		ENABLE_OUT <= 1'b0;
	end
end

endmodule