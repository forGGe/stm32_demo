Reposotory contains simple examples, usefull STM32 Peripherial library and core files, 
used for flashing STM32 Discovery board (STM32F407 on board).
Note: library a bit modified to work with STM32F4xx Discovery board. Original stm32 periph lib resides here:
[http://www.st.com/web/en/catalog/tools/PF257901](http://www.st.com/web/en/catalog/tools/PF257901)

Usual commands are following (given that working directory is project directory):

- specify toolchain (gnu-arm-none-eabi e.g.), configure and build
    ```bash
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/gnu-arm-none-eabi.cmake ..
    $ make
    ```

- launch openocd (this may require root privileges):
    ```bash
    $ openocd -f ./misc/stm32f4discovery.cfg
    ```

- flash using telnet (given that openocd is running):
    ```
    $ telnet localhost 4444
    > poll
    > reset halt
    > flash probe 0
    > flash write_image erase build/simple_led.bin 0x08000000
    ```

- or flash using gdb (given that openocd is running):
    ```
    $ arm-none-eabi-gdb -f ./build/mod/simple_led
    (gdb) target remote :3333
    (gdb) monitor reset halt
    (gdb) load		# note: it loads into adress defined in the linker script
    ```

