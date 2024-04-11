library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity preVGA is
  Port ( 
  clk : in std_logic;
  hcount : in std_logic_vector(9 downto 0);           
  vcount : in std_logic_vector(9 downto 0);
  RGB1 : in std_logic_vector(11 downto 0);
  RGB2 : in std_logic_vector(11 downto 0);
  RGB3 : in std_logic_vector(11 downto 0);
  RGB4 : in std_logic_vector(11 downto 0);
  RGB5 : in std_logic_vector(11 downto 0);
  RGB6 : in std_logic_vector(11 downto 0);
  RGB7 : in std_logic_vector(11 downto 0);
  main : in std_logic;
  RGB : out std_logic_vector(11 downto 0)
  );
end preVGA;

--This module is to give priority to certain sprites/screens
architecture Behavioral of preVGA is
    signal int_hcount : integer range 0 to 1024 := 0;
    signal int_vcount : integer range 0 to 512 := 0;
begin
    process (clk)
        begin
            
            int_hcount <= To_integer(unsigned(hcount)) + 1;
            int_vcount <= To_integer(unsigned(vcount)) + 1;
            
            if RGB1 /= x"001"  then--letters
                RGB <= RGB1;
            elsif RGB6 /= x"001"  then--mainmenu/switch
                RGB <= RGB6;
            elsif RGB5 /= x"001"  then--tank 2
                RGB <= RGB5;
            elsif RGB4 /= x"001" then--tank 1
                RGB <= RGB4;
            elsif RGB2 /= x"001" then--bullet
                RGB <= RGB2;
            elsif RGB3 /= x"001" then--floor
                RGB <= RGB3;
            else 
                if main = '1' then
                    RGB <= x"FFF";
                else 
                    RGB <= x"CEF";
                end if;
           end if;
    end process;
end Behavioral;
