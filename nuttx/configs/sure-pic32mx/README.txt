configs/pic32mx README
=====================

This README file discusses the port of NuttX to the "Advanced USB Storage
Demo Board," Model DB-DP11215, from Sure Electronics
(http://www.sureelectronics.net/).  This board features the MicroChip
PIC32MX440F512H.  See also http://www.sureelectronics.net/goods.php?id=1168
for further information about the Sure DB-DP11215 board.

Contents
========

  PIC32MX440F512H Pin Out
  Toolchains
  Loading NuttX with PICkit2
  PIC32MX Configuration Options
  Configurations

PIC32MX440F512H Pin Out
=======================

  PIC32MX440F512H 64-Pin QFN (USB) Pin Out.

  LEFT SIDE, TOP-TO-BOTTOM (if pin 1 is in upper left)
  PIN  NAME                          SIGNAL         NOTES
  ---- ----------------------------- -------------- -------------------------------
    1  PMD5/RE5                      PMPD5          Display, JP1-12, DB4
    2  PMD6/RE6                      PMPD6          Display, JP1-13, DB6
    3  PMD7/RE7                      PMPD7          Display, JP1-14, DB7
    4  SCK2/PMA5/CN8/RG6             SCK            SD connector SCK, FLASH (U1) SCK*
    5  SDI2/PMA4/CN9/RG7             SDI            SD connector DO, FLASH (U1) SO*
    6  SDO2/PMA3/CN10/RG8            SDO            SD connector DI, FLASH (U1) SI*
    7  MCLR\                         PIC_MCLR       Pulled high, J7-1, ICSP
    8  SS2/PMA2/CN11/RG9             UTIL_CS        FLASH (U1) CS*
    9  Vss                                          Grounded
   10  Vdd                           +3.3V          ---
   11  AN5/C1IN+/Vbuson/CN7/RB5      Vbuson/AN5/RB5 To USB VBUS circuitry
   12  AN4/C1IN-/CN6/RB4             SW_OK          SW3, Pull high, low means SW3 closed
   13  AN3/C2IN+/CN5/RB3             SW_UP          SW1, Pull high, low means SW1 closed
   14  AN2/C2IN-/CN4/RB2             SW_Down        SW2, Pull high, low means SW2 closed
   15  PGEC1/AN1/Vref-/CVref-/CN3/   ADC_SENSE_SWITCHED_+VBUS To USB VBUS circuitry
       RB1
   16  PGED1/AN0/VREF+/CVREF+/PMA6/                     N/C            Not connected
       CN2/RB0
  *FLASH (U1, SOIOC) not populated

  BOTTOM SIDE, LEFT-TO-RIGHT (if pin 1 is in upper left)
  PIN  NAME                          SIGNAL         NOTES
  ---- ----------------------------- -------------- -------------------------------
   17  PGEC2/AN6/OCFA/RB6            PIC_PGC2       J7-5, ICSP
   18  PGED2/AN7/RB7                 PIC_PGD2       J7-4, ICSP
   19  AVdd                          +3.3V          ---
   20  AVss                                         Grounded
   21  AN8/U2CTS/C1OUT/RB8           N/C            Not connected
   22  AN9/C2OUT/PMA7/RB9            N/C            Not connected
   23  TMS/AN10/CVREFOUT/PMA13/RB10  UTIL_WP        FLASH (U1) WP*
   24  TDO/AN11/PMA12//RB11          SD_CS          SD connector CS
   25  Vss                                          Grounded
   26  Vdd                           +3.3V          ---
   27  TCK/AN12/PMA11/RB12           SD_CD          SD connector CD
   28  TDI/AN13/PMA10/RB13           SD_WD          SD connector WD
   29  AN14/U2RTS/PMALH/PMA1/RB14    N/C            Not connected
   30  AN15/OCFB/PMALL/PMA0/CN12/    PMPA0          Display, JP1-4, RS
       RB15
   31  SDA2/U2RX/PMA9/CN17/RF4       RXD2_MCU       J5 DB9 via RS232 driver
   32  SCL2/U2TX/PMA8/CN18/RF5       TXD2_MCU       J5 DB9 via RS232 driver

  *FLASH (U1, SOIOC) not populated

  RIGHT SIDE, TOP-TO-BOTTOM (if pin 1 is in upper left)
  PIN  NAME                          SIGNAL         NOTES
  ---- ----------------------------- -------------- -------------------------------
   48  SOSCO/T1CK/CN0/RC14           SOSCO          32.768KHz XTAL (Y1)
   47  SOSCI/CN1/RC13                SOSCI          32.768KHz XTAL (Y1)
   46  OC1/INT0/RD0                  PWM1           Used to control backlight level (K)
   45  IC4/PMCS1/PMA14/INT4/RD11     PMPCS1         Display, JP1-6, E
   44  SCL1/IC3/PMCS2/PMA15/INT3/    USB_OPT        USB PHY
       RD10
   43  U1CTS/SDA1/IC2/INT2/RD9       USB_OPTEN      USB PHY
   42  RTCC/IC1/INT1/RD8             N/C            Not connected
   41  Vss                                          Grounded
   40  OSC2/CLKO/RC15                OSC2           20MHz XTAL (Y2)
   39  OSC1/CLKI/RC12                OSC1           20MHz XTAL (Y2)
   38  Vdd                           +3.3V          ---
   37  D+/RG2                        APPS_D+        USB connectors via PHY
   36  D-/RG3                        APPS_D-        USB connectors via PHY
   35  Vusb                          +3.3V          ---
   34  Vbus                          VBUS_DEVICE_MODE Display, USB Mini-B, USB Type A, JP1-1, +5V
   33  USBID/RF3                     N/C            Not connected

  TOP SIDE, LEFT-TO-RIGHT (if pin 1 is in upper left)
  PIN  NAME                          SIGNAL         NOTES
  ---- ----------------------------- -------------- -------------------------------
   64  PMPD4/RE4                     PMPD4          Display, JP1-11, DB4
   63  PMPD3/RE3                     PMPD3          Display, JP1-10, DB3
   62  PMPD2/RE2                     PMPD2          Display, JP1-9, DB2
   61  PMPD1/RE1                     PMPD1          Display, JP1-8, DB1
   60  PMPD0/RE0                     PMPD0          Display, JP1-7, DB0
   59  RF1                           RF1            Low illuminates LED/R/ERR
   58  RF0                           RF0            Low illuminates LED/Y/flash
   57  ENVREG                        ENVREG         Pulled high
   56  Vcap/Vddcore                  VDDCORE        Capactors to ground
   55  CN16/RD7                      RD7            Low illuminates LED/Y/USB
   54  CN15/RD6                      RD6            Low illuminates LED/Y/SD
   53  PMRD/CN14/RD5                 PMPRD          Display, JP1-5, R/W
   52  OC5/IC5/PMWR/CN13/RD4         N/C            Not connected
   51  U1TX/OC4/RD3                  CP2102_RXD     J6-3, UART1 (also CP2102*)
   50  U1RX/OC3/RD2                  CP2102_TXD     J6-2, UART1 (also CP2102*)
   49  U1RTS/OC2/RD1                 PWM2           Used to control backlight level (Vo)

  *USB-to-UART bridge (U1, CP2102) not populated

Toolchains
==========

  I am using the free, LITE version of the PIC32MX toolchain available
  for download from the microchip.com web site.  I am using the Windows
  version.  The MicroChip toolchain is the only toolchaing currently
  supported in these configurations, but it should be a simple matter to
  adapt to other toolchains by modifying the Make.defs file include in
  each configuration.

  Toolchain Options:

  CONFIG_PIC32MX_MICROCHIPW      - MicroChip full toolchain for Windows
  CONFIG_PIC32MX_MICROCHIPL      - MicroChip full toolchain for Linux
  CONFIG_PIC32MX_MICROCHIPW_LITE - MicroChip LITE toolchain for Windows
  CONFIG_PIC32MX_MICROCHIPL_LITE - MicroChip LITE toolchain for Linux

  Windows Native Toolchains
  
  NOTE:  There are several limitations to using a Windows based toolchain in a
  Cygwin environment.  The three biggest are:

  1. The Windows toolchain cannot follow Cygwin paths.  Path conversions are
     performed automatically in the Cygwin makefiles using the 'cygpath' utility
     but you might easily find some new path problems.  If so, check out 'cygpath -w'

  2. Windows toolchains cannot follow Cygwin symbolic links.  Many symbolic links
     are used in Nuttx (e.g., include/arch).  The make system works around these
     problems for the Windows tools by copying directories instead of linking them.
     But this can also cause some confusion for you:  For example, you may edit
     a file in a "linked" directory and find that your changes had not effect.
     That is because you are building the copy of the file in the "fake" symbolic
     directory.  If you use a Windows toolchain, you should get in the habit of
     making like this:

       make clean_context all

     An alias in your .bashrc file might make that less painful.

  3. Dependencies are not made when using Windows versions of the GCC.  This is
     because the dependencies are generated using Windows pathes which do not
     work with the Cygwin make.

     Support has been added for making dependencies with the windows-native toolchains.
     That support can be enabled by modifying your Make.defs file as follows:

    -  MKDEP                = $(TOPDIR)/tools/mknulldeps.sh
    +  MKDEP                = $(TOPDIR)/tools/mkdeps.sh --winpaths "$(TOPDIR)"

     If you have problems with the dependency build (for example, if you are not
     building on C:), then you may need to modify tools/mkdeps.sh

Loading NuttX with PICkit2
==========================

  NOTE:  You need a PICKit3 if you plan to use the MPLAB debugger!  The PICKit2
  can, however, still be used to load problems.  Instructions for the PICKit3
  are similar.

  Intel Hex Forma Files:
  ----------------------

    When NuttX is built it will produce two files in the top-level NuttX
    directory:

    1) nuttx - This is an ELF file, and
    2) nuttx.ihx - This is an Intel Hex format file.  This is controlled by
       the setting CONFIG_INTELHEX_BINARY in the .config file.

    The PICkit tool wants an Intel Hex format file to burn into FLASH.
    However, there are two problems with the generated nutt.ihx:
  
    1) The tool expects Intel Hex format files to be named *.hex.  This
       is not a significant issue.  However, just renaming the file to
       nuttx.hex is *not* sufficient.  There is another problem:
    2) The tool expects the nuttx.hex file to contain physical addresses.
       But the nuttx.ihx file generated from the top-level make will have
       address in the KSEG0 and KSEG1 regions.

  tools/mkpichex:
  ---------------

    There is a simple tool in the configs/sure-pic32mx/tools directory
    that can be used to solve both issues with the nuttx.ihx file.  But,
    first, you must build the the tools:

      cd configs/sure-pic32mx/tools
      make

    Now you will have an excecutable file call mkpichex (or mkpichex.exe on
    Cygwin).  This program will take the nutt.ihx file as an input, it will
    convert all of the KSEG0 and KSEG1 addresses to physical address, and
    it will write the modified file as nuttx.hex.

    To use this file, you need to do the following things:

      . ./setenv.sh    # Source setenv.sh.  Among other this, this script
                       # will add configs/sure-pic32mx/tools to your
                       # PATH variable
      make             # Build nuttx and nuttx.ihx
      mkpichex $PWD    # Convert nuttx.ihx to nuttx.hex.  $PWD is the path
                       # to the top-level build directory.  It is the only
                       # required input to mkpichex.

