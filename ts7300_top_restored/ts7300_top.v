/* Copyright 2005-2006, Technologic Systems
* All Rights Reserved.
*
* Author(s): Jesse Off <joff@...>
*
* Boilerplate Verilog for use in Technologic Systems TS-7300 FPGA computer
* at http://www.embeddedarm.com/epc/ts7300-spec-h.htm. Implements bus cycle
* demultiplexing to an internal 16 and 32 bit WISHBONE bus, and 10/100
* ethernet interfrootace.
*
* Full-featured FPGA bitstream from Technologic Systems includes "TS-SDCORE"
* SD card core, 8 "TS-UART" serial ports, "TS-VIDCORE" VGA video framebuffer and
* accelerator, and 2 PWM/Timer/Counter "TS-XDIO" cores for the various GPIO
* pins. Binary bitstream comes with board. Contact Technologic Systems
* for custom FPGA development on the TS-7300 or for non-GPL licensing of this
* or any of the above (not-included-here) TS-cores and OS drivers.
*
* To load the bitstream to the FPGA on the TS-7300, Technologic Systems provides
* a Linux program "load_ts7300" that takes the ts7300_top.rbf file generated
* by Altera's Quartus II on the Linux flash filesystem (yaffs, ext2,
* jffs2, etc..) and loads the FPGA. Loading the FPGA takes approx 0.2 seconds
* this way and can be done (and re-done) at any time during power-up without
* any special JTAG/ISP cables.
*/

/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License v2 as published by
* the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/* This module is a sample dummy stub that can be filled in by the user. Any access's on
* the TS-7300 CPU for address 0x72a00000 to 0x72fffffc arrive here. Keep in mind
* the address is a word address not the byte address and address 0x0 is 0x72000000.
* The interface used here is the WISHBONE bus, described in detail on
* http://www.opencores.org
*
* There is a 40-pin header next to the FPGA. It is broken up into 2 20 pin
* connectors. One is labeled DIO2 and contains the 18 dedicated GPIO pins. The
* other contains 17 signals that are used by the TS-VIDCORE but can also be used
* as GPIO if video is not used. DO NOT DRIVE THESE SIGNALS OVER 3.3V!!! They
* go straight into the FPGA pads unbuffered.
* ___________________________________________________________
* | 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40|
* | 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39|
* \-----------------------------------------------------------/
* * | * DIO2
*
* pins #2 and #22 are grounds
* pin #20 is fused 5V (polyfuse)
* pin #40 is regulated 3.3V
* pin #18 can be externally driven high to disable DB15 VGA connector DACs
* pin #36 and #38 also go to the red and green LEDs (active low)
* pin #39 is a dedicated clock input and cannot be programmed for output
*
*/

/***********************************
 *
 * UserCore modified version. Modification author 
 * nigel.gunton@uwe.ac.uk January 2008, April 2009.
 * 
 * Modified to provide bi-directional data on the header pins by the activation 
 * of the oe register and alteration of the usercore into vga, dio and misc 
 * registers.
 * 
 */

/******************************************************************
 * Stub module removed and substituted with ts_7300_usercore.vhd
 * which is easier if you are more familiar with vhdl. The mapping 
 * (component/instance) is near the bottom of this file. 
 * Also dummyreg was undeclared anywhere so needed to be added to usercore.
 * 
 * 
 * ***************************************************************/

