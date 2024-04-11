library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;


entity floor_rom is                                               --ROM entity declaration
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        ground_onoff : in std_logic;
        grounddata_x : in std_logic_vector(6 downto 0):= (others => '0');
        grounddata_y : in std_logic_vector(4 downto 0):= (others => '0');
        ground_enable : in std_logic;
        enable : in std_logic
    );
end floor_rom;

--This module displays all the different floor blocks.
architecture Behavioral of floor_rom is
    signal int_hcount : integer range 0 to 1024 := 0;
    signal int_vcount : integer range 0 to 512 := 0;
    signal sprite_index : integer range 0 to 24 := 0;           --Signal for sprite index
    type romtype is array (0 to 63) of std_logic_vector(11 downto 0);
    type filled is array (0 to 80, 0 to 30) of std_logic;
    signal background : filled;
    signal toggle : std_logic := '0';

--Here you can change the floor sprite.
constant floor : romtype := (                    
x"050", x"050", x"050", x"050", x"050", x"050", x"050", x"050",
x"050", x"090", x"090", x"090", x"090", x"090", x"090", x"050",
x"050", x"090", x"090", x"090", x"090", x"090", x"090", x"050",
x"050", x"090", x"090", x"090", x"090", x"090", x"090", x"050",
x"050", x"090", x"090", x"090", x"090", x"090", x"090", x"050",
x"050", x"090", x"090", x"090", x"090", x"090", x"090", x"050",
x"050", x"090", x"090", x"090", x"090", x"090", x"090", x"050",
x"050", x"050", x"050", x"050", x"050", x"050", x"050", x"050"
);

begin    
    process(clk)
        variable i, j : integer;
    begin
        if(rising_edge(clk)) then
        
            if toggle = '0' then
            --fill the array with ones, that way when initialised it will show a full floor.
                for i in background'range(1) loop
                    for j in background'range(2) loop
                        if j /= 0 and j /= 1 then
                            background(i,j) <= '1';
                        else 
                            background(i,j) <= '0';
                        end if;
                    end loop;    
                end loop;  
                toggle <= '1';
            end if;
            
            int_hcount <= To_integer(unsigned(hcount)) + 1;
            int_vcount <= To_integer(unsigned(vcount)) + 1;
            
            --to turn on or off a specific floortile.
            if enable = '0' then
                background(to_integer(unsigned(grounddata_x)),to_integer(unsigned(grounddata_y))) <= ground_onoff ;
            end if;
            
            if int_hcount < 144 and int_vcount < 31 then
                --prevents screen tearing
                sprite_index <= 0;
            end if;
            
            if int_hcount >= 144 and int_hcount < 784 and int_vcount >= 272 and int_vcount < 511 then -- 640x480 scherm
                --the sprites are 8x8 pixels so to make sure the correct tile is being shown or not
                --the hcount is divided by 8 and that position corresponds with a place in the dubble array
                if background((int_hcount-144)/8, (int_vcount-272)/8) = '1' then
                    --to make sure the correct index is show because we show the same sprite multiple times per row
                    --this case statement is made the sprite_index resets when it hits 7, and the + x is 
                    --made to correct the displacement
                    case int_vcount mod 8 is
                        when 0 =>
                            RGB <= floor(sprite_index);
                        when 1 =>
                            RGB <= floor(sprite_index + 8);
                        when 2 =>
                            RGB <= floor(sprite_index + 16);
                        when 3 =>
                            RGB <= floor(sprite_index + 24);
                        when 4 =>
                            RGB <= floor(sprite_index + 32);
                        when 5 =>
                            RGB <= floor(sprite_index + 40);    
                        when 6 =>
                            RGB <= floor(sprite_index + 48);
                        when 7 =>
                            RGB <= floor(sprite_index + 56);
                        when others =>
                            RGB <= "000000000001";
                        end case;  
                    
                    --reset sprite_index
                    if sprite_index = 7 then
                        sprite_index <= 0;
                    else
                        sprite_index <= sprite_index + 1;            
                    end if;
                else 
                    RGB <= "000000000001";
                end if;
            else
                RGB <= "000000000001"; -- zwart
            end if;
        end if;
    end process;

    
end Behavioral;