PIC32MX Configuration Options
=============================

  General Architecture Settings:

    CONFIG_ARCH - Identifies the arch/ subdirectory.  This should
     be set to:

       CONFIG_ARCH=mips

    CONFIG_ARCH_family - For use in C code:

       CONFIG_ARCH_MIPS=y

    CONFIG_ARCH_architecture - For use in C code:

       CONFIG_ARCH_MIPS32=y

    CONFIG_ARCH_CHIP - Identifies the arch/*/chip subdirectory

       CONFIG_ARCH_CHIP=pic32mx

    CONFIG_ARCH_CHIP_name - For use in C code to identify the exact
       chip:

       CONFIG_ARCH_CHIP_PIC32MX440F512H=y

    CONFIG_ARCH_BOARD - Identifies the configs subdirectory and
       hence, the board that supports the particular chip or SoC.

       CONFIG_ARCH_BOARD=sure-pic32mx

    CONFIG_ARCH_BOARD_name - For use in C code

       CONFIG_ARCH_BOARD_SUREPIC32MX=y

    CONFIG_ARCH_LOOPSPERMSEC - Must be calibrated for correct operation
       of delay loops

    CONFIG_ENDIAN_BIG - define if big endian (default is little
       endian)

    CONFIG_DRAM_SIZE - Describes the installed DRAM (CPU SRAM in this case):

       CONFIG_DRAM_SIZE=(32*1024) (32Kb)

       There is an additional 32Kb of SRAM in AHB SRAM banks 0 and 1.

    CONFIG_DRAM_START - The start address of installed DRAM

       CONFIG_DRAM_START=0x10000000

    CONFIG_DRAM_END - Last address+1 of installed RAM

       CONFIG_DRAM_END=(CONFIG_DRAM_START+CONFIG_DRAM_SIZE)

    CONFIG_ARCH_IRQPRIO - The PIC32MXx supports interrupt prioritization

       CONFIG_ARCH_IRQPRIO=y

    CONFIG_ARCH_LEDS - Use LEDs to show state. Unique to boards that
       have LEDs

    CONFIG_ARCH_INTERRUPTSTACK - This architecture supports an interrupt
       stack. If defined, this symbol is the size of the interrupt
       stack in bytes.  If not defined, the user task stacks will be
       used during interrupt handling.

    CONFIG_ARCH_STACKDUMP - Do stack dumps after assertions

    CONFIG_ARCH_LEDS -  Use LEDs to show state. Unique to board architecture.

    CONFIG_ARCH_CALIBRATION - Enables some build in instrumentation that
       cause a 100 second delay during boot-up.  This 100 second delay
       serves no purpose other than it allows you to calibratre
       CONFIG_ARCH_LOOPSPERMSEC.  You simply use a stop watch to measure
       the 100 second delay then adjust CONFIG_ARCH_LOOPSPERMSEC until
       the delay actually is 100 seconds.

    PIC32MX Configuration

      CONFIG_PIC32MX_MVEC - Select muli- vs. single-vectored interrupts

    Individual subsystems can be enabled:

       CONFIG_PIC32MX_WDT            - Watchdog timer
       CONFIG_PIC32MX_T2             - Timer 2 (Timer 1 is the system time and always enabled)
       CONFIG_PIC32MX_T3             - Timer 3
       CONFIG_PIC32MX_T4             - Timer 4
       CONFIG_PIC32MX_T5             - Timer 5
       CONFIG_PIC32MX_IC1            - Input Capture 1
       CONFIG_PIC32MX_IC2            - Input Capture 2
       CONFIG_PIC32MX_IC3            - Input Capture 3
       CONFIG_PIC32MX_IC4            - Input Capture 4
       CONFIG_PIC32MX_IC5            - Input Capture 5
       CONFIG_PIC32MX_OC1            - Output Compare 1
       CONFIG_PIC32MX_OC2            - Output Compare 2
       CONFIG_PIC32MX_OC3            - Output Compare 3
       CONFIG_PIC32MX_OC4            - Output Compare 4
       CONFIG_PIC32MX_OC5            - Output Compare 5
       CONFIG_PIC32MX_I2C1           - I2C 1
       CONFIG_PIC32MX_I2C2           - I2C 2
       CONFIG_PIC32MX_SPI2           - SPI 2
       CONFIG_PIC32MX_UART1          - UART 1
       CONFIG_PIC32MX_UART2          - UART 2
       CONFIG_PIC32MX_ADC            - ADC 1
       CONFIG_PIC32MX_PMP            - Parallel Master Port
       CONFIG_PIC32MX_CM1            - Comparator 1
       CONFIG_PIC32MX_CM2            - Comparator 2
       CONFIG_PIC32MX_RTCC           - Real-Time Clock and Calendar
       CONFIG_PIC32MX_DMA            - DMA
       CONFIG_PIC32MX_FLASH          - FLASH
       CONFIG_PIC32MX_USBDEV         - USB device
       CONFIG_PIC32MX_USBHOST        - USB host


    PIC32MX Configuration Settings
    DEVCFG0:
      CONFIG_PIC32MX_DEBUGGER - Background Debugger Enable. Default 3 (disabled). The
        value 2 enables.
      CONFIG_PIC32MX_ICESEL - In-Circuit Emulator/Debugger Communication Channel Select
        Default 1 (PG2)
      CONFIG_PIC32MX_PROGFLASHWP  - Program FLASH write protect.  Default 0xff (disabled)
      CONFIG_PIC32MX_BOOTFLASHWP - Default 1 (disabled)
      CONFIG_PIC32MX_CODEWP - Default 1 (disabled)
    DEVCFG1: (All settings determined by selections in board.h)
    DEVCFG2: (All settings determined by selections in board.h)
    DEVCFG3: 
      CONFIG_PIC32MX_USBIDO - USB USBID Selection.  Default 1 if USB enabled
        (USBID pin is controlled by the USB module), but 0 (GPIO) otherwise.
      CONFIG_PIC32MX_VBUSIO - USB VBUSON Selection (Default 1 if USB enabled
        (VBUSON pin is controlled by the USB module, but 0 (GPIO) otherwise.
      CONFIG_PIC32MX_WDENABLE - Enabled watchdog on power up.  Default 0 (watchdog
        can be enabled later by software).

    The priority of interrupts may be specified.  The value ranage of
    priority is 4-31. The default (16) will be used if these any of these
    are undefined.

       CONFIG_PIC32MX_CTPRIO         - Core Timer Interrupt
       CONFIG_PIC32MX_CS0PRIO        - Core Software Interrupt 0
       CONFIG_PIC32MX_CS1PRIO        - Core Software Interrupt 1
       CONFIG_PIC32MX_INT0PRIO       - External Interrupt 0
       CONFIG_PIC32MX_INT1PRIO       - External Interrupt 1
       CONFIG_PIC32MX_INT2PRIO       - External Interrupt 2
       CONFIG_PIC32MX_INT3PRIO       - External Interrupt 3
       CONFIG_PIC32MX_INT4PRIO       - External Interrupt 4
       CONFIG_PIC32MX_FSCMPRIO       - Fail-Safe Clock Monitor
       CONFIG_PIC32MX_T1PRIO         - Timer 1 (System timer) priority
       CONFIG_PIC32MX_T2PRIO         - Timer 2 priority
       CONFIG_PIC32MX_T3PRIO         - Timer 3 priority
       CONFIG_PIC32MX_T4PRIO         - Timer 4 priority
       CONFIG_PIC32MX_T5PRIO         - Timer 5 priority
       CONFIG_PIC32MX_IC1PRIO        - Input Capture 1
       CONFIG_PIC32MX_IC2PRIO        - Input Capture 2
       CONFIG_PIC32MX_IC3PRIO        - Input Capture 3
       CONFIG_PIC32MX_IC4PRIO        - Input Capture 4
       CONFIG_PIC32MX_IC5PRIO        - Input Capture 5
       CONFIG_PIC32MX_OC1PRIO        - Output Compare 1
       CONFIG_PIC32MX_OC2PRIO        - Output Compare 2
       CONFIG_PIC32MX_OC3PRIO        - Output Compare 3
       CONFIG_PIC32MX_OC4PRIO        - Output Compare 4
       CONFIG_PIC32MX_OC5PRIO        - Output Compare 5
       CONFIG_PIC32MX_I2C1PRIO       - I2C 1
       CONFIG_PIC32MX_I2C2PRIO       - I2C 2
       CONFIG_PIC32MX_SPI2PRIO       - SPI 2
       CONFIG_PIC32MX_UART1PRIO      - UART 1
       CONFIG_PIC32MX_UART2PRIO      - UART 2
       CONFIG_PIC32MX_CN             - Input Change Interrupt
       CONFIG_PIC32MX_ADCPRIO        - ADC1 Convert Done
       CONFIG_PIC32MX_PMPPRIO        - Parallel Master Port
       CONFIG_PIC32MX_CM1PRIO        - Comparator 1
       CONFIG_PIC32MX_CM2PRIO        - Comparator 2
       CONFIG_PIC32MX_FSCMPRIO       - Fail-Safe Clock Monitor
       CONFIG_PIC32MX_RTCCPRIO       - Real-Time Clock and Calendar
       CONFIG_PIC32MX_DMA0PRIO       - DMA Channel 0
       CONFIG_PIC32MX_DMA1PRIO       - DMA Channel 1
       CONFIG_PIC32MX_DMA2PRIO       - DMA Channel 2
       CONFIG_PIC32MX_DMA3PRIO       - DMA Channel 3
       CONFIG_PIC32MX_FCEPRIO        - Flash Control Event
       CONFIG_PIC32MX_USBPRIO        - USB

  PIC32MXx specific device driver settings.  NOTE:  For the Sure board,
  UART2 is brought out to the DB9 connector and serves as the serial
  console.

    CONFIG_UARTn_SERIAL_CONSOLE - selects the UARTn for the
       console and ttys0 (default is the UART0).
    CONFIG_UARTn_RXBUFSIZE - Characters are buffered as received.
       This specific the size of the receive buffer
    CONFIG_UARTn_TXBUFSIZE - Characters are buffered before
       being sent.  This specific the size of the transmit buffer
    CONFIG_UARTn_BAUD - The configure BAUD of the UART.  Must be
    CONFIG_UARTn_BITS - The number of bits.  Must be either 7 or 8.
    CONFIG_UARTn_PARTIY - 0=no parity, 1=odd parity, 2=even parity
    CONFIG_UARTn_2STOP - Two stop bits

  PIC32MXx USB Device Configuration

  PIC32MXx USB Host Configuration (the PIC32MX does not support USB Host)

Configurations
==============

Each PIC32MX configuration is maintained in a sudirectory and can be
selected as follow:

    cd tools
    ./configure.sh sure-pic32mx/<subdir>
    cd -
    . ./setenv.sh

Where <subdir> is one of the following:

  ostest:
    This configuration directory, performs a simple OS test using
    apps/examples/ostest.
