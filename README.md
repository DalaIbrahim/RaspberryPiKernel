## 🧠 Overview  
This project is a minimal operating system kernel built from scratch for the Raspberry Pi using C and ARM Assembly. It was developed as part of our university Operating Systems course (CMPS 240) and runs on bare metal—without any pre-existing OS layer. The kernel supports UART communication, basic memory management, a command-line interface (CLI), dynamic memory allocation, and a linked list implementation. All development and testing were performed in a Linux virtual machine using QEMU for ARM emulation.

---

## ⚙️ Features

- **UART Communication:** Low-level serial I/O via `uart_init`, `uart_putc`, and `custom_printf`.
- **Memory Management:**
  - Page-based allocation using 4KB pages.
  - Dynamic heap allocator via custom `kmalloc` and `kfree`.
- **Command-Line Interface:** Minimal shell-like loop for receiving and executing commands.
- **Custom Utility Functions:** Includes `strcmp`, `atoi`, `strchr`, and `printf` equivalents.
- **Linked List:** Fully dynamic linked list with node creation, traversal, and cleanup.
- **Encryption Module:** Caesar cipher implementation for simple text encryption/decryption.

---

## 🛠️ Tools & Technologies

- **C** and **ARM Assembly**
- **Cross-Compilation Toolchain:** `gcc-arm-none-eabi`
- **Emulator:** [QEMU](https://www.qemu.org/) for ARM hardware emulation
- **Build System:** `Makefile`
- **Development Environment:** Linux-based Virtual Machine (Ubuntu)

---

## 📂 Project Structure
├── boot.S # Kernel bootloader (ARM assembly)
├── kernel.c # Main kernel logic
├── uart.c / uart.h # UART communication functions
├── memory.c / memory.h # Memory paging and heap allocator
├── linkedlist.c / .h # Linked list implementation
├── utils.c / utils.h # Custom string & I/O utilities
├── Makefile # Build instructions
└── README.md # Project documentation

## 🚀 Getting Started

### Prerequisites
- Linux environment (e.g., Ubuntu VM)
- `gcc-arm-none-eabi` installed
- `qemu-system-arm` installed

### Build & Run
```bash
# Compile
make

# Run in QEMU
qemu-system-arm -M versatilepb -m 128M -nographic -kernel kernel8.img
