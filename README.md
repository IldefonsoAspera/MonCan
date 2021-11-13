# ThreadX CAN bus test

The objectives of this project are the followings:

* Develop this mini project on top of ThreadX and STM32 drivers to gain experience using them
* Implement initial logger implementation that has its own thread and uses minimum execution time on its frontend
* Implement initial UART implementation with ping pong buffers that can transmit them on its own thread
* Test the CAN bus HAL library by interconnecting TX and RX and checking received packets

The project consists on several threads (3) that manage the CAN bus, the logging and the serial port.

The highest priority one inserts packets on the bus. The pins are configured to be internally connected,
so that every time a packet is sent, it is received in the peripheral. The cycle is as follows:
* Packet is inserted in peripheral
* Delay of 10 ms to allow for packet reception
* Check receive fifo and print of contained info
* Delay of 1 s

The next thread, the logger thread, has a lower priority than the CAN thread and processes any data pending in its input queue.
Strings and numbers are extracted and processed, then the resulting strings are inserted in the UART thread buffer.

The lowest priority thread, the UART thread, switches the active buffer (where data is inserted) and performs a blocking 
transmission of all filled bytes of the inactive buffer. The transmission is blocking but as it is the lowest priority thread,
the scheduler will switch the active thread if needed.

The input to the logger thread is a queue, so resuming that thread is automatic.
The input to the serial thread is a mutex and a flag, so resuming that thread is also automatic.