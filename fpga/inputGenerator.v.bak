module inputGenerator(
clk,
X_COORD,
Y_COORD,
VALUE,
ENABLE,
reset
);

input			clk;
input			reset;

output	[3:0]	X_COORD;
output	[3:0]	Y_COORD;
output	[1:0] VALUE;
output	ENABLE;

reg 		[3:0] X_COORD;
reg		[3:0] Y_COORD;
reg		ENABLE;

reg		[15:0] counter;

wire		VALUE;

assign VALUE = 2'b10;


always @(posedge clk) begin
		if(reset) begin
			counter=16'b0;
			X_COORD=4'b0;
			Y_COORD=4'b0;
		end else begin
			counter <= counter+1;
			if(counter==16'hffff) begin
				X_COORD <= X_COORD+1;
				ENABLE <= 1'b1;
				if(X_COORD==4'b1111) begin
					Y_COORD <= Y_COORD+1;
				end
			end else begin
				ENABLE <= 1'b0;
			end
		end
end


endmodule
