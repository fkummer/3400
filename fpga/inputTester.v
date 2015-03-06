module inputTester(
X_COORD,
Y_COORD,
VALUE,
ENABLE
);

output	[3:0]	X_COORD;
output	[3:0]	Y_COORD;
output	[1:0] VALUE;
output	ENABLE;

wire	[3:0]	X_COORD;
wire	[3:0]	Y_COORD;
wire	[1:0] VALUE;
wire	ENABLE;



assign X_COORD = 4'b0;
assign Y_COORD = 4'b0;
assign VALUE = 2'b11;
assign ENABLE = 1;




endmodule