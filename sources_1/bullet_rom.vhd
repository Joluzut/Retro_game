library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;


entity bullet_rom is                                               --ROM entity declaration
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        sprite_x : in std_logic_vector(9 downto 0);
        sprite_y : in std_logic_vector(9 downto 0);
        RGB : out std_logic_vector(11 downto 0)
    );
end bullet_rom;

--This module shows the bullet.
architecture Behavioral of bullet_rom is
    signal signal_hcount: STD_LOGIC_VECTOR(9 downto 0);     --Signal for horizontal counter
    signal signal_vcount: STD_LOGIC_VECTOR(9 downto 0);     --Signal for vertical counter
    signal sprite_index : integer range 0 to 24 := 0;           --Signal for sprite index

    type romtype is array (0 to 24) of std_logic_vector(11 downto 0);

-- Here you can change the sprite of the ball:
    constant ball : romtype := (
x"001", x"000", x"000", x"000", x"001",
x"000", x"000", x"000", x"000", x"000",
x"000", x"000", x"000", x"000", x"000",
x"000", x"000", x"000", x"000", x"000",
x"001", x"000", x"000", x"000", x"001"
);

begin

    process(clk)
    begin
        if(rising_edge(clk)) then
            signal_hcount <= hcount; 
            signal_vcount <= vcount;

            if signal_hcount <= 144 and signal_vcount <= 31 then
                sprite_index <= 0;
            end if;
            
            if signal_hcount >= 144 and signal_hcount < 784 and signal_vcount >= 31 and signal_vcount < 511 then -- 640x480 scherm
                if signal_hcount >= sprite_x + 144 and signal_hcount < sprite_x + 149 and signal_vcount >= sprite_y + 31 and signal_vcount < sprite_y + 36 then -- 5x5 sprite
                    RGB <= ball(sprite_index);
                    
                        if sprite_index = 24 then   --reset sprite index at max value
                            sprite_index <= 0;
                        else
                            sprite_index <= sprite_index + 1;   --Increase sprite index
                        end if;
                else 
                    RGB <= "000000000001";
                end if;
            else
                RGB <= (others => '0'); -- zwart
            end if;
        end if;
    end process;

    
end Behavioral;