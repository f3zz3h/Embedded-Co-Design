-------------------------------------------------------------------------------
--
-- Title : R4B user core
-- Design : r4b
-- Author : Bulent Selek
-- Company : Best Bilgisayar ve Elektronik Sanayi Ticaret
--
-------------------------------------------------------------------------------
--
-- File : ts7300_usercore.vhd
-- Generated : Fri Jan 8 13:22:36 2007
--
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Modified by Nigel Gunton
-- Company : Bristol Institute of Technology
-- version 1.0
-- full access to the DIO pins split into 3 registers plus output enable registers
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ts7300_usercore is

-- 75Mhz clock is fed to this module */
  port (
    wb_clk_i       : in  std_logic;
    wb_rst_i       : in  std_logic;
    wb_adr_i       : in  std_logic_vector ( 21 downto 0 );
    wb_dat_i       : in  std_logic_vector ( 31 downto 0 );
    wb_cyc_i       : in  std_logic;
    wb_stb_i       : in  std_logic;
    wb_we_i        : in  std_logic;
    wb_ack_o       : out std_logic;
    wb_dat_o       : out std_logic_vector ( 31 downto 0 );
    headerpin_i    : in  std_logic_vector( 40 downto 1 );
    headerpin_o    : out std_logic_vector( 40 downto 1 );
    headerpin_oe_o : out std_logic_vector( 40 downto 1 );
    irq_o          : out std_logic;
-- SDRAM
    sdram_ras_o    : out std_logic;     -- SDRAM row address strobe
    sdram_cas_o    : out std_logic;     -- SDRAM column address strobe
    sdram_we_n_o   : out std_logic;     -- SDRAM write enable
    sdram_ba_o     : out std_logic_vector( 1 downto 0 );  -- SDRAM bank address
    sdram_saddr_o  : out std_logic_vector( 11 downto 0 );  -- SDRAM row/column address
    sdram_sdata_i  : in  std_logic_vector ( 15 downto 0 );  -- data from SDRAM
    sdram_sdata_o  : out std_logic_vector ( 15 downto 0 );  -- data to SDRAM
    sdram_sdata_oe : out std_logic      -- true if data is output to SDRAM on sDOut
    );
end ts7300_usercore;

architecture ts7300_usercore of ts7300_usercore is

--- PWM Stuff ---

component pwm
  
  port (
    clk          : in  std_logic;
    duty_cycle   : in  std_logic_vector(15 downto 0);
    pwm_enable   : in  std_logic;
    resetn       : in  std_logic;
    pwm_out      : out std_logic);

end component;

  constant deadbeaf    : std_logic_vector( 31 downto 0 ) := x"deadcafe";
  signal   vga_reg     : std_logic_vector( 14 downto 0 );  -- only 14 downto 0 valid
  signal   dio2_reg    : std_logic_vector( 15 downto 0 );  -- only 15 downto 0 valid
  signal   dummyreg    : std_logic_vector( 31 downto 0 );  -- temporary reg
  signal   misc_reg    : std_logic_vector( 3 downto 0 );  -- only 3 downto 0 valid
  signal   vga_enable  : std_logic_vector( 14 downto 0 );  -- 1 = output enabled
  signal   dio2_enable : std_logic_vector( 15 downto 0 );  -- ditto
  signal   misc_enable : std_logic_vector( 3 downto 0 );  -- ditto
  signal   vga_read    : std_logic_vector(14 downto 0);  -- read from I/O pins
  signal   dio2_read   : std_logic_vector(15 downto 0);  -- ditto
  signal   misc_read   : std_logic_vector(3 downto 0);  -- ditto
  signal  DUTY_CYCLEA : std_logic_vector(15 downto 0) := X"006F";
  signal  DUTY_CYCLEB : std_logic_vector(15 downto 0) := X"005F";
  signal  DUTY_CYCLEC : std_logic_vector(15 downto 0) := X"007F";
  signal  DUTY_CYCLED : std_logic_vector(15 downto 0) := X"008F";
  signal PWM_not_reset, PWM_enable_A, PWM_enable_B, PWM_enable_C, PWM_enable_D : std_logic := '1';

  
  --- Read and write cycles of FPGA by and to the Processor
