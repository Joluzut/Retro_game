library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity spiDecoder is
  Port ( dataRead : in std_logic_vector(23 downto 0) := (others => '0'); --  data read 
         Clk : in std_logic;
         enable : in std_logic;
         tankdata_x : out std_logic_vector(9 downto 0):= (others => '0');
         tankdata_y : out std_logic_vector(9 downto 0):= (others => '0');
         tankdata_x1 : out std_logic_vector(9 downto 0):= (others => '0');
         tankdata_y1 : out std_logic_vector(9 downto 0):= (others => '0');
         bulletdata_x : out std_logic_vector(9 downto 0):= (others => '0');
         bulletdata_y : out std_logic_vector(9 downto 0):= (others => '0');
         ground_onoff : out std_logic;
         grounddata_x : out std_logic_vector(6 downto 0):= (others => '0');
         grounddata_y : out std_logic_vector(4 downto 0):= (others => '0');
         ground_enable : out std_logic;
         letterselector : out std_logic_vector(5 downto 0);
         containerselector : out std_logic_vector(3 downto 0);
         positieselector : out std_logic_vector(3 downto 0);
         barrelloc1, barrelloc2 : out std_logic_vector(3 downto 0);
         switch : out std_logic_vector(15 downto 0);
         music : out std_logic_vector(11 downto 0)
  );
end spiDecoder;

--This module translates the recieved spi to useable data for this code
architecture Behavioral of spiDecoder is
signal signal_tankdata_x, signal_tankdata_y : std_logic_vector(9 downto 0);
signal signal_tankdata_x1, signal_tankdata_y1 : std_logic_vector(9 downto 0);
signal signal_bulletdata_x, signal_bulletdata_y : std_logic_vector(9 downto 0);
signal flag : std_logic;

begin
    process (Clk)
    begin
        if enable = '0' then
            case dataRead(23 downto 16) is
                when "00000000" =>--tank 1 x position
                    signal_tankdata_x <= dataRead(9 downto 0);
                    tankdata_x <= signal_tankdata_x;
                when "00000001" =>--tank 1 y position
                    signal_tankdata_y <= dataRead(9 downto 0);
                    tankdata_y <= signal_tankdata_y;
                when "00010000" =>--tank 2 x position
                    signal_tankdata_x1 <= dataRead(9 downto 0);
                    tankdata_x1 <= signal_tankdata_x1;
                when "00010001" => --tank 2 y position
                    signal_tankdata_y1 <= dataRead(9 downto 0);
                    tankdata_y1 <= signal_tankdata_y1;
                when "00100000" =>--bullet x position
                    signal_bulletdata_x <= dataRead(9 downto 0);
                    bulletdata_x <= signal_bulletdata_x;
                when "00100001" =>--bullet y position
                    signal_bulletdata_y <= dataRead(9 downto 0);
                    bulletdata_y <= signal_bulletdata_y;
                when "00110101" =>--floortile on
                    ground_onoff <= '1';
                    grounddata_x <= dataRead(15 downto 9);
                    grounddata_y <= dataRead(8 downto 4);
                when "00110100" =>--floortile off
                    ground_onoff <= '0';
                    grounddata_x <= dataRead(15 downto 9);
                    grounddata_y <= dataRead(8 downto 4);
                when "01110001" =>--specific letter on specific place
                    letterselector <= dataRead(5 downto 0);
                    containerselector <= dataRead(15 downto 12);
                    positieselector <= dataRead(11 downto 8);
                when "01010101" =>--position barrel tank 1
                    barrelloc1 <= dataRead(3 downto 0);
                when "01100110" =>--position barrel tank 2
                    barrelloc2 <= dataRead(3 downto 0);
                when "11100011" =>--switch between mainmenu -> gamescreen, gamescreen -> mainmenu
                    switch <= dataRead(15 downto 0);
                    flag <= '1';
                when "11000011" =>--soundeffects
                    music <= dataRead(11 downto 0);
                when others =>
                    NULL;
            end case;
        end if;
    end process;

end Behavioral;
