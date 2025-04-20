# Lab 8 – Covert Channel: Flush+Reload Attack

Author: Darasimi Ogunbinu-Peters
Course: CptS 426 – Spring 2025
Assignment: Lab 8 – Cache Timing Covert Channel
Date Completed: April 18, 2025
✅ Overview

This project demonstrates a covert communication channel using the Flush+Reload side-channel attack. Two separate processes (sender and receiver) communicate through timing-based access to a shared memory cache line in /dev/shm/. The sender encodes bits by manipulating the cache state. The receiver decodes these bits by measuring access latency.
📁 Project Files

    sender.c – Sends a character bit-by-bit by either accessing or flushing a shared memory line.

    receiver.c – Measures access latency to infer the bits and reconstructs the message.

    util.cpp / util.hpp – Provides helper functions for timing (rdtscp), memory access, and shared memory mapping.

    Makefile – Builds the sender and receiver executables.

    Daralab8_covert_channel_report.docx – Final report and documentation (submitted separately).

    Lab_8_covert_channel.docx – Original assignment instructions (for reference).

🔧 How to Compile

From the project directory, run:

make clean && make

This compiles both sender and receiver programs. Use make clean to remove previous binaries.
▶️ How to Run (Timing-Sensitive Execution)

Open two WSL terminals in the same project folder:

Terminal 1 – Receiver:

taskset -c 0 ./receiver 8

You will see:

Receiving 8 bits...
Press ENTER to start listening...

Do not press ENTER yet.

Terminal 2 – Sender:

taskset -c 0 ./sender A

You will see:

Sending: A
Press ENTER to begin transmission...

Now press ENTER on both terminals (Receiver first by ~0.2 seconds). Repeat if the sync fails.
🛠 System Configuration

    Cache line size: 64 bytes

    Shared memory: /dev/shm/covert_shared (64 bytes)

    Processor affinity: taskset -c 0 (to bind both to the same core)

    Bit delay: 500ms per bit (usleep(500000))

    Timing threshold: 3000 ns

    Timer: rdtscp()

🧪 Sample Screenshots

    Receiver waiting for sync

    Sender transmitting message

    Debug output of access times

    Final decoded (or partially decoded) message

(All screenshots included in the submitted Word report)
📘 Observations

    Flush+Reload timing reliably distinguishes 1 and 0 when tuned carefully.

    Main challenge: syncing both terminals for consistent transmission.

    Using /dev/shm/ simplified shared memory between processes.

    Taskset improved results by pinning both programs to one CPU core.

📌 Notes

    Total time spent: ~10 hours

    WSL timing jitter made synchronization imperfect, but the design and implementation logic are solid.

    Even with partial success, this lab demonstrated a working side-channel transmission pipeline.