begin
  process ( wb_clk_i )
    variable addr : std_logic_vector ( 23 downto 0 );
  begin
    if ( wb_clk_i'event ) and ( wb_clk_i = '1' ) then
      addr := "00" & wb_adr_i;

      if (wb_cyc_i = '1' and wb_stb_i = '1' ) and ( wb_we_i = '1') then
-- WRITE CYCLE
        case addr is
          when x"280000" => vga_reg     <= wb_dat_i(14 downto 0);
          when x"280001" => vga_enable  <= wb_dat_i(14 downto 0);
          when x"280002" => dio2_reg    <= wb_dat_i(15 downto 0);
          when x"280003" => dio2_enable <= wb_dat_i(15 downto 0);
          when x"280004" => misc_reg    <= wb_dat_i(3 downto 0);
          when x"280005" => misc_enable <= wb_dat_i(3 downto 0);
			 when x"280006" => DUTY_CYCLEA <= wb_dat_i(15 downto 0);
			 when x"280007" => DUTY_CYCLEB <= wb_dat_i(15 downto 0);
			 when x"280008" => DUTY_CYCLEC <= wb_dat_i(15 downto 0);
			 when x"280009" => DUTY_CYCLED <= wb_dat_i(15 downto 0);
			 when x"28000A" => PWM_enable_A <= wb_dat_i(0);
			 when x"28000B" => PWM_enable_B <= wb_dat_i(0);
			 when x"28000C" => PWM_enable_C <= wb_dat_i(0);
			 when x"28000D" => PWM_enable_D <= wb_dat_i(0);
			 when x"28000E" => PWM_not_reset <= wb_dat_i(0);
          when others    => null;
        end case;
      end if;
-- READ CYCLE
      case addr is
        when x"280000"   => dummyreg    <= "00000000000000000" & vga_read;
        when x"280001"   => dummyreg    <= "00000000000000000" & vga_enable;
        when x"280002"   => dummyreg    <= "0000000000000000" & dio2_read;
        when x"280003"   => dummyreg    <= "0000000000000000" & dio2_enable;
        when x"280004"   => dummyreg    <= "0000000000000000000000000000" & misc_read;
		  when x"280005"   => dummyreg    <= "0000000000000000000000000000" & misc_enable;
		  when x"280006" => dummyreg <= "0000000000000000" & DUTY_CYCLEA;
		  when x"280007" => dummyreg <= "0000000000000000" & DUTY_CYCLEB;
		  when x"280008" => dummyreg <= "0000000000000000" & DUTY_CYCLEC;
		  when x"280009" => dummyreg <= "0000000000000000" & DUTY_CYCLED;
		  when x"28000A" => dummyreg <= "0000000000000000000000000000000" & PWM_enable_A;
		  when x"28000B" => dummyreg <= "0000000000000000000000000000000" & PWM_enable_B;
		  when x"28000C" => dummyreg <= "0000000000000000000000000000000" & PWM_enable_C;
		  when x"28000D" => dummyreg <= "0000000000000000000000000000000" & PWM_enable_D;
		  when x"28000E" => dummyreg <= "0000000000000000000000000000000" & PWM_not_reset ;
		  when others      => dummyreg    <= x"deadbeef";
      end case;
    end if;
  end process;


  wb_ack_o <= wb_cyc_i and wb_stb_i;    -- 0-wait state WISHBONE
  wb_dat_o <= dummyreg;
  irq_o    <= '0';

-------------------------------------------------------------------------------
-- FPGA DIO output enable assignments: 40 downto 1
-- What follows is a pigs ear of assignments as the output enables are in pin
-- number order and the enable registers are in data name order
-------------------------------------------------------------------------------
  headerpin_oe_o(1)  <= vga_enable(0);   -- blu0
  headerpin_oe_o(3)  <= vga_enable(1);   -- blu1
  headerpin_oe_o(5)  <= vga_enable(2);   -- blu2
  headerpin_oe_o(7)  <= vga_enable(3);   -- blu3
  headerpin_oe_o(9)  <= vga_enable(4);   -- blu4
  headerpin_oe_o(11) <= vga_enable(5);   -- grn0
  headerpin_oe_o(13) <= vga_enable(6);   -- grn1
  headerpin_oe_o(15) <= vga_enable(7);   -- grn2
  headerpin_oe_o(17) <= vga_enable(8);   -- grn3
  --headerpin_oe_o(19) <= vga_enable(9);   -- grn4
  headerpin_oe_o(19) <= '1';
  
  headerpin_oe_o(4)  <= vga_enable(10);  -- red0
  headerpin_oe_o(6)  <= vga_enable(11);  -- red1
  headerpin_oe_o(8)  <= vga_enable(12);  -- red2
  headerpin_oe_o(10) <= vga_enable(13);  -- red3
  headerpin_oe_o(12) <= vga_enable(14);  -- red4


	headerpin_oe_o(21) <= '1';
	headerpin_oe_o(23) <= '1';
	headerpin_oe_o(25) <= '1';
  --headerpin_oe_o(21) <= dio2_enable(0);     -- DIO2
  --headerpin_oe_o(23) <= dio2_enable(1);     -- DIO2
  --headerpin_oe_o(25) <= dio2_enable(2);     -- DIO2
  headerpin_oe_o(27) <= dio2_enable(3);     -- DIO2
  headerpin_oe_o(29) <= dio2_enable(4);     -- DIO2
  headerpin_oe_o(31) <= dio2_enable(5);     -- DIO2
  headerpin_oe_o(33) <= dio2_enable(6);     -- DIO2
  headerpin_oe_o(35) <= dio2_enable(7);     -- DIO2
  headerpin_oe_o(37) <= dio2_enable(8);     -- DIO2
  -- THE FOLLOWING PIN IS DEDICATED CLK INPUT ONLY
  -- headerpin_oe_o(39) <= dio2_enable(9);  -- DIO2
  headerpin_oe_o(24) <= dio2_enable(10);    -- DIO2
  headerpin_oe_o(26) <= dio2_enable(11);    -- DIO2
  headerpin_oe_o(28) <= dio2_enable(12);    -- DIO2
  headerpin_oe_o(30) <= dio2_enable(13);    -- DIO2
  headerpin_oe_o(32) <= dio2_enable(14);    -- DIO2
  headerpin_oe_o(34) <= dio2_enable(15);    -- DIO2



  headerpin_oe_o(14) <= misc_enable(0);  -- HSYNC
  headerpin_oe_o(16) <= misc_enable(1);  -- VSYNC
  headerpin_oe_o(36) <= misc_enable(2);  -- RX_LD
  headerpin_oe_o(38) <= misc_enable(3);  -- TX_LD

-- the following MUST NOT be altered as they have dedicated use

  headerpin_oe_o(2)  <= '0';            -- GND
  headerpin_oe_o(18) <= '0';            -- OEV
  headerpin_oe_o(20) <= '0';            -- +5v fused
  headerpin_oe_o(22) <= '0';            -- GND
  headerpin_oe_o(39) <= '0';            -- external clk input only
  headerpin_oe_o(40) <= '0';            -- +3.3v

-------------------------------------------------------------------------------
-- output assignments
-------------------------------------------------------------------------------
 
 headerpin_o(1) <= vga_reg(0);     --blu0
 headerpin_o(3) <= vga_reg(1);     --blu1
 headerpin_o(5) <= vga_reg(2);     --blu2
 headerpin_o(6) <= vga_reg(3);     --blu3
 headerpin_o(7) <= vga_reg(4);     --blu4
 headerpin_o(11) <= vga_reg(5);     --grn0
 headerpin_o(13) <= vga_reg(6);     --grn1
 headerpin_o(15) <= vga_reg(7);     --grn2
 headerpin_o(17) <= vga_reg(8);     --grn3
  
  --For PWM now
  --headerpin_o(19) <= dio2_reg(0);       -- DIO2
  --headerpin_o(21) <= dio2_reg(0);       -- DIO2
  --headerpin_o(23) <= dio2_reg(1);       -- DIO2
  --headerpin_o(25) <= dio2_reg(2);       -- DIO2
  
  headerpin_o(27) <= dio2_reg(3);       -- DIO2
  headerpin_o(29) <= dio2_reg(4);       -- DIO2
  headerpin_o(31) <= dio2_reg(5);       -- DIO2
  headerpin_o(33) <= dio2_reg(6);       -- DIO2
  headerpin_o(35) <= dio2_reg(7);       -- DIO2
  headerpin_o(37) <= dio2_reg(8);       -- DIO2
  
  -- THE FOLLOWING PIN IS INPUT ONLY
  -- headerpin_o(39) <= dio2_reg(9);    -- DIO2
  headerpin_o(24) <= dio2_reg(10);      -- DIO2
  headerpin_o(26) <= dio2_reg(11);      -- DIO2
  headerpin_o(28) <= dio2_reg(12);      -- DIO2
  headerpin_o(30) <= dio2_reg(13);      -- DIO2
  headerpin_o(32) <= dio2_reg(14);      -- DIO2
  headerpin_o(34) <= dio2_reg(15);      -- DIO2

  headerpin_o(14) <= misc_reg(0);       -- HSYNC
  headerpin_o(16) <= misc_reg(1);       -- VSYNC
  headerpin_o(36) <= misc_reg(2);       -- RX_LD
  headerpin_o(38) <= misc_reg(3);       -- TX_LD

  -- just to keep synthesis warnings down ;)
  headerpin_o(2)  <= '0';               -- GND
  headerpin_o(18) <= '0';               -- OEV
  headerpin_o(20) <= '0';               -- +5v fused
  headerpin_o(22) <= '0';               -- GND
  headerpin_o(40) <= '0';               -- +3.3v

   -----------------------------------------------------------------------------
  -- read values on the IO pins; the special pins are handled in ts7300_top.v
  -----------------------------------------------------------------------------
  vga_read(0)  <= headerpin_i(1);       -- blu0    
  vga_read(1)  <= headerpin_i(3);       -- blu1
  vga_read(2)  <= headerpin_i(5);       -- blu2
  vga_read(3)  <= headerpin_i(7);       -- blu3
  vga_read(4)  <= headerpin_i(9);       -- blu4
  vga_read(5)  <= headerpin_i(11);      -- grn0
  vga_read(6)  <= headerpin_i(13);      -- grn1
  vga_read(7)  <= headerpin_i(15);      -- grn2
  vga_read(8)  <= headerpin_i(17);      -- grn3
  
  dio2_read(0)  <= headerpin_i(21);     -- DIO2
  dio2_read(1)  <= headerpin_i(23);     -- DIO2
  dio2_read(2)  <= headerpin_i(25);     -- DIO2
  dio2_read(3)  <= headerpin_i(27);     -- DIO2
  dio2_read(4)  <= headerpin_i(29);     -- DIO2
  dio2_read(5)  <= headerpin_i(31);     -- DIO2
  dio2_read(6)  <= headerpin_i(33);     -- DIO2
  dio2_read(7)  <= headerpin_i(35);     -- DIO2
  dio2_read(8)  <= headerpin_i(37);     -- DIO2
  
