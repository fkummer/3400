//THIS PROJECT DEVELOPED BY CORNELL ECE 3400 STAFF. PLEASE
//DO NOT REUSE OR DISTRIBUTE THIS CODE WITHOUT PERMISSION
//SPRING 2015

module blockArray (
	clk,
	r_index,
	w_index,
	value,
	w_en,
	out,
	reset
);

input clk;
input [7:0] r_index; //x&y coords
input [7:0] w_index; //x&y coords
input [1:0] value; //write value
input			reset;
input w_en; //write enable
output [1:0] out; //gives r_index's value

reg [1:0] array[255:0]; //data array
reg [1:0] pre_out; //since out value is a wire

integer i;

always @(posedge clk) begin
	if (reset) begin
		for(i=0; i<255; i=i+1) begin
			array[i] <= 2'b0;
		end
	end else begin
		if (w_en == 1) begin //write value
			array[w_index] <= value;
		end
		//pre_out <= array[r_index]; // can use M9K
	end
end

//assign out = pre_out; // can use M9K
assign out = array[r_index]; // forces logic memory
endmodule
