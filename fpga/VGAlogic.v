
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

//THIS PROJECT DEVELOPED BY CORNELL ECE 3400 STAFF. PLEASE
//DO NOT REUSE OR DISTRIBUTE THIS CODE WITHOUT PERMISSION
//SPRING 2015

module VGAlogic(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// KEY //////////
	KEY,

	////// GPIO_0, GPIO_0 connect to GPIO Default //////
	GPIO,
	GPIO_IN, 
	GPIO_1,
	GPIO_1_IN
);


//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK //////////
input 		          		CLOCK_50;

//////////// KEY //////////
input 		     [1:0]		KEY;

///// GPIO_0, GPIO_0 connect to GPIO Default //////////
inout 		    [33:0]		GPIO;
input 		     [1:0]		GPIO_IN;
inout				 [33:0]		GPIO_1;
input				  [1:0]		GPIO_1_IN;


//=======================================================
//  REG/WIRE declarations
//=======================================================
reg CLOCK_25;
wire [9:0] PIXEL_COORD_X;
wire [9:0] PIXEL_COORD_Y;
wire [7:0] PIXEL_COLOR;

wire reset;
wire [1:0] pixel_in;
wire [7:0] pixel_r_index;
wire [7:0] pixel_w_index;
wire [1:0] pixel_out;
wire pixel_wen;


wire [3:0]	pixel_w_x;
wire [3:0]	pixel_w_y;


VGADriver driver(
	.CLOCK(CLOCK_25),
	.PIXEL_COLOR_IN(PIXEL_COLOR),
	.PIXEL_X(PIXEL_COORD_X),
	.PIXEL_Y(PIXEL_COORD_Y),
	.PIXEL_COLOR_OUT({GPIO[9],GPIO[11],GPIO[13],GPIO[15],GPIO[17],GPIO[19],GPIO[21],GPIO[23]}),
	.H_SYNC_NEG(GPIO[7]),
	.V_SYNC_NEG(GPIO[5])
);

blockArray pixelArray(
	.clk(CLOCK_50),
	.r_index(pixel_r_index),
	.w_index(pixel_w_index),
	.value(pixel_in),
	.w_en(pixel_wen),
	.out(pixel_out),
	.reset(reset)
);



dataInput arduino(
	.CLOCK_50(CLOCK_50),
	.DATA_IN({GPIO_1[20],GPIO_1[21],GPIO_1[18],GPIO_1[19],GPIO_1[16],GPIO_1[17],GPIO_1[14],GPIO_1[15],GPIO_1[12],GPIO_1[13]}),
	//.DATA_IN(10'b0000000010),
	.ENABLE_IN(GPIO_1[9]),
	.X_COORD(pixel_w_x),
	.Y_COORD(pixel_w_y),
	.VALUE(pixel_in),
	.ENABLE_OUT(pixel_wen),
	.RESET(reset)
);

/*
inputTester TEST(
	.X_COORD(pixel_w_x),
	.Y_COORD(pixel_w_y),
	.VALUE(pixel_in),
	.ENABLE(pixel_wen)
);
*/
/*
inputGenerator tester(
	.clk(CLOCK_50),
	.X_COORD(pixel_w_x),
	.Y_COORD(pixel_w_y),
	.VALUE(pixel_in),
	.ENABLE(pixel_wen),
	.reset(reset),
	.click(KEY[1])
);
*/



assign pixel_w_index [7:4] = pixel_w_x;
assign pixel_w_index [3:0] = pixel_w_y;

assign pixel_r_index[7:4] = PIXEL_COORD_X[8:5]; //shift by 5 for pixel->block transform
assign pixel_r_index[3:0] = PIXEL_COORD_Y[8:5];

//=======================================================
//  Structural coding
//=======================================================
always @(posedge CLOCK_50) begin
	CLOCK_25 <= ~CLOCK_25; //VGA needs 25 MHz clock - FPGA has 50 MHz clock
end

assign reset = ~KEY[0];

blockFormat Formatter(
	.X_COORD(PIXEL_COORD_X),
	.Y_COORD(PIXEL_COORD_Y),
	.value(pixel_out),
	.color(PIXEL_COLOR)
);



//translate 2 bit pixel array values to 8 bit RGB
// 8'bGGG_RRR_BB 
/*
assign PIXEL_COLOR = (PIXEL_COORD_X[9]==1) ?  8'b000_000_00 : //black if x>512
							(pixel_out == 2'b00)? 8'b001_011_11 : //full red
							(pixel_out == 2'b01)? 8'b000_111_00 : // 1/2 green
							(pixel_out == 2'b10)? 8'b110_000_01 : // 1/4 blue
								8'b101_101_00;
								*/
							/*(pixel_out == 2'b11 && PIXEL_COORD_X < 200)? 8'b100_100_01 : // gray
							(pixel_out == 2'b11 && PIXEL_COORD_X < 400)? 8'b111_111_11 : // white
																						8'b111_111_00 ; // yellow
																						*/

endmodule