/* Now begins the real guts of the TS-7300 Cyclone2 EP2C8 FPGA
* boilerplate. You should only have to look below here if the above
* stub module isn't enough for you.
*/
module ts7300_top(
		  fl_d_pad,
		  bd_pad,
		  isa_add11_pad,
		  isa_add12_pad,
		  isa_add14_pad,
		  isa_add15_pad,
		  isa_add1_pad,
		  add_pad,
		  start_cycle_pad,
		  bd_oe_pad,
		  dio0to8_pad,
		  dio9_pad,
		  dio10to17_pad,
		  sdram_data_pad,
		  clk_25mhz_pad,
		  clk_75mhz_pad,
		  isa_wait_pad,
		  dma_req_pad,
		  irq7_pad,
		  sd_soft_power_pad,
		  sd_hard_power_pad,
		  sd_wprot_pad,
		  sd_present_pad,
		  sd_dat_pad,
		  sd_clk_pad,
		  sd_cmd_pad,
		  eth_mdio_pad,
		  eth_mdc_pad,
		  eth_rxdat_pad,
		  eth_rxdv_pad,
		  eth_rxclk_pad,
		  eth_rxerr_pad,
		  eth_txdat_pad,
		  eth_txclk_pad,
		  eth_txen_pad,
		  eth_txerr_pad,
		  eth_col_pad,
		  eth_crs_pad,
		  eth_pd_pad,
		  sdram_add_pad,
		  sdram_ras_pad,
		  sdram_cas_pad,
		  sdram_we_pad,
		  sdram_ba_pad,
		  sdram_clk_pad,
		  wr_232_pad,
		  rd_mux_pad,
		  mux_cntrl_pad,
		  mux_pad,
		  blue_pad,
		  red_pad,
		  green_pad,
		  hsync_pad,
		  vsync_pad
		  );

   inout [7:0] fl_d_pad;       /* to/from max2 */
   inout [7:0] bd_pad;	       /* to/from arm sdram */
   input       isa_add11_pad;  /* buffered/muxed from add11 */
   input       isa_add12_pad;  /* ditto add12*/
   input       isa_add14_pad;  /* ditto add14*/
   input       isa_add15_pad;  /* ditto add15*/
   input       isa_add1_pad;   /* is this misnamed? isa_add01 ? from max2? check in pin file*/
   input [3:0] add_pad;        /* add17, add18, add19, add20 from arm9*/
   input       start_cycle_pad;  /* bus cycle start from max2 (equ wb_cyc)  */
   input       bd_oe_pad;
   inout [8:0] dio0to8_pad;
   input 	   dio9_pad;
   inout [7:0] dio10to17_pad;
   inout [15:0]    sdram_data_pad;
   input 	   clk_25mhz_pad;
   output 	   clk_75mhz_pad;
   inout 	   isa_wait_pad;
   inout 	   dma_req_pad;
   inout 	   irq7_pad;
   output 	   sd_soft_power_pad;
   output 	   sd_hard_power_pad;
   input 	   sd_wprot_pad;
   input 	   sd_present_pad;
   inout [3:0] 	   sd_dat_pad;
   output 	   sd_clk_pad;
   inout 	   sd_cmd_pad;
   inout 	   eth_mdio_pad;
   output 	   eth_mdc_pad;
   input [3:0] 	   eth_rxdat_pad;
   input 	   eth_rxdv_pad;
   input 	   eth_rxclk_pad;
   input 	   eth_rxerr_pad;
   output [3:0]    eth_txdat_pad;
   input 	   eth_txclk_pad;
   output 	   eth_txen_pad;
   output 	   eth_txerr_pad;
   input 	   eth_col_pad;
   input 	   eth_crs_pad;
   output 	   eth_pd_pad;
   output [12:0]   sdram_add_pad;
   output 	   sdram_ras_pad;
   output 	   sdram_cas_pad;
   output 	   sdram_we_pad;
   output [1:0]    sdram_ba_pad;
   output 	   wr_232_pad;
   output 	   rd_mux_pad;
   output 	   mux_cntrl_pad;
   inout [3:0] 	   mux_pad;
   inout  [4:0] blue_pad;
   inout  [4:0] red_pad;
   inout [4:0] green_pad;
   inout  	   hsync_pad;
   inout  	   vsync_pad;
   output 	   sdram_clk_pad;

   /* Set to 1'b0 to disable ethernet. If you disable this, don't
    * attempt to load the ethernet driver module! */
   parameter 	   ethernet = 1'b0;

   /* Bus cycles from the ep9302 processor come in to the FPGA multiplexed by
    * the MAX2 CPLD on the TS-7300. Any access on the ep9302 for addresses
    * 0x72000000 - 0x72ffffff are routed to the FPGA. The ep9302 CS7 SMCBCR register
    * at 0x8008001c physical should be set to 0x10004508 -- 16-bit,
    * ~120 nS bus cycle. The FPGA must be loaded and sending 75Mhz to the MAX2
    * on clk_75mhz_pad before any bus cycles are attempted.
    *
    * Since the native multiplexed bus is a little unfriendly to deal with
    * and non-standard, as our first order of business we translate it into
    * something more easily understood and better documented: a 16 bit WISHBONE bus.
    */

   reg 		   epwbm_done, epwbm_done32;
   reg 		   isa_add1_pad_q;
   reg [23:0] 	   ep93xx_address;
   reg 		   epwbm_we_o, epwbm_stb_o;
   wire [23:0] 	   epwbm_adr_o;
   reg [15:0] 	   epwbm_dat_o;
   reg [15:0] 	   epwbm_dat_i;
   reg [15:0] 	   ep93xx_dat_latch;
   reg 		   epwbm_ack_i;
   wire 	   epwbm_clk_o = clk_75mhz_pad;
   wire 	   epwbm_cyc_o = start_cycle_posedge_q;
   wire 	   ep93xx_databus_oe = !epwbm_we_o && start_cycle_posedge && !bd_oe_pad;
   wire 	   pll_locked, clk_150mhz;
   wire 	   epwbm_rst_o = !pll_locked;

   assign 	   fl_d_pad[7:0] = ep93xx_databus_oe ?ep93xx_dat_latch[7:0] : 8'hzz;
   assign 	   bd_pad[7:0] = ep93xx_databus_oe ?ep93xx_dat_latch[15:8] : 8'hzz;
   assign 	   isa_wait_pad = start_cycle_negedge ? epwbm_done : 1'bz;
   assign 	   epwbm_adr_o[23:2] = ep93xx_address[23:2];
   reg 		   ep93xx_address1_q;
   assign 	   epwbm_adr_o[0] = ep93xx_address[0];
   assign 	   epwbm_adr_o[1] = ep93xx_address1_q;

   /* Use Altera's PLL to multiply 25Mhz from the ethernet PHY to 75Mhz */
   pll clkgencore(
		  .inclk0(clk_25mhz_pad),
		  .c0(clk_150mhz),
		  .c1(clk_75mhz_pad),
		  .locked(pll_locked)
		  );

   reg 		   ep93xx_end, ep93xx_end_q;
   reg 		   start_cycle_negedge, start_cycle_posedge, bd_oe_negedge,
		   bd_oe_posedge;
   reg 		   start_cycle_negedge_q, start_cycle_posedge_q;
   reg 		   bd_oe_negedge_q, bd_oe_posedge_q;

   always @(posedge clk_75mhz_pad) 
     begin
	start_cycle_negedge_q <= start_cycle_negedge;
	start_cycle_posedge_q <= start_cycle_posedge;
	bd_oe_negedge_q <= bd_oe_negedge;
	bd_oe_posedge_q <= bd_oe_posedge;
	isa_add1_pad_q <= isa_add1_pad;
	
	if ((bd_oe_negedge_q && epwbm_we_o) ||
	    (start_cycle_posedge_q && !epwbm_we_o) && !epwbm_done) 
	  begin
	     epwbm_stb_o <= 1'b1;
	     ep93xx_address1_q <= isa_add1_pad_q;
	     epwbm_dat_o <= {bd_pad[7:0], fl_d_pad[7:0]};
	  end
	
	if (epwbm_stb_o && epwbm_ack_i) 
	  begin
	     epwbm_stb_o <= 1'b0;
	     epwbm_done <= 1'b1;
	     ep93xx_dat_latch <= epwbm_dat_i;
	  end
	
	if (epwbm_done && !epwbm_done32 && 
	    (ep93xx_address[1] !=isa_add1_pad_q)) 
	  begin
	     epwbm_done <= 1'b0;
	     epwbm_done32 <= 1'b1;
	  end

	ep93xx_end_q <= 1'b0;
	
	if ((start_cycle_negedge_q && start_cycle_posedge_q &&
	     bd_oe_negedge_q && bd_oe_posedge) || !pll_locked) 
	  begin
	     ep93xx_end <= 1'b1;
	     ep93xx_end_q <= 1'b0;
	  end

	if (ep93xx_end) 
	  begin
	     ep93xx_end <= 1'b0;
	     ep93xx_end_q <= 1'b1;
	     epwbm_done32 <= 1'b0;
	     epwbm_stb_o <= 1'b0;
	     epwbm_done <= 1'b0;
	     start_cycle_negedge_q <= 1'b0;
	     start_cycle_posedge_q <= 1'b0;
	     bd_oe_negedge_q <= 1'b0;
	     bd_oe_posedge_q <= 1'b0;
	  end
     end

   wire start_cycle_negedge_aset = !start_cycle_pad && pll_locked;
   always @(posedge ep93xx_end_q or posedge start_cycle_negedge_aset) 
     begin
	if (start_cycle_negedge_aset) 
	  start_cycle_negedge <= 1'b1;
	else start_cycle_negedge <= 1'b0;
     end

   always @(posedge start_cycle_pad or posedge ep93xx_end_q) 
     begin
	if (ep93xx_end_q) 
	  start_cycle_posedge <= 1'b0;
	else 
	  if (start_cycle_negedge) 
	    start_cycle_posedge <= 1'b1;
     end

   always @(posedge start_cycle_pad) 
     begin
	epwbm_we_o <= fl_d_pad[7];
	ep93xx_address[23] <= fl_d_pad[0];
	ep93xx_address[22] <= fl_d_pad[1];
	ep93xx_address[21] <= fl_d_pad[2];
	ep93xx_address[20:17] <= add_pad[3:0];
	ep93xx_address[16] <= fl_d_pad[3];
	ep93xx_address[15] <= isa_add15_pad;
	ep93xx_address[14] <= isa_add14_pad;
	ep93xx_address[13] <= fl_d_pad[4];
	ep93xx_address[12] <= isa_add12_pad;
	ep93xx_address[11] <= isa_add11_pad;
	ep93xx_address[10] <= bd_pad[0];
	ep93xx_address[9] <= bd_pad[1];
	ep93xx_address[8] <= bd_pad[2];
	ep93xx_address[7] <= bd_pad[3];
	ep93xx_address[6] <= bd_pad[4];
	ep93xx_address[5] <= bd_pad[5];
	ep93xx_address[4] <= bd_pad[6];
	ep93xx_address[3] <= bd_pad[7];
	ep93xx_address[2] <= fl_d_pad[5];
	ep93xx_address[1] <= isa_add1_pad;
	ep93xx_address[0] <= fl_d_pad[6];
     end

   always @(negedge bd_oe_pad or posedge ep93xx_end_q) 
     begin
	if (ep93xx_end_q) 
	  bd_oe_negedge <= 1'b0;
	else 
	  if (start_cycle_posedge) 
	    bd_oe_negedge <= 1'b1;
     end

   always @(posedge bd_oe_pad or posedge ep93xx_end_q) 
     begin
	if (ep93xx_end_q) 
	  bd_oe_posedge <= 1'b0;
	else 
	  if (bd_oe_negedge) 
	    bd_oe_posedge <= 1'b1;
     end

   wire [15:0] epwbm_wb32m_bridgecore_dat;
   wire        epwbm_wb32m_bridgecore_ack;
   wire [31:0] wb32m_dat_o;
   reg [31:0]  wb32m_dat_i;
   wire [21:0] wb32m_adr_o;
   wire [3:0]  wb32m_sel_o;
   wire        wb32m_cyc_o, wb32m_stb_o, wb32m_we_o;
   reg 	       wb32m_ack_i;
   wire        wb32m_clk_o = epwbm_clk_o;
   wire        wb32m_rst_o = epwbm_rst_o;
   wb32_bridge epwbm_wb32m_bridgecore
     (
      .wb_clk_i(epwbm_clk_o),
      .wb_rst_i(epwbm_rst_o),
      
      .wb16_adr_i(epwbm_adr_o[23:1]),
      .wb16_dat_i(epwbm_dat_o),
      .wb16_dat_o(epwbm_wb32m_bridgecore_dat),
      .wb16_cyc_i(epwbm_cyc_o),
      .wb16_stb_i(epwbm_stb_o),
      .wb16_we_i(epwbm_we_o),
      .wb16_ack_o(epwbm_wb32m_bridgecore_ack),
      
      .wbm_adr_o(wb32m_adr_o),
      .wbm_dat_o(wb32m_dat_o),
      .wbm_dat_i(wb32m_dat_i),
      .wbm_cyc_o(wb32m_cyc_o),
      .wbm_stb_o(wb32m_stb_o),
      .wbm_we_o(wb32m_we_o),
      .wbm_ack_i(wb32m_ack_i),
      .wbm_sel_o(wb32m_sel_o)
      );

   /* At this point we have turned the multiplexed ep93xx bus cycle into a
    * WISHBONE master bus cycle with the local regs/wires:
    *
    * [15:0] epwbm_dat_i -- WISHBONE master 16-bit databus input
    * [15:0] epwbm_dat_o -- WISHBONE master 16-bit databus output
    * epwbm_clk_o -- WISHBONE master clock output (75 Mhz)
    * epwbm_rst_o -- WISHBONE master reset output
    * [23:0] epwbm_adr_o -- WISHBONE byte address output
    * epwbm_we_o -- WISHBONE master write enable output
    * epwbm_stb_o -- WISHBONE master strobe output
    * epwbm_cyc_o -- WISHBONE master cycle output
    * epwbm_ack_i -- WISHBONE master ack input
    *
    * The WISHBONE slave or WISHBONE interconnect can withhold the bus cycle ack
    * as long as necessary as the above logic will ensure the processor will be
    * halted until the cycle is complete. In that regard, it is possible
    * to lock up the processor if nothing acks the WISHBONE bus cycle. (!)
    *
    * Note that the above is only a 16-bit WISHBONE bus. A special WISHBONE
    * to WISHBONE bridge is used to combine two back-to-back 16 bit reads or
    * writes into a single atomic 32-bit WISHBONE bus cycle. Care should be
    * taken to never issue a byte or halfword ARM insn (ldrh, strh, ldrb, strb) to
    * address space handled here. This bridge is presented as a secondary
    * WISHBONE master bus prefixed with wb32m_:
    *
    * [31:0] wb32m_dat_i -- WISHBONE master 32-bit databus input
    * [31:0] wb32m_dat_o -- WISHBONE master 32-bit databus output
    * wb32m_clk_o -- WISHBONE master clock output (75 Mhz)
    * wb32m_rst_o -- WISHBONE master reset output
    * [21:0] wb32m_adr_o -- WISHBONE master word address
    * wb32m_we_o -- WISHBONE master write enable output
    * wb32m_stb_o -- WISHBONE master strobe output
    * wb32m_cyc_o -- WISHBONE master cycle output
    * wb32m_ack_i -- WISHBONE master ack input
    * wb32m_sel_o -- WISHBONE master select output -- always 4'b1111
    */
