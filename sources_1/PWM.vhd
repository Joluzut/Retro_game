library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity PWM is
    Port ( clk : in  STD_LOGIC;
           pwm_in : in  STD_LOGIC_VECTOR (12 downto 0);  -- Input PWM frequency
           pwm_out : out  STD_LOGIC);
end PWM;

--This PWM module accepts frequenties from 0 to 8kHz and sends them to an output pin.
architecture Behavioral of PWM is
    signal counter : INTEGER range 0 to 79999 := 0;  -- Clock frequency is 8 MHz
    signal pwm_signal : STD_LOGIC;
    signal frequency : INTEGER;
begin
    process (clk)
        variable pwm_period : INTEGER;
    begin
        if rising_edge(clk) then
            -- Convert pwm_in from std_logic_vector to integer
            frequency <= to_integer(unsigned(pwm_in));
            
            pwm_period := 8000000 / (frequency + 1);  -- Calculate PWM period for 8 MHz clock
            
            counter <= counter + 1;
            if counter < pwm_period / 2 then
                pwm_signal <= '1';
            else
                pwm_signal <= '0';
            end if;
            
            if counter = pwm_period - 1 then
                counter <= 0;
            end if;
        end if;
    end process;

    pwm_out <= pwm_signal;
end Behavioral;