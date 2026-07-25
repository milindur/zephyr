.. zephyr:board:: s32k3x8evb

Overview
********

The S32K3X8EVB-Q289 is an evaluation board for the NXP S32K358 general-purpose
automotive MCU (Arm Cortex-M7 lockstep pair plus one independent Cortex-M7,
8 MiB flash, 768 KiB system SRAM, 289 MAPBGA). The board features an FS26
power SBC, an on-board debug interface, Arduino-footprint expansion headers,
user RGB LEDs, push buttons, potentiometers, a touch pad, an SD card slot and
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

- FS26 power system basis chip
- On-board debug interface on the J55 micro-USB connector, plus JTAG
  connectors for an external debugger

Supported Features
******************

.. zephyr:board-supported-hw::

Connections and IOs
*******************

.. note::
   The pin assignments below follow the NXP S32K358 RTD example projects and
   are pending verification against the S32K3X8EVB-Q289 hardware user manual
   and schematic (available from NXP with a registered account). Verify the
   console, LED and button wiring against the board manual before relying on
   them.

.. list-table:: Default Zephyr peripherals
   :header-rows: 1

   * - Function
     - MCU pin
     - Usage
   * - LPUART3 TX
     - PTD2
     - Console output, 115200 8N1
   * - LPUART3 RX
     - PTD3
     - Console input
   * - PTF21
     - GPIO output
     - User LED (``led0``)
   * - PTB17
     - GPIO input, EIRQ31
     - User button (``sw0``), external interrupt capable

The console is not hard-wired: connect a 3.3 V USB-UART adapter (or the
on-board debug interface's virtual COM port, if routed) to the LPUART3 pins.

Programming and Debugging
*************************

.. zephyr:board-supported-runners::

Applications for the ``s32k3x8evb/s32k358`` board can be built in the usual
way (see :ref:`build_an_application`). Flashing and debugging use an external
SEGGER J-Link probe attached to one of the JTAG connectors.

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

Open a serial terminal at 115200 8N1 on the console UART; after reset the
board shows the Zephyr banner:

.. code-block:: console

   *** Booting Zephyr OS build ... ***
   Hello World! s32k3x8evb/s32k358

References
**********

- `S32K3X8EVB-Q289 product page
  <https://www.nxp.com/design/design-center/development-boards-and-designs/automotive-development-platforms/s32k-mcu-platforms/s32k3x8evb-q289-evaluation-board-for-automotive-general-purpose:S32K3X8EVB-Q289>`_
- `S32K3 MCU family <https://www.nxp.com/products/S32K3>`_