/******************************************************************
 * blue_pad, green_pad etc, etc are the physical pin I/Os.
 * headerpin_o is the data to be output.
 * headerpin_oe should be the tristate control.
 * headerpin_i should be the incoming data
 * This is a hacked version to try and get the I/O tristate
 * control to synthesise properly :(
 *******************************************************/
   wire [31:0] usercore_dat;
   wire        usercore_ack;
   reg 	       usercore_stb;
   wire [40:1]  headerpin_i;
   reg [40:1]  temp_reg;
   
   wire [40:1] headerpin_oe, headerpin_o;
   integer     i;

   // grab the current inputs
   assign headerpin_i[1]  = blue_pad[0];
   assign headerpin_i[3]  = blue_pad[1];
   assign headerpin_i[5]  = blue_pad[2];
   assign headerpin_i[7]  = blue_pad[3];
   assign headerpin_i[9]  = blue_pad[4];   
   assign headerpin_i[11] = green_pad[0];
   assign headerpin_i[13] = green_pad[1];
   assign headerpin_i[15] = green_pad[2];
   assign headerpin_i[17] = green_pad[3];
   assign headerpin_i[19] = green_pad[4];      
   assign headerpin_i[4]  = red_pad[0];
   assign headerpin_i[6]  = red_pad[1];
   assign headerpin_i[8]  = red_pad[2];
   assign headerpin_i[10] = red_pad[3];
   assign headerpin_i[12] = red_pad[4];

   assign headerpin_i[21] = dio0to8_pad[0];
   assign headerpin_i[23] = dio0to8_pad[1];
   assign headerpin_i[25] = dio0to8_pad[2];
   assign headerpin_i[27] = dio0to8_pad[3];
   assign headerpin_i[29] = dio0to8_pad[4];
   assign headerpin_i[31] = dio0to8_pad[5];
   assign headerpin_i[33] = dio0to8_pad[6];
   assign headerpin_i[35] = dio0to8_pad[7];
   assign headerpin_i[37] = dio0to8_pad[8];      

   assign headerpin_i[24] = dio10to17_pad[0];
   assign headerpin_i[26] = dio10to17_pad[1];
   assign headerpin_i[28] = dio10to17_pad[2];
   assign headerpin_i[30] = dio10to17_pad[3];
   assign headerpin_i[32] = dio10to17_pad[4];
   assign headerpin_i[34] = dio10to17_pad[5];
   assign headerpin_i[36] = dio10to17_pad[6];
   assign headerpin_i[38] = dio10to17_pad[7];   
   assign headerpin_i[39] = dio9_pad;

   assign headerpin_i[14] = hsync_pad;
   assign headerpin_i[16] = vsync_pad;
   

