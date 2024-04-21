library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;


entity screen_rom is                                               --ROM entity declaration
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        switch : in std_logic_vector(15 downto 0);
        main : out std_logic
    );
end screen_rom;

--This module shows the switches between mainmenu and gamescreen and back and the mainmenu itself.
architecture Behavioral of screen_rom is
    signal int_hcount : integer range 0 to 1024 := 0;
    signal int_vcount : integer range 0 to 512 := 0;
    signal flag_mainmenu, flag_switch, flag_switch2 : std_logic;  
    signal counter : integer range 0 to 500001 := 10;  
    signal whiteline : integer range 0 to 700 := 0;
    signal counter1 : integer range 0 to 10000001 := 10;  
    signal blok : integer range 0 to 10 := 0;
    signal flag : std_logic := '0';
    
begin

    process(clk)
begin
    if rising_edge(clk) then
        int_hcount <= To_integer(unsigned(hcount)) + 2;
        int_vcount <= To_integer(unsigned(vcount));

        --data recieved from spi_decoder translated.
        if switch(15 downto 8) = "10110110" then
            flag_switch <= '1';
        end if;
        if switch(7 downto 0) = "01001001" then
            flag_switch2 <= '1';
        end if; 
        
        if int_hcount >= 144 and int_hcount < 784 and int_vcount >= 31 and int_vcount < 511 then -- 640x480 screen            
            --main menu colors and when they need to be activated
            if flag_mainmenu = '1' then
                if int_vcount >= 231 and int_vcount < 311 and blok > 6 then
                    RGB <= x"FFF";
                elsif int_vcount >= 311 and int_vcount < 351 and blok > 5 then
                    RGB <= x"BDB";
                elsif int_vcount >= 351 and int_vcount < 391 and blok > 4 then
                    RGB <= x"8B8";
                elsif int_vcount >= 391 and int_vcount < 431 and blok > 3 then
                    RGB <= x"4A4";
                elsif int_vcount >= 431 and int_vcount < 471 and blok > 2 then
                    RGB <= x"292";
                elsif int_vcount >= 471 and int_vcount < 511 and blok > 1 then
                    RGB <= x"090";
                elsif int_vcount >= 191 and int_vcount < 231 and blok > 5 then
                    RGB <= x"BDB";
                elsif int_vcount >= 151 and int_vcount < 191 and blok > 4 then
                    RGB <= x"8B8";
                elsif int_vcount >= 111 and int_vcount < 151 and blok > 3 then
                    RGB <= x"4A4";
                elsif int_vcount >= 71 and int_vcount < 111 and blok > 2 then
                    RGB <= x"292";    
                elsif int_vcount >= 31 and int_vcount < 71 and blok > 1 then
                    RGB <= x"090";
                else
                    RGB <= x"FFF";
                end if;            
            else
                RGB <= x"001";
            end if;
            
            --mainmenu -> gamescreen
            if flag_switch = '1' then
            flag_mainmenu <= '1';
                if counter > 0 then
                    counter <= counter - 1;
                else
                    if whiteline < 280 then
                        whiteline <= whiteline + 1;
                    else
                        whiteline <= 0; -- Reset whiteline after it reaches 600
                        flag_switch <= '0'; --prevents the switch from looping
                        flag_mainmenu <= '0'; 
                    end if;
                    counter <= 300000; -- Reset counter after it reaches 0
                end if;
                -- shows a growing line of invisibility
                if int_vcount >= 271 - whiteline and int_vcount < 271 + whiteline then
                    RGB <= x"001"; 
                end if;
            end if;
            
            --gamescreen -> mainmenu
            if flag_switch2 = '1' then
            --if blok increasing from 0 to 6 more bars a getting shown.
                if flag = '0' then
                    blok <= 0;
                    flag <= '1';
                    flag_mainmenu <= '1';
                end if;
                if counter1 > 0 then
                    counter1 <= counter1 - 1;
                else
                    blok <= blok + 1; 
                    counter1 <= 10000000;
                    if blok = 6 then
                        flag_switch2 <= '0';
                        flag <= '0';
                    end if;
                end if;
            end if;
            
        else
            RGB <= (others => '0'); -- black
        end if;
        main <= flag_mainmenu;
    end if;
end process;    
end Behavioral;