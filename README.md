Reposotory contains library and main files used for flashing STM32 Discovery board (STM32F407 on board)
Note: library a bit modified to work with STM32F4xx Discovery board. Original stm32 periph lib resides here:
[http://www.st.com/web/en/catalog/tools/PF257901](http://www.st.com/web/en/catalog/tools/PF257901)

Usual commands are following (given that working directory is project directory):

- configure and build
    ```bash
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    ```

- launch openocd (this may require root privileges):
    ```bash
    $ openocd -f ./stm32f4discovery.cfg
    ```

- flashing using telnet (given that openocd is running):
    ```
    $ telnet localhost 4444
    > poll
    > reset halt
    > flash probe 0
    > flash write_image erase build/demo.bin 0x08000000
    ```

- flashing using gdb (given that openocd is running):
    ```
    arm-none-eabi-gdb -f ./build/demo.elf
    (gdb) target remote :3333
    (gdb) monitor reset halt
    (gdb) load		# note: it loads into memory, not to flash
    ```

