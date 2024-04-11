library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity MusicPlayer is
 Port ( 
    main : in std_logic;
    pwm_out : out  STD_LOGIC_VECTOR (12 downto 0);
    clk : in std_logic;
    music : in std_logic_vector(11 downto 0)
 );
end MusicPlayer;

--this module plays 2 songs standard depending of which screen it is on and can play soundeffects
architecture Behavioral of MusicPlayer is
signal counter : integer range 0 to 10000000 := 0;
signal counter2 : integer range 0 to 21 := 0;
signal counter3 : integer range 0 to 10000000 := 0;
signal counter4 : integer range 0 to 11 := 0;
signal signal_pwm : integer range 0 to 8000;
signal explosie, shoot, victory, flag : std_logic;

begin
    process (clk)
    begin
        if rising_edge(clk) then
        
        --decodes spi_decoder  to usable data for this module
        if music(3 downto 0) = "1111" then
            explosie <= '1';
            shoot <= '0';
            victory <= '0';
            flag <= '1';
        end if;
        if music(7 downto 4) = "1111" then
            explosie <= '0';
            shoot <= '1';
            victory <= '0';
            flag <= '1';
        end if;
        if music(11 downto 8) = "1111" then
            explosie <= '0';
            shoot <= '0';
            victory <= '1';
            flag <= '1';
        end if;
        
        --counter for normal music
        counter <= counter + 1;
        if counter = 10000000 then
            counter <= 0;
            counter2 <= counter2 + 1;
            if counter2 = 20 then
                counter2 <= 0;
            end if;
        end if;
           
        --counter for soundeffects 
        if flag = '1' then
            counter3 <= counter3 + 1;
            if counter3 = 1000000 then
                counter3 <= 0;
                counter4 <= counter4 + 1;
                if counter4 = 10 then
                --when the time is done everything is reset
                    counter4 <= 0;
                    flag <= '0';
                    explosie <= '0';
                    shoot <= '0';
                    victory <= '0';
                end if;
            end if;
        end if;
            
        --plays if in the mainscreen
        if main = '1' then
            case counter2 is 
                when 0 =>
                    signal_pwm <= 587; -- D (440 * 1.33484)
            when 1 =>
                signal_pwm <= 523; -- C# (392 * 1.33484)
            when 2 =>
                signal_pwm <= 466; -- A# (349 * 1.33484)
            when 3 =>
                signal_pwm <= 440; -- A (330 * 1.33484)
            when 4 =>
                signal_pwm <= 392; -- G (294 * 1.33484)
            when 5 =>
                signal_pwm <= 349; -- F (262 * 1.33484)
            when 6 =>
                signal_pwm <= 392; -- G (294 * 1.33484)
            when 7 =>
                signal_pwm <= 349; -- F (262 * 1.33484)
            when 8 =>
                signal_pwm <= 392; -- G (294 * 1.33484)
            when 9 =>
                signal_pwm <= 440; -- A (330 * 1.33484)
            when 10 =>
                signal_pwm <= 659; -- E (494 * 1.33484)
            when 11 =>
                signal_pwm <= 587; -- D (440 * 1.33484)
            when 12 =>
                signal_pwm <= 523; -- C# (392 * 1.33484)
            when 13 =>
               signal_pwm <= 466; -- A# (349 * 1.33484)
            when 14 =>
               signal_pwm <= 440; -- A (330 * 1.33484)
            when 15 =>
               signal_pwm <= 392; -- G (294 * 1.33484)
            when 16 =>
               signal_pwm <= 349; -- F (262 * 1.33484)
            when 17 =>
               signal_pwm <= 392; -- G (294 * 1.33484)
            when 18 =>
               signal_pwm <= 440; -- A (330 * 1.33484)
            when 19 =>
               signal_pwm <= 466; -- A# (349 * 1.33484)
            when 20 =>
               signal_pwm <= 523; -- C# (392 * 1.33484)
            when others =>
                NULL;
            end case;
        end if;
        
        --plays this tune on the gamescreen    
        if main = '0' then
            case counter2 is 
            when 0 =>
                signal_pwm <= 440; 
            when 1 =>
                signal_pwm <= 392;
            when 2 =>
                signal_pwm <= 349;
            when 3 =>
                signal_pwm <= 330;
            when 4 =>
                signal_pwm <= 294;
            when 5 =>
                signal_pwm <= 262;
            when 6 =>
                signal_pwm <= 294;
            when 7 =>
                signal_pwm <= 262;
            when 8 =>
                signal_pwm <= 294;
            when 9 =>
                signal_pwm <= 330;
            when 10 =>
                signal_pwm <= 494;
            when 11 =>
                signal_pwm <= 440;
            when 12 =>
                signal_pwm <= 392;
            when 13 =>
               signal_pwm <= 349;
            when 14 =>
               signal_pwm <= 330;
            when 15 =>
               signal_pwm <= 294;
            when 16 =>
               signal_pwm <= 262;
            when 17 =>
               signal_pwm <= 294;
            when 18 =>
               signal_pwm <= 330;
            when 19 =>
               signal_pwm <= 349;
            when 20 =>
               signal_pwm <= 392;
            when others =>
               NULL;
            end case;
            
            if explosie = '1' then
                case counter4 is 
                when 0 =>
                    signal_pwm <= 50; 
                when 1 =>
                    signal_pwm <= 160;
                when 2 =>
                    signal_pwm <= 70;
                when 3 =>
                    signal_pwm <= 180;
                when 4 =>
                    signal_pwm <= 90;
                when 5 =>
                    signal_pwm <= 200;
                when 6 =>
                    signal_pwm <= 100; 
                when 7 =>
                    signal_pwm <= 220;
                when 8 =>
                    signal_pwm <= 110;
                when 9 =>
                    signal_pwm <= 240;
                when 10 =>
                    signal_pwm <= 130;
                when others =>
                    NULL;
                end case;  
            end if;   
            
            if shoot = '1' then
                case counter4 is 
                when 0 =>
                    signal_pwm <= 300; 
                when 1 =>
                    signal_pwm <= 410;
                when 2 =>
                    signal_pwm <= 320;
                when 3 =>
                    signal_pwm <= 430;
                when 4 =>
                    signal_pwm <= 340;
                when 5 =>
                    signal_pwm <= 450;
                when 6 =>
                    signal_pwm <= 360; 
                when 7 =>
                    signal_pwm <= 470;
                    shoot <= '0';
                    flag <= '0';
                    counter4 <= 0;
                when 8 =>
                    signal_pwm <= 380;
                when 9 =>
                    signal_pwm <= 490;
                when 10 =>
                    signal_pwm <= 400;
                when others =>
                    NULL;
                end case;
            end if;
                
            if victory = '1' then
                case counter4 is 
                when 0 =>
                    signal_pwm <= 600; 
                when 1 =>
                    signal_pwm <= 600;
                when 2 =>
                    signal_pwm <= 600;
                when 3 =>
                    signal_pwm <= 800;
                when 4 =>
                    signal_pwm <= 800;
                when 5 =>
                    signal_pwm <= 800;
                when 6 =>
                    signal_pwm <= 500; 
                when 7 =>
                    signal_pwm <= 500;
                when 8 =>
                    signal_pwm <= 500;
                when 9 =>
                    signal_pwm <= 900;
                when 10 =>
                    signal_pwm <= 900;
                when others =>
                    NULL;
                end case;  
            end if;
            end if;
            pwm_out <= std_logic_vector(To_unsigned(signal_pwm,13));
        end if;
    end process;

end Behavioral;
