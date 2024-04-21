library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;


entity tank_rom is                                               --ROM entity declaration
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        sprite_x : in std_logic_vector(9 downto 0);
        sprite_y : in std_logic_vector(9 downto 0);
        color : in std_logic;
        barrelloc : in std_logic_vector(3 downto 0);
        RGB : out std_logic_vector(11 downto 0)
    );
end tank_rom;

--this module is to show the tank and it different barrel states.
architecture Behavioral of tank_rom is
    signal signal_hcount: STD_LOGIC_VECTOR(9 downto 0);     --Signal for horizontal counter
    signal signal_vcount: STD_LOGIC_VECTOR(9 downto 0);     --Signal for vertical counter
    signal sprite_index, sprite : integer range 0 to 255 := 0;           --Signal for sprite index
    signal pos: integer range 0 to 11 := 0;
    signal RGB_signal : std_logic_vector(11 downto 0);
    type romtype is array (0 to 255) of std_logic_vector(11 downto 0);
    
--Here you can change the sprite of the tank:
--x"111" represents one state of the barrel and will only be visible when the tank is in that state.
    constant barrel : romtype := (    
x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", 
x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", 
x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001", x"001",                            
x"001", x"001", x"001", x"001", x"001", x"001", x"777", x"001", x"DDD", x"666", x"001", x"001", x"001", x"001", x"001", x"001",
x"001", x"001", x"001", x"001", x"888", x"001", x"777", x"001", x"DDD", x"666", x"001", x"555", x"001", x"001", x"001", x"001",
x"001", x"001", x"001", x"001", x"888", x"001", x"777", x"777", x"6D6", x"666", x"001", x"555", x"001", x"001", x"001", x"001",
x"001", x"001", x"999", x"001", x"888", x"888", x"001", x"777", x"6D6", x"001", x"555", x"555", x"001", x"444", x"001", x"001",
x"001", x"001", x"999", x"999", x"001", x"888", x"001", x"777", x"6D6", x"001", x"555", x"001", x"444", x"444", x"001", x"001",
x"AAA", x"AAA", x"001", x"999", x"999", x"888", x"888", x"ABA", x"ABA", x"555", x"555", x"444", x"444", x"001", x"333", x"333",
x"001", x"AAA", x"AAA", x"AAA", x"999", x"999", x"ABA", x"ABA", x"ABA", x"ABA", x"444", x"444", x"333", x"333", x"333", x"001",
x"001", x"001", x"001", x"AAA", x"AAA", x"ABA", x"ABA", x"ABA", x"ABA", x"ABA", x"ABA", x"333", x"333", x"001", x"001", x"001",
x"BBB", x"BBB", x"BBB", x"BBB", x"BBB", x"ABA", x"ABA", x"ABA", x"ABA", x"ABA", x"ABA", x"222", x"222", x"222", x"222", x"222",
x"001", x"001", x"001", x"001", x"001", x"ABA", x"ABA", x"ABA", x"ABA", x"ABA", x"ABA", x"001", x"001", x"001", x"001", x"001",
x"001", x"001", x"001", x"001", x"000", x"000", x"000", x"000", x"000", x"000", x"000", x"000", x"001", x"001", x"001", x"001",
x"001", x"001", x"001", x"000", x"787", x"000", x"787", x"000", x"787", x"000", x"787", x"000", x"000", x"001", x"001", x"001",
x"001", x"001", x"001", x"001", x"000", x"000", x"000", x"000", x"000", x"000", x"000", x"000", x"001", x"001", x"001", x"001"

);

begin

    process(clk)
    begin
        if(rising_edge(clk)) then
            signal_hcount <= hcount + 1;  
            signal_vcount <= vcount + 1;
            
            -- For the different colors each tank has.
            if color = '1' then
                RGB_signal <= x"2AF";
            else
                RGB_signal <= x"F32";
            end if;
             
            pos <= To_integer(unsigned(barrelloc));
                       
            if signal_hcount <= 144 and signal_vcount <= 31 then
                sprite_index <= 0;
            end if;
            
            if signal_hcount >= 144 and signal_hcount < 784 and signal_vcount >= 31 and signal_vcount < 511 then -- 640x480 scherm
                if signal_hcount >= sprite_x + 144 and signal_hcount < sprite_x + 160 and signal_vcount >= sprite_y + 31 and signal_vcount < sprite_y + 47 then -- 16x16 sprite   
               
                case pos is 
                --To filter the correct barrel placement this case statement is made
                --this way only the correct values are being shown.
                    when 0 =>
                        -- x"222" and x"ABA represents the body of the tank."
                        if barrel(sprite_index) = x"222" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;  
                        -- x"787 represents the middle of the track and x"000" represents the track itself.                         
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 1 =>
                        if barrel(sprite_index) = x"333" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 2 =>
                        if barrel(sprite_index) = x"444" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 3 =>
                        if barrel(sprite_index) = x"555" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 4 =>
                        if barrel(sprite_index) = x"666" or barrel(sprite_index) = x"6D6" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 5 =>
                        if barrel(sprite_index) = x"DDD" or barrel(sprite_index) = x"6D6" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 6 =>
                        if barrel(sprite_index) = x"777" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 7 =>
                        if barrel(sprite_index) = x"888" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 8 =>
                        if barrel(sprite_index) = x"999" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 9 =>
                        if barrel(sprite_index) = x"AAA" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when 10 =>
                        if barrel(sprite_index) = x"BBB" or barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    when others =>
                        if barrel(sprite_index) = x"ABA" then
                            RGB <= RGB_signal;                            
                        elsif barrel(sprite_index) = x"787" or barrel(sprite_index) = x"000" then
                            RGB <= barrel(sprite_index);
                        else 
                            RGB <= x"001";
                        end if;
                    end case; 
                    
                    if sprite_index = 255 then   --reset sprite index at max value
                            sprite_index <= 0;
                        else
                            sprite_index <= sprite_index + 1;   --Increase sprite index
                        end if;
                    else
                            RGB <= x"001";
                    end if;
            else
                RGB <= (others => '0'); -- black
            end if;
        end if;
    end process;

    
end Behavioral;