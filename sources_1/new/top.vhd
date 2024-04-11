library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity top is
  port (
    -- Define input ports
    Reset : in std_logic;
    clk : in std_logic;
    clkSPI : in std_logic; -- incoming clock of SPI 
    dataSPI : in std_logic; -- incoming data of SPI
    csSPI : in std_logic;   -- incoming select of SPI
    clrSPI : in std_logic;
    
    -- Define output ports
    Red, Green, Blue : out std_logic_vector(3 downto 0);
    hsync : out std_logic;
    vsync : out std_logic;
    dataout : out std_logic_vector(15 downto 0);
    pwm_out : out std_logic
  );
end entity top;

-- Define the architecture for the combined module
architecture Behavioral of top is
  -- Instantiate the VGA module
  component VGA is
    Port ( 
        clk_25 : in STD_LOGIC;
        reset : in STD_LOGIC;
        RGB_IN : in STD_LOGIC_VECTOR(11 downto 0);
        RGB : out STD_LOGIC_VECTOR(11 downto 0);
        HSync, VSync : out STD_LOGIC;
        HCount : out STD_LOGIC_VECTOR(9 downto 0);
        VCount : out STD_LOGIC_VECTOR(9 downto 0)
    );
    end component VGA;

  -- Instantiate the clocking wizard module
  component prescaler is
    port (
        clk_25 : out std_logic;
        clk_8 : out std_logic;
        reset : in std_logic;
        clk_100 : in std_logic
    );
  end component prescaler;

  -- Instantiate the bullet_ROM module
  component bullet_rom is                                             
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        sprite_x : in std_logic_vector(9 downto 0);
        sprite_y : in std_logic_vector(9 downto 0);
        RGB : out std_logic_vector(11 downto 0)
    );  
  end component bullet_rom;
  
  -- Instantiate the floor_ROM module  
  component floor_rom is
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);
        vcount : in std_logic_vector(9 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        ground_onoff, ground_enable : in std_logic;
        grounddata_x : in std_logic_vector(6 downto 0):= (others => '0');
        grounddata_y : in std_logic_vector(4 downto 0):= (others => '0');
        enable : in std_logic
    );
  end component floor_rom;
  
  -- Instantiate the pre_VGA module
  component preVGA is
    Port ( 
        clk : in std_logic;
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
  end component preVGA;
  
  -- Instantiate the spi_Slave module
  component spiSlave is
    Port (
        clkBoard : in std_logic; -- clock basys3 100MHz
        clkSPI : in std_logic; -- incoming clock of SPI 
        dataSPI : in std_logic; -- incoming data of SPI
        csSPI : in std_logic;   -- incoming select of SPI
        clrSPI : in std_logic;
        dataRead : out std_logic_vector(23 downto 0) := (others => '0'); --  data read
        enable_out : out std_logic
    );
  end component spiSlave;

  -- Instantiate the tank_ROM module
  component tank_rom is  
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        sprite_x : in std_logic_vector(9 downto 0);
        sprite_y : in std_logic_vector(9 downto 0);
        barrelloc : in std_logic_vector(3 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        color : in std_logic
    );
  end component tank_rom;

  -- Instantiate the ROM module
  component spiDecoder is
    Port ( dataRead : in std_logic_vector(23 downto 0) := (others => '0'); --  data read 
         Clk : in std_logic;
         tankdata_x : out std_logic_vector(9 downto 0);
         tankdata_y : out std_logic_vector(9 downto 0);
         enable : in std_logic;
         ground_onoff, ground_enable : out std_logic;
         grounddata_x : out std_logic_vector(6 downto 0):= (others => '0');
         grounddata_y : out std_logic_vector(4 downto 0):= (others => '0');
         tankdata_x1 : out std_logic_vector(9 downto 0):= (others => '0');
         tankdata_y1 : out std_logic_vector(9 downto 0):= (others => '0');
         bulletdata_x : out std_logic_vector(9 downto 0):= (others => '0');
         bulletdata_y : out std_logic_vector(9 downto 0):= (others => '0');
         letterselector : out std_logic_vector(5 downto 0);
         containerselector : out std_logic_vector(3 downto 0);
         positieselector : out std_logic_vector(3 downto 0);
         barrelloc1, barrelloc2 : out std_logic_vector(3 downto 0);
         switch: out std_logic_vector(15 downto 0);
         music : out std_logic_vector(11 downto 0)
    );
  end component spiDecoder;
  
  -- Instantiate the show alphabet module
  component alphabet_loc is
    Port ( 
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        data_in : in std_logic_vector(0 downto 0);
        letterselector : in std_logic_vector(5 downto 0);
        containerselector : in std_logic_vector(3 downto 0);
        positieselector : in std_logic_vector(3 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        sprite_index_out : out std_logic_vector(13 downto 0)
    );
  end component alphabet_loc;

  -- Instantiate the Alphabet_ROM module
  COMPONENT Alphabet_Rom IS
    PORT (
      clka : IN STD_LOGIC;
      addra : IN STD_LOGIC_VECTOR(13 DOWNTO 0);
      douta : OUT STD_LOGIC_VECTOR(0 DOWNTO 0)
    );
  END COMPONENT Alphabet_Rom;

  -- Instantiate the mainmenu/switch module
  component screen_rom is                                            
    port (
        clk : in std_logic;
        hcount : in std_logic_vector(9 downto 0);           
        vcount : in std_logic_vector(9 downto 0);
        RGB : out std_logic_vector(11 downto 0);
        switch : in std_logic_vector(15 downto 0);
        main : out std_logic
    );
  end component screen_rom;

  -- Instantiate the PWM module
  component PWM is
    Port ( clk : in  STD_LOGIC;
           pwm_in : in  STD_LOGIC_VECTOR (12 downto 0);  -- Input PWM frequency
           pwm_out : out  STD_LOGIC
           );
  end component PWM;

  -- Instantiate the sounds module
  component MusicPlayer is
    Port ( 
        pwm_out : out  STD_LOGIC_VECTOR (12 downto 0);
        clk : in std_logic;
        main : in std_logic;
        music : in std_logic_vector(11 downto 0)
      );
  end component MusicPlayer;

--signals
signal signal_hcount : std_logic_vector(9 downto 0);
signal signal_vcount : std_logic_vector(9 downto 0);
signal sprite_x, sprite_y : std_logic_vector(9 downto 0);
signal tankdata_x1, tankdata_y1, bulletdata_x, bulletdata_y : std_logic_vector(9 downto 0);
signal clk_25, clk_8 : std_logic;
signal signal_RGB, signal_RGB1, signal_RGB2, signal_RGB3, signal_RGB4, signal_RGB5, signal_RGB6, signal_RGB7 : std_logic_vector(11 downto 0);
signal RGB_OUT : std_logic_vector(11 downto 0);
signal dataRead : std_logic_vector(23 downto 0);
signal enable : std_logic;
signal signal_ground_onoff, signal_ground_enable : std_logic;
signal signal_grounddata_x : std_logic_vector(6 downto 0):= (others => '0');
signal signal_grounddata_y : std_logic_vector(4 downto 0):= (others => '0');
signal addra_signal : STD_LOGIC_VECTOR(13 DOWNTO 0);
signal addra_signal1 : STD_LOGIC_VECTOR(11 DOWNTO 0);
signal douta_signal, douta_signal1 : STD_LOGIC_VECTOR(0 DOWNTO 0);
signal letterselector : std_logic_vector(5 downto 0);
signal containerselector : std_logic_vector(3 downto 0);
signal positieselector : std_logic_vector(3 downto 0);
signal barrelloc1, barrelloc2 : std_logic_vector(3 downto 0);
signal color1, color2, mainmenu : std_logic;
signal pwm_in : STD_LOGIC_VECTOR (12 downto 0);
signal switch : std_logic_vector(15 downto 0);
signal music : std_logic_vector(11 downto 0);

begin
  --for the color of the different tanks
  color1 <= '1';
  color2 <= '0';

  VGA_inst : VGA
    port map (
      clk_25 => clk_25,
      Reset => Reset,
      RGB_IN => signal_RGB,
      hcount => signal_hcount,
      vcount => signal_vcount,
      hsync => hsync,
      vsync => vsync,
      RGB => RGB_OUT
    );
     
    prescaler_inst : prescaler
    port map (
        clk_25 => clk_25,
        clk_8 => clk_8,
        reset => reset,
        clk_100 => clk
    );
    
    bullet_rom_inst : bullet_rom
    port map (
        clk => clk_25,
        hcount => signal_hcount,
        vcount => signal_vcount,
        sprite_x => bulletdata_x,
        sprite_y => bulletdata_y,
        RGB => signal_RGB2
    );

    tank_rom_inst : tank_rom
    port map (
        clk => clk_25,
        hcount => signal_hcount,
        vcount => signal_vcount,
        sprite_x => sprite_x,
        sprite_y => sprite_y,
        RGB => signal_RGB4,
        barrelloc => barrelloc1,
        color => color1
    );
    
    tank_rom1_inst : tank_rom
    port map (
        clk => clk_25,
        hcount => signal_hcount,
        vcount => signal_vcount,
        sprite_x => tankdata_x1,
        sprite_y => tankdata_y1,
        RGB => signal_RGB5,
        barrelloc => barrelloc2,
        color => color2
    );
    
    preVGA_inst : preVGA
    port map (
        clk => clk_25,
        RGB => signal_RGB,
        RGB1 => signal_RGB1,
        RGB2 => signal_RGB2,
        RGB3 => signal_RGB3,
        RGB4 => signal_RGB4,
        RGB5 => signal_RGB5,
        RGB6 => signal_RGB6,
        main => mainmenu,
        RGB7 => signal_RGB7
    );
  
    floor_rom_inst : floor_rom
    port map (
        clk => clk_25,
        hcount => signal_hcount,
        vcount => signal_vcount,
        RGB => signal_RGB3,
        ground_onoff => signal_ground_onoff,
        grounddata_x => signal_grounddata_x,
        grounddata_y => signal_grounddata_y,
        ground_enable => signal_ground_enable,
        enable => enable
    );
    
    spiSlave_inst : spiSlave
    port map (
        clkBoard => clk,
        clkSPI => clkSPI,
        dataSPI => dataSPI,
        csSPI => csSPI,
        clrSPI => clrSPI,
        dataRead => dataRead,
        enable_out => enable
    );
        
    spiDecoder_inst : spiDecoder
    port map ( 
        dataRead => dataRead,
        Clk  => clk_25,
        tankdata_x => sprite_x,
        tankdata_y => sprite_y,
        tankdata_x1 => tankdata_x1,
        tankdata_y1 => tankdata_y1,
        bulletdata_x => bulletdata_x,
        bulletdata_y => bulletdata_y,
        enable => enable,
        ground_onoff => signal_ground_onoff,
        grounddata_x => signal_grounddata_x,
        grounddata_y => signal_grounddata_y,
        ground_enable => signal_ground_enable,
        letterselector => letterselector,
        containerselector => containerselector,
        positieselector => positieselector,
        barrelloc1 => barrelloc1,
        barrelloc2 => barrelloc2,
        switch => switch,
        music => music
    );
    
  alphabet_loc_inst : alphabet_loc
    Port map ( 
        clk => clk_25,
        hcount => signal_hcount,   
        vcount => signal_vcount,
        data_in => douta_signal,
        RGB => signal_RGB1,
        sprite_index_out => addra_signal,
        letterselector => letterselector,
        containerselector => containerselector,
        positieselector => positieselector
    );
         
  Alphabet_Rom_inst : Alphabet_Rom
    PORT map (
        clka => clk_25,
        addra => addra_signal,
        douta => douta_signal
    );
    
  screen_rom_inst : screen_rom                                            
    port map (
        clk => clk_25,
        hcount => signal_hcount,           
        vcount => signal_vcount,
        RGB => signal_RGB6,
        switch => switch,
        main => mainmenu
    );
    
  PWM_inst : PWM
    Port map( 
        clk => clk_8,
        pwm_in => pwm_in,
        pwm_out => pwm_out
    );
    
  MusicPlayer_inst : MusicPlayer 
    Port map ( 
        pwm_out => pwm_in,
        clk => clk_25,
        main => mainmenu,
        music => music
    );

  --for the output of the vga
  Red <= RGB_OUT(11 downto 8);
  Green <= RGB_OUT(7 downto 4);
  Blue <= RGB_OUT(3 downto 0);
  
  --for debugging on the basys3
  dataout(15 downto 12) <= dataread(23 downto 20);
  dataout(11 downto 0) <= dataread(11 downto 0);
  
end architecture Behavioral;