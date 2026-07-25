.. zephyr:board:: s32k3x8evb

Overview
********

The S32K3X8EVB-Q289 is an evaluation board for the NXP S32K358 general-purpose
automotive MCU (Arm Cortex-M7 lockstep pair plus one independent Cortex-M7,
8 MiB flash, 768 KiB system SRAM, 289 MAPBGA). The board features an FS26
power SBC, an on-board debug interface (OpenSDA), a dedicated USB-to-UART/I2C
bridge (MCP2221A), Arduino-footprint expansion headers, two user RGB LEDs,
two push buttons, potentiometers, a touch pad, an SD card slot and
CAN/LIN/Ethernet interfaces.

Zephyr runs on the lockstep Cortex-M7 core (CM7_0).

Hardware
********

- NXP S32K358

  - Arm Cortex-M7 lockstep core at up to 240 MHz, plus one independent
    Cortex-M7 core (not used by Zephyr)
  - 8 MiB code flash (last 48 KiB reserved by the Secure BAF), 128 KiB data
    flash, 768 KiB system SRAM (SRAM0..2), 64 KiB ITCM and 128 KiB DTCM per
    core pair

- FS26 power system basis chip. In the default board configuration the FS26
  starts in debug mode, so the firmware does not need to service the FS26
  watchdog.
- On-board OpenSDA debug interface (micro-USB), 20-pin Cortex Debug and JTAG
  connectors for an external debugger
- MCP2221A USB-to-UART/I2C bridge on a separate USB connector

Supported Features
******************

.. zephyr:board-supported-hw::

Connections and IOs
*******************

Pin assignments follow the S32K3X8EVB-Q289 Hardware User Manual (rev. C):

.. list-table:: Default Zephyr peripherals
   :header-rows: 1

   * - Function
     - MCU pin
     - Usage
   * - LPUART13 TX
     - PTC26
     - Console output to the MCP2221A USB-to-UART bridge (default
       zero-ohm configuration), 115200 8N1
   * - LPUART13 RX
     - PTC27
     - Console input from the MCP2221A bridge
   * - PTG29 / PTG30 / PTG31
     - GPIO output
     - User RGB LED D32 red/green/blue (``led0``/``led1``/``led2``),
       active high
   * - PTF21 / PTF22 / PTF23
     - GPIO output
     - User RGB LED D33 red/green/blue, active high
   * - PTH1
     - GPIO input, EIRQ17
     - User push button SW4 (``sw0``), active high
   * - PTH3
     - GPIO input, EIRQ19
     - User push button SW5 (``sw1``), active high

The console is reached through the board's USB-to-UART/I2C USB connector
(MCP2221A); no wiring is required in the default configuration. As an
alternative, LPUART6 (PTA15/PTA16) is routed to the OpenSDA virtual COM port
and can be selected by moving the corresponding zero-ohm resistors (see the
hardware user manual, "USB to I2C/UART Interface").

Programming and Debugging
*************************

.. zephyr:board-supported-runners::

Applications for the ``s32k3x8evb/s32k358`` board can be built in the usual
way (see :ref:`build_an_application`). Flashing and debugging use an external
SEGGER J-Link probe attached to one of the JTAG/Cortex Debug connectors.

.. warning::
   Every J-Link operation on S32K3 devices (flash, reset, attach) must pass
   SEGGER's ``S32K3xx_NoRAMInit.JLinkScript``. Without it, the J-Link ECC RAM
   initialization runs on the live core and corrupts the context of a running
   thread, crashing the firmware seconds to minutes later (crash signature:
   ``PC = 0x20000000``, ``r2 = 0xdeadbeef``). Download the script from the
   `SEGGER wiki for NXP S32K3xx <https://kb.segger.com/NXP_S32K3xx>`_ and pass
   it on every invocation:

   .. code-block:: console

      west flash --runner jlink \
        --tool-opt="-JLinkScriptFile /path/to/S32K3xx_NoRAMInit.JLinkScript"

Here is an example for the :zephyr:code-sample:`hello_world` application:

.. code-block:: console

   west build -b s32k3x8evb/s32k358 samples/hello_world
   west flash --runner jlink \
     --tool-opt="-JLinkScriptFile /path/to/S32K3xx_NoRAMInit.JLinkScript"

Open a serial terminal at 115200 8N1 on the MCP2221A virtual COM port; after
reset the board shows the Zephyr banner:

.. code-block:: console

   *** Booting Zephyr OS build ... ***
   Hello World! s32k3x8evb/s32k358

References
**********

- `S32K3X8EVB-Q289 product page
  <https://www.nxp.com/design/design-center/development-boards-and-designs/automotive-development-platforms/s32k-mcu-platforms/s32k3x8evb-q289-evaluation-board-for-automotive-general-purpose:S32K3X8EVB-Q289>`_
- S32K3X8EVB-Q289 Hardware User Manual, rev. C (NXP, registration required)
- `S32K3 MCU family <https://www.nxp.com/products/S32K3>`_
