module inputGenerator(
clk,
X_COORD,
Y_COORD,
VALUE,
ENABLE,
reset,
click
);

input			clk;
input			reset;
input 		click;

output	[3:0]	X_COORD;
output	[3:0]	Y_COORD;
output	[1:0] VALUE;
output	ENABLE;

reg 		[3:0] X_COORD;
reg		[3:0] Y_COORD;
reg		ENABLE;

reg		[23:0] counter;

wire		[1:0] VALUE;

assign VALUE = 2'b01;


always @(posedge clk) begin
		if(reset) begin
			counter=24'b0;
			X_COORD=4'b0;
			Y_COORD=4'b0;
		end else begin
			counter <= counter-24'b1;
			if(counter==16'd0) begin
				ENABLE <= 1'b1;
				if(X_COORD==4'b1010) begin
					X_COORD <= 4'b0;
					if(Y_COORD == 4'b1000) begin
						Y_COORD <= 4'b0000;
					end else begin
						Y_COORD <= Y_COORD+4'b01;
					end
				end else begin
					X_COORD <= X_COORD+4'b1;
				end
			end else begin
				ENABLE <= 1'b0;
			end
		end
end

/*
always @(negedge click) begin
		if(reset) begin
			X_COORD=4'b0;
			Y_COORD=4'b0;
		end else begin
			ENABLE <= 1'b1;
					if(X_COORD==4'b1010) begin
						Y_COORD <= Y_COORD+4'b01;
						X_COORD <= 4'b0;
					end else begin
						X_COORD <= X_COORD+4'b1;
			end
		end
end
*/

endmodule
