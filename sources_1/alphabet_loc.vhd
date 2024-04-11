library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

entity alphabet_loc is
    Port ( 
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        letterselector : in std_logic_vector(5 downto 0);
        containerselector : in std_logic_vector(3 downto 0);
        positieselector : in std_logic_vector(3 downto 0);
        data_in : in std_logic_vector(0 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        sprite_index_out : out std_logic_vector(13 downto 0)
         );
end alphabet_loc;

--this module shows all the letters of the alphabet on 10 different containers that are 16 entities long
architecture Behavioral of alphabet_loc is
    signal int_hcount : integer range 0 to 1024 := 0;
    signal int_vcount : integer range 0 to 512 := 0;
    signal int_positie : integer range 0 to 16 := 0;
    signal sprite_index1, teller1, counter1 : integer range 0 to 16 := 0;
    signal sprite_index2, teller2, counter2 : integer range 0 to 16 := 0;
    signal sprite_index3, teller3, counter3 : integer range 0 to 16 := 0;
    signal sprite_index4, teller4, counter4 : integer range 0 to 16 := 0;
    signal sprite_index5, teller5, counter5 : integer range 0 to 16 := 0;
    signal sprite_index6, teller6, counter6 : integer range 0 to 16 := 0;
    signal sprite_index7, teller7, counter7 : integer range 0 to 16 := 0;
    signal sprite_index8, teller8, counter8 : integer range 0 to 16 := 0;
    signal sprite_index9, teller9, counter9 : integer range 0 to 16 := 0;
    signal sprite_index10, teller10, counter10 : integer range 0 to 16 := 0;
    signal int_letter : integer range 0 to 64 := 0;
    type container is array (0 to 15) of integer range 0 to 36;
    signal flag : std_logic := '0';
    signal container1,container2,container3,container4,container5,container6,container7,container8 : container;
    signal container9,container10,container11,container12,container13,container14,container15,container16 : container;

begin
    process(clk)
    begin
        if(rising_edge(clk)) then
            int_positie <= To_integer(unsigned(positieselector));
            int_letter <= To_integer(unsigned(letterselector));
            
            --enters the right letters into the correct container and position
            case containerselector is
                        when "0000" =>
                            container1(int_positie) <= int_letter;
                        when "0001" =>
                            container2(int_positie) <= int_letter;
                        when "0010" =>
                            container3(int_positie) <= int_letter;
                        when "0011" =>
                            container4(int_positie) <= int_letter;
                        when "0100" =>
                            container5(int_positie) <= int_letter;
                        when "0101" =>
                            container6(int_positie) <= int_letter;
                        when "0110" =>
                            container7(int_positie) <= int_letter;
                        when "0111" =>
                            container8(int_positie) <= int_letter;
                        when "1000" =>
                            container9(int_positie) <= int_letter;
                        when "1001" =>
                            container10(int_positie) <= int_letter;
                        when "1010" =>
                            container11(int_positie) <= int_letter;
                        when "1011" =>
                            container12(int_positie) <= int_letter;
                        when "1100" =>
                            container13(int_positie) <= int_letter;
                        when "1101" =>
                            container14(int_positie) <= int_letter;
                        when "1110" =>
                            container15(int_positie) <= int_letter;
                        when "1111" =>
                            container16(int_positie) <= int_letter;
                        when others =>
                            RGB <= "000000000001";
                        end case; 
            
            int_hcount <= To_integer(unsigned(hcount)) + 2;
            int_vcount <= To_integer(unsigned(vcount));

            if int_hcount < 144 and int_vcount < 31 then
                --prevents screen tearing
                sprite_index1 <= 0;
                sprite_index2 <= 0;
            end if;
            
            if int_hcount >= 144 and int_hcount < 400 and int_vcount >= 33 and int_vcount < 49 then  
                --the sprite_index1 counts to 15 because the letters are 16x16 pixels
                --because there are more than 1 letters being shown the sprite index needs to reset
                --every first layer from 0 to 15. So when sprite index gets reset teller1 goes one up
                --for the length of the container this being 16. If this happens a third counter goed up
                --This last counter adds + 16 to this calculation. Meaning it is on the second row of the
                --sprite. This way all letters can be shown simultaneously.
                --This code is for 1 container of the 10 total. The rest is the same execpt the location
                --it is being shown. And the counters are unique.                       
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index1+(container1(teller1)*256) + (counter1 * 16),14));
                if sprite_index1 = 15 then
                    sprite_index1 <= 0;
                    teller1 <= teller1 + 1;
                    if teller1 = 15 then
                        teller1 <= 0;
                        counter1 <= counter1 + 1;
                        if counter1 = 15 then
                            counter1 <= 0;
                        end if;
                    end if;
                else
                    sprite_index1 <= sprite_index1 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 484 and int_hcount < 740 and int_vcount >= 33 and int_vcount < 49 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index2+(container2(teller2)*256) + (counter2 * 16),14));
                if sprite_index2 = 15 then
                    sprite_index2 <= 0;
                    teller2 <= teller2 + 1;
                    if teller2 = 15 then
                        teller2 <= 0;
                        counter2 <= counter2 + 1;
                        if counter2 = 15 then
                            counter2 <= 0;
                        end if;
                    end if;
                else
                    sprite_index2 <= sprite_index2 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 144 and int_hcount < 400 and int_vcount >= 493 and int_vcount < 509 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index3+(container3(teller3)*256) + (counter3 * 16),14));
                if sprite_index3 = 15 then
                    sprite_index3 <= 0;
                    teller3 <= teller3 + 1;
                    if teller3 = 15 then
                        teller3 <= 0;
                        counter3 <= counter3 + 1;
                        if counter3 = 15 then
                            counter3 <= 0;
                        end if;
                    end if;
                else
                    sprite_index3 <= sprite_index3 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 484 and int_hcount < 740 and int_vcount >= 493 and int_vcount < 509 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index4+(container4(teller4)*256) + (counter4 * 16),14));
                if sprite_index4 = 15 then
                    sprite_index4 <= 0;
                    teller4 <= teller4 + 1;
                    if teller4 = 15 then
                        teller4 <= 0;
                        counter4 <= counter4 + 1;
                        if counter4 = 15 then
                            counter4 <= 0;
                        end if;
                    end if;
                else
                    sprite_index4 <= sprite_index4 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;

            if int_hcount >= 314 and int_hcount < 570 and int_vcount >= 50 and int_vcount < 66 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index5+(container5(teller5)*256) + (counter5 * 16),14));
                if sprite_index5 = 15 then
                    sprite_index5 <= 0;
                    teller5 <= teller5 + 1;
                    if teller5 = 15 then
                        teller5 <= 0;
                        counter5 <= counter5 + 1;
                        if counter5 = 15 then
                            counter5 <= 0;
                        end if;
                    end if;
                else
                    sprite_index5 <= sprite_index5 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 314 and int_hcount < 570 and int_vcount >= 121 and int_vcount < 137 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index6+(container6(teller6)*256) + (counter6 * 16),14));
                if sprite_index6 = 15 then
                    sprite_index6 <= 0;
                    teller6 <= teller6 + 1;
                    if teller6 = 15 then
                        teller6 <= 0;
                        counter6 <= counter6 + 1;
                        if counter6 = 15 then
                            counter6 <= 0;
                        end if;
                    end if;
                else
                    sprite_index6 <= sprite_index6 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 314 and int_hcount < 570 and int_vcount >= 192 and int_vcount < 208 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index7+(container7(teller7)*256) + (counter7 * 16),14));
                if sprite_index7 = 15 then
                    sprite_index7 <= 0;
                    teller7 <= teller7 + 1;
                    if teller7 = 15 then
                        teller7 <= 0;
                        counter7 <= counter7 + 1;
                        if counter7 = 15 then
                            counter7 <= 0;
                        end if;
                    end if;
                else
                    sprite_index7 <= sprite_index7 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 314 and int_hcount < 570 and int_vcount >= 263 and int_vcount < 279 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index8+(container8(teller8)*256) + (counter8 * 16),14));
                if sprite_index8 = 15 then
                    sprite_index8 <= 0;
                    teller8 <= teller8 + 1;
                    if teller8 = 15 then
                        teller8 <= 0;
                        counter8 <= counter8 + 1;
                        if counter8 = 15 then
                            counter8 <= 0;
                        end if;
                    end if;
                else
                    sprite_index8 <= sprite_index8 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 314 and int_hcount < 570 and int_vcount >= 334 and int_vcount < 350 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index9+(container9(teller9)*256) + (counter9 * 16),14));
                if sprite_index9 = 15 then
                    sprite_index9 <= 0;
                    teller9 <= teller9 + 1;
                    if teller9 = 15 then
                        teller9 <= 0;
                        counter9 <= counter9 + 1;
                        if counter9 = 15 then
                            counter9 <= 0;
                        end if;
                    end if;
                else
                    sprite_index9 <= sprite_index9 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if int_hcount >= 314 and int_hcount < 570 and int_vcount >= 405 and int_vcount < 421 then                         
                sprite_index_out <= std_logic_vector(to_unsigned(sprite_index10+(container10(teller10)*256) + (counter10 * 16),14));
                if sprite_index10 = 15 then
                    sprite_index10 <= 0;
                    teller10 <= teller10 + 1;
                    if teller10 = 15 then
                        teller10 <= 0;
                        counter10 <= counter10 + 1;
                        if counter10 = 15 then
                            counter10 <= 0;
                        end if;
                    end if;
                else
                    sprite_index10 <= sprite_index10 + 1;            
                end if;
                flag <= '1';
            else 
               RGB <= "000000000001";
            end if;
            
            if data_in = 1 then
            --data in changes based of sprite_index_out
                    RGB <= x"F00";
                else 
                    RGB <= "000000000001";
                end if;   
        end if;    
    end process;

end Behavioral;
