library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity VGA is   --VGA entity
    Port (
        clk_25 : in STD_LOGIC;
        reset : in STD_LOGIC;
        RGB_IN : in STD_LOGIC_VECTOR(11 downto 0);
        RGB : out STD_LOGIC_VECTOR(11 downto 0);
        HSync, VSync : out STD_LOGIC;
        HCount : out STD_LOGIC_VECTOR(9 downto 0);
        VCount : out STD_LOGIC_VECTOR(9 downto 0)
    );
end VGA;

--This module is to show the game of the VGA.
architecture Behavioral of VGA is
    signal signal_hcount: STD_LOGIC_VECTOR(9 downto 0);     --Signal for horizontal counter
    signal signal_vcount: STD_LOGIC_VECTOR(9 downto 0);     --Signal for vertical counter
begin
    process (clk_25, reset)
    begin
        if reset = '1' then                                 --Reset position of sprite
            signal_hcount <= (others => '0');
            signal_vcount <= (others => '0');
        elsif rising_edge(clk_25) then
        
        -- Draw ROM RGB Values
        if RGB_IN = x"001" then--if this value is given it will be consired invisible
            RGB <= x"FFF";
        else
            RGB <= RGB_IN; 
        end if;
            
            if signal_hcount < 97 then --Pulse width horizontaal
                hsync <= '0';
              else
                hsync <= '1';
              end if;
        
              if signal_vcount < 3 then --Pulse width verticaal
                vsync <= '0';
              else
                vsync <= '1';
              end if;
             
              signal_hcount <= signal_hcount + 1;
             
              if signal_hcount = 800 then   --Reset hcount
                signal_vcount <= signal_vcount + 1;
                signal_hcount <= (others => '0');
              end if;
             
              if signal_vcount = 521 then          --Reset vcount  
                signal_vcount <= (others => '0');
              end if;
          end if;
          vcount <= signal_vcount;
          hcount <= signal_hcount;
      end process;
end Behavioral;