// misc fixed values
   assign headerpin_i[22] = 1'b0;
   assign headerpin_i[40] = 1'b1;
   assign headerpin_i[2] = 1'b0;
   assign headerpin_i[20] = 1'b1;
   assign headerpin_i[18] = 1'b0;
 
// assign outputs or tristates
   assign blue_pad[0] = temp_reg[1];
   assign blue_pad[1] = temp_reg[3];
   assign blue_pad[2] = temp_reg[5];
   assign blue_pad[3] = temp_reg[7];
   assign blue_pad[4] = temp_reg[9];
   assign green_pad[0] = temp_reg[11];
   assign green_pad[1] = temp_reg[13];
   assign green_pad[2] = temp_reg[15];
   assign green_pad[3] = temp_reg[17];
   assign green_pad[4] = temp_reg[19];   
   assign red_pad[0] = temp_reg[4];
   assign red_pad[1] = temp_reg[6];
   assign red_pad[2] = temp_reg[8];
   assign red_pad[3] = temp_reg[10];
   assign red_pad[4] = temp_reg[12];      

   assign vsync_pad = temp_reg[16];
   assign hsync_pad = temp_reg[14];
   
   
   assign dio0to8_pad[0] = temp_reg[21];
   assign dio0to8_pad[1] = temp_reg[23];
   assign dio0to8_pad[2] = temp_reg[25];
   assign dio0to8_pad[3] = temp_reg[27];
   assign dio0to8_pad[4] = temp_reg[29];
   assign dio0to8_pad[5] = temp_reg[31];
   assign dio0to8_pad[6] = temp_reg[33];
   assign dio0to8_pad[7] = temp_reg[35];
   assign dio0to8_pad[8] = temp_reg[37];
   assign dio10to17_pad[0] = temp_reg[24];
   assign dio10to17_pad[1] = temp_reg[26];   
   assign dio10to17_pad[2] = temp_reg[28];
   assign dio10to17_pad[3] = temp_reg[30];
   assign dio10to17_pad[4] = temp_reg[32];
   assign dio10to17_pad[5] = temp_reg[34];     
   assign dio10to17_pad[6] = temp_reg[36];    
   assign dio10to17_pad[7] = temp_reg[38];

   
