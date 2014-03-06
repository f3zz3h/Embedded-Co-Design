-------------------------------------------------------------------------------
-- Title      : PWM for robot servos
-- Project    : 
-------------------------------------------------------------------------------
-- File       : my_pwm.vhd
-- Author     : <ngunton@ptolome.cems.uwe.ac.uk>
-- Company    : FoCEMS, UWE
-- Last update: 2011-03-28
-- Platform   : Altera Quartus with std_arith library and DE0 board
-------------------------------------------------------------------------------
-- Description: A very simple example of a pwm hardware implementation. It has
-- three processes, one to divide the 50Mhz clock down to ~1khz, an eight bit
-- counter for the full cycle (at 1khz) and a loadable duty cycle register (8
-- bit). Button 0 is the reset, press to start. switches 7 to 0 are the load
-- value for the duty cycle register. Switch 9 is the pwm output enable. The
-- PWM output is connected to LED0 which will range from always on (100% duty)
-- to always off (0% duty). Enjoy.
-- 
-------------------------------------------------------------------------------
-- Revisions  : 0
-- Date        Version  Author  Description
-- 2007/02/07  1.0      ngunton	Created
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_Arith.all;

entity pwm is
  
  port (
    clk          : in  std_logic;
    duty_cycle   : in  std_logic_vector(15 downto 0);
    pwm_enable   : in  std_logic;
    resetn       : in  std_logic;
    pwm_out      : out std_logic);

end pwm;


architecture behavioural of pwm is
signal clock_div : unsigned(15 downto 0);
signal counter : unsigned(7 downto 0);
signal pwm_out_sig, slow_clk : std_logic;
signal duty_cycle_reg : unsigned(7 downto 0);

begin  -- behavioural

-- purpose: control the pwm_out 
-- type   : sequential
-- inputs : slow_clk, resetn, pwm_enable
-- outputs: pwm_out_sig

  pulse: process (slow_clk, resetn, pwm_enable)
begin  -- process pulse
  if resetn = '0' then                  -- asynchronous reset (active low)
    pwm_out_sig <= '0';    
	 duty_cycle_reg <= conv_unsigned(X"6F",8);  
	 slow_clk <= '0'; -- Resets the clock
  else  
		if clk'event and clk = '1' then
			duty_cycle_reg <= unsigned(duty_cycle (7 downto 0));
			
			if clock_div = conv_unsigned(X"0B00",16) then --clock speed
				clock_div <= conv_unsigned(X"0000",16);
				slow_clk <= not slow_clk; -- toggle
			else
				clock_div <= clock_div + 1;
			end if;
		end if;
	
    if slow_clk'event and slow_clk = '1' then    -- rising clock edge
      counter <= counter +1;  
      if pwm_enable = '1' and (counter < conv_unsigned(duty_cycle_reg,8)) then
        pwm_out_sig <= '1';
      else
        pwm_out_sig <= '0';
      end if;
    end if;
  end if;
end process pulse;

pwm_out <= pwm_out_sig;

      

end behavioural;