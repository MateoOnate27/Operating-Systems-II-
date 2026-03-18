# Workshop 4: RTOS on a Raspberry Pi 4

This repository contains the source code and experimental results for Workshop 4 of the Operating Systems II course. The project focuses on measuring and comparing the latency and jitter between a standard Linux kernel and a Real-Time (RT) kernel using the `PREEMPT_RT` patch.

## System Requirements
- **Hardware:** Raspberry Pi 4 Model B (64-bit).
- **Operating System:** Raspberry Pi OS Lite (64-bit).
- **Required Tools:** `build-essential`, `stress`, `rt-tests`, `sysstat`.

## Repository Structure
- `rt_task.c`: A periodic real-time task that measures jitter (deviation from a 1ms period).
- `rt_multi.c`: An advanced multi-threaded program using the POSIX RT API with different priorities and periods.


# Mid-term Exam: Dynamic System Information Driver

This project implements a **Linux Kernel Module (LKM)** designed for the Raspberry Pi 4. It creates a character device at `/dev/sysinfo` that allows users to retrieve real-time system metadata through a dynamic command interface.

## 🚀 Features
The driver supports bidirectional communication, allowing the user to set the output mode by writing specific strings to the device.

| Command | Description |
| :--- | :--- |
| `all` | Displays Kernel release, CPU count, and system jiffies (Default). |
| `kernel` | Displays only the Kernel release version. |
| `cpu` | Displays the number of online CPU cores. |
| `time` | Displays the current system jiffies. |

## 🛠 Prerequisites
Before compiling, ensure your system has the necessary kernel headers and build tools:
```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
