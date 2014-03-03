 -------------------------------------------------------------------------------
-- Title      : PWM for robot servos
-- Project    : VHDL and C Robot Arm Co Design
-------------------------------------------------------------------------------
-- File       : PWM 1,40.vhd
-- Author     : Peter Smith
-- Company    : University of the West of England
-- Last update: 2014-02-26
-- Platform   : Altera Quartus with std_arith library
--            : TS7300
-------------------------------------------------------------------------------
-- Description: PWM output to dive the Robot arm EMU ROBOT HS4.
--
-------------------------------------------------------------------------------
-- Revisions   : 0
-- Date        : 22/07/2013
-- Version     : 1.40
-- Author      : PSmith
-- Description : Initial Issue
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_Arith.all;

entity my_pwm is

  port (
    clk           : in  std_logic;
    duty_cycle   : in  std_logic_vector(15 downto 0);
    pwm_enable    : in  std_logic;
    resetn        : in  std_logic;
    pwm_out      : out std_logic);

end my_pwm;


architecture behavioural of my_pwm is
signal clock_div : unsigned(15 downto 0);
signal counter : unsigned(7 downto 0);
signal pwm_out_sig : std_logic;
signal duty_cycle_reg : unsigned(7 downto 0);
signal slow_clk: std_logic;

begin  -- behavioural


  -- purpose: manage the duty cycle register
  -- type   : sequential
  -- inputs : clk, resetn, duty_cycle
  -- outputs: duty_cycle_reg

 -- control: process (clk, resetn, duty_cycle)
 -- begin  -- process control
 --   if resetn = '0' then                -- asynchronous reset (active low)
 --     duty_cycle_reg <= conv_unsigned(X"F0",8);  -- reset duty cycle value
 --   else
 --     if clk = '1' then  -- rising clock edge    -- clk'event and
 --       duty_cycle_reg <= unsigned(duty_cycle (7 downto 0));
 --     end if;            -- end of rising edge loop
 --   end if;            -- end of reset loop
 -- end process control;


  -- purpose: divide system clock down from 50Mhz to 1khz
  --                         Need to check clock speed !!!!  75Mhz
  --  75000 / 2 = 37500 = 927C
  -- type   : sequential
  -- inputs : clk, resetn
  -- outputs: slow_clk

 -- clockdiv: process (clk, resetn)
 -- begin  -- process clockdiv
 --   if resetn = '0' then                -- asynchronous reset (active low)
 --     slow_clk <= '0';            -- set slow clock to 0, reset value
 --     clock_div <= X"0000";        -- set clock div to 0, reset value
 --   elsif clk'event and clk = '1' then  -- rising clock edge
 --     if clock_div = conv_unsigned(X"927C",16) then --61A8 for 50M
 --       clock_div <= conv_unsigned(X"0000",16);    -- reset count value
 --       slow_clk <= not slow_clk;        -- toggle slow clock
 --     else
 --       clock_div <= clock_div +1;    -- increment clock_div count
 --     end if;                -- end of rising edge loop
 --   end if;                -- end of reset loop
 -- end process clockdiv;




-- purpose: control the  PWM output
-- type   : sequential
-- inputs : slow_clk, resetn, pwm_enable
-- outputs: pwm_out_sigA

  pulse: process (slow_clk, resetn, pwm_enable)
begin  -- process pulse
 -- pwm_enable = '1';
  if resetn = '0' then                  -- asynchronous reset (active low)
    pwm_out_sig <= '0';            -- output low
    duty_cycle_reg <= conv_unsigned(X"6F",8);  -- reset duty cycle value
    --                                          range of 63 to 159
    --                                                   3F to  9F
     slow_clk <= '0';            -- set slow clock to 0, reset value
  else
    if clk'event and clk = '1' then  -- rising clock edge
       duty_cycle_reg <= unsigned(duty_cycle (7 downto 0));

       if clock_div = conv_unsigned(X"0176",16) then -- speed of clock
        clock_div <= conv_unsigned(X"0000",16);    -- reset count value
        slow_clk <= not slow_clk;        -- toggle slow clock
      else
        clock_div <= clock_div +1;    -- increment clock_div count
      end if;                -- end of rising edge loop
    end if;

    if slow_clk'event and slow_clk = '1' then    -- rising slow clock edge
      counter <= counter +1;           -- increment counter
      if( pwm_enable = '1' and counter < conv_unsigned(duty_cycle_reg,8)) then    --  duty_cycle_reg,8
                                                            --  pwm_enable and
        pwm_out_sig <= '1';        -- output high, pulse width
      else
        pwm_out_sig <= '0';        -- output low, end pulse and hold low
      end if;                -- end of pwm_enable loop
    end if;                -- end of rising edge loop
    --pwm_out_sig <= not pwm_out_sig;
  end if;                -- end of reset loop
end process pulse;

  pwm_out <= not pwm_out_sig;        -- output the signal on pin pwm_outA





end behavioural;