-- dio2_read(9) deliberately left out; see above
  dio2_read(10) <= headerpin_i(24);     -- DIO2
  dio2_read(11) <= headerpin_i(26);     -- DIO2
  dio2_read(12) <= headerpin_i(28);     -- DIO2
  dio2_read(13) <= headerpin_i(30);     -- DIO2
  dio2_read(14) <= headerpin_i(32);     -- DIO2
  dio2_read(15) <= headerpin_i(34);     -- DIO2

  misc_read(0) <= headerpin_i(14);      -- HSYNC
  misc_read(1) <= headerpin_i(16);      -- VSYNC
  misc_read(2) <= headerpin_i(36);      -- RX_LD but not a lot of point
  misc_read(3) <= headerpin_i(38);      -- TX_LD



 PWM_1:pwm
   port map (
  clk => wb_clk_i,
  duty_cycle => DUTY_CYCLEA,
  pwm_enable =>  PWM_enable_A,--Enable_output_A,
  resetn => PWM_not_reset,
  pwm_out => headerpin_o(19));
  
  
 PWM_2:pwm
   port map (
  clk => wb_clk_i,
  duty_cycle => DUTY_CYCLEB,
  pwm_enable =>  PWM_enable_B,--Enable_output_A,
    resetn => PWM_not_reset,
  pwm_out => headerpin_o(21));
  
  
 PWM_3:pwm
   port map (
  clk => wb_clk_i,
  duty_cycle => DUTY_CYCLEB,
  pwm_enable =>  PWM_enable_C,--Enable_output_A,
    resetn => PWM_not_reset,
  pwm_out => headerpin_o(23));
  
  
 PWM_4:pwm
   port map (
  clk => wb_clk_i,
  duty_cycle => DUTY_CYCLEB,
  pwm_enable =>  PWM_enable_D,--Enable_output_A,
    resetn => PWM_not_reset,
  pwm_out => headerpin_o(25));





end ts7300_usercore;