always @( headerpin_o or headerpin_oe )
     begin
	for (i = 0; i < 5; i = i + 1)
	  begin
	     if (headerpin_oe[1 + (i * 2)])
	       temp_reg[1+(i*2)] =headerpin_o[1 + (i * 2)];
	     else
	       temp_reg[1 + (i * 2)] = 1'bz;
	     if (headerpin_oe[11 + (i * 2)])
	       temp_reg[11 + (i * 2)] = headerpin_o[11 + (i * 2)];
	     else
	       temp_reg[11 + (i * 2)] = 1'bz;
	     if (headerpin_oe[4 + (i * 2)])
	       temp_reg[4 + (i * 2)] = headerpin_o[4 + (i * 2)];
	     else
	      temp_reg[4 + (i * 2)] = 1'bz;
	  end

	for (i = 0; i < 8; i = i + 1)
	  begin
	     if (headerpin_oe[24 + (i * 2)])
	       temp_reg[24 + (i * 2)] = headerpin_o[24 + (i * 2)];
	     else
	       temp_reg[24 + (i * 2)] = 1'bz;
	     if (headerpin_oe[21 + (i * 2)])
	       temp_reg[21 + (i * 2)] = headerpin_o[21 + (i * 2)];
	     else
	       temp_reg[21 + (i * 2)] = 1'bz;	       
	  end

	if (headerpin_oe[14]) 
	  temp_reg[14] = headerpin_o[14];	  
	else 
	  temp_reg[14] = 1'bz;

	if (headerpin_oe[16]) 
	  temp_reg[16] = headerpin_o[16];	  
	else 
	  temp_reg[16] = 1'bz;

	if (headerpin_oe[37]) 
	  temp_reg[37] = headerpin_o[37];	  
	else
	  temp_reg[37] = 1'bz;
     end
   wire usercore_drq, usercore_irq;

   // SDRAM
   wire [12:0] uc_sdram_add_pad;
   wire        uc_sdram_ras_pad;
   wire        uc_sdram_cas_pad;
   wire        uc_sdram_we_pad;
   wire [1:0]  uc_sdram_ba_pad;
   wire [15:0] uc_sdram_data_pad_i;
   wire [15:0] uc_sdram_data_pad_o;
   reg 	       uc_sdram_data_pad_oe;

    //////////////////////////////////////////////////////////////////////// 
   // this is the interface to your component. It shouldn't need changing//
   ////////////////////////////////////////////////////////////////////////

   ts7300_usercore usercore
     (
      .wb_clk_i(wb32m_clk_o),
      .wb_rst_i(wb32m_rst_o),
      .wb_cyc_i(wb32m_cyc_o),
      .wb_stb_i(usercore_stb),
      .wb_we_i(wb32m_we_o),
      .wb_ack_o(usercore_ack),
      .wb_dat_o(usercore_dat),
      .wb_dat_i(wb32m_dat_o),
      .wb_adr_i(wb32m_adr_o),
      
      .headerpin_i(headerpin_i[40:1]),
      .headerpin_o(headerpin_o[40:1]),
      .headerpin_oe_o(headerpin_oe[40:1]),
      
      .irq_o(usercore_irq),
      
      .sdram_ras_o( sdram_ras_pad ),
      .sdram_cas_o( sdram_cas_pad ),
      .sdram_we_n_o( sdram_we_pad ),
      .sdram_ba_o( sdram_ba_pad ),
      .sdram_saddr_o( sdram_add_pad ),
      .sdram_sdata_i( uc_sdram_data_pad_i ),
      .sdram_sdata_o( uc_sdram_data_pad_o ),
      .sdram_sdata_oe( uc_sdram_data_pad_oe )
      );

   /* IRQ7 is actually ep9302 VIC IRQ #40 */
   assign      irq7_pad = ( usercore_irq ) ? 1'b1 : 1'bz;

   //assign sdram_add_pad = uc_sdram_add_pad;
   //assign sdram_ba_pad = uc_sdram_ba_pad;
   //assign sdram_cas_pad = uc_sdram_cas_pad;
   //assign sdram_ras_pad = uc_sdram_ras_pad;
   //assign sdram_we_pad = uc_sdram_we_pad;
   assign      sdram_clk_pad = clk_75mhz_pad & pll_locked;
   assign      sdram_data_pad = uc_sdram_data_pad_oe ? uc_sdram_data_pad_o :
				16'bz;
   assign      uc_sdram_data_pad_i = uc_sdram_data_pad_oe ?
				     uc_sdram_data_pad_i : sdram_data_pad;



   /* Now we set up the address decode and the return WISHBONE master
    * databus and ack signal multiplexors. This is very simple, on the native
    * WISHBONE bus (epwbm_*) if the address is >= 0x72100000, the 16 to 32 bit
    * bridge is selected. The 32 bit wishbone bus contains 3 wishbone
    * slaves: the ethernet core, the ethernet packet RAM, and the usercore. If the
    * address >= 0x72a00000 the usercore is strobed and expected to ack, for
    * address >= 0x72102000 the ethernet core is strobed and expected to ack
    * otherwise the bus cycle goes to the ethernet RAM core.
    */

   always @(epwbm_adr_o or epwbm_wb32m_bridgecore_dat or
	    epwbm_wb32m_bridgecore_ack or usercore_dat or usercore_ack or
	    wb32m_adr_o or wb32m_stb_o) 
     begin
	epwbm_dat_i = 16'hxxxx;
	epwbm_ack_i = 1'bx;
	if (epwbm_adr_o >= 24'h100000) 
	  begin
	     epwbm_dat_i = epwbm_wb32m_bridgecore_dat;
	     epwbm_ack_i = epwbm_wb32m_bridgecore_ack;
	  end

	usercore_stb = 1'b0;
	// ethcore_stb = 1'b0;
	// ethramcore_stb = 1'b0;
	// if (wb32m_adr_o >= 22'h280000) begin
	usercore_stb = wb32m_stb_o;
	wb32m_dat_i = usercore_dat;
	wb32m_ack_i = usercore_ack;
	// end
     end

   /* Various defaults for signals not used in this boilerplate project: */
   
   /* No use for DMA -- used by TS-SDCORE on shipped bitstream */
   assign dma_req_pad = 1'bz;

   /* PHY always on */
   assign eth_pd_pad = 1'b1;

   /* SDRAM signals outputing 0's -- used by TS-VIDCORE in shipped
    bitstream */
   /*
    assign sdram_add_pad = 12'd0;
    assign sdram_ba_pad = 2'd0;
    assign sdram_cas_pad = 1'b0;
    assign sdram_ras_pad = 1'b0;
    assign sdram_we_pad = 1'b0;
    assign sdram_clk_pad = 1'b0;
    assign sdram_data_pad = 16'd0;
    */

   /* serial (RS232) mux signals safely "parked" -- used by TS-UART */
   assign rd_mux_pad = 1'b1;
   assign mux_cntrl_pad = 1'b0;
   assign wr_232_pad = 1'b1;
   assign mux_pad = 4'hz;

   /* SD flash card signals "parked" -- used by TS-SDCORE */
   assign sd_soft_power_pad = 1'b0;
   assign sd_hard_power_pad = 1'b1;
   assign sd_dat_pad = 4'hz;
   assign sd_clk_pad = 1'b0;
   assign sd_cmd_pad = 1'bz;

endmodule

