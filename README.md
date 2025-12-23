# 📁 Device-Driven Access Control Using OS Event Handling

## 📌 Overview

This project demonstrates **operating-system–level access control** driven by hardware device events.  
File access permissions are dynamically granted or revoked based on the presence of a USB device, using **standard Linux OS mechanisms**.

**No encryption.**  
**No kernel modification.**  
**Pure OS behavior.**

---

## 🎯 Why This Project Exists

Most “security” projects operate at the **application layer**.  
This project intentionally focuses on **OS internals**, showing how:

- The **kernel detects hardware**
- **Events are pushed to user space**
- **File permissions are enforced by the OS**
- **Access control is handled during `open()` system calls**

> The goal is learning OS behavior, not building a commercial security product.

---

## 🧠 Core OS Concepts Demonstrated

- **Kernel vs User Space**  
- **Device detection and drivers**  
- **udev and kernel-generated events**  
- **Netlink sockets**  
- **UNIX file permissions & VFS**  
- **Permission checks during `open()`**

---

## 🧩 How It Works (High-Level Flow)

1. A **file is selected at runtime** to be protected.
2. The program immediately **revokes access** to the file (`chmod 000`).
3. A **background listener** subscribes to kernel device events.
4. When a **USB device is inserted**:
   - Kernel generates a **uevent**
   - User-space listener receives it
   - File permissions are **restored** (`chmod 600`)
5. When the **USB is removed**:
   - Permissions are **revoked again**
6. The **OS enforces access naturally**, without any hacks.

---

## 🔐 Important OS Behavior

- File permissions are **checked only at open()**
- If a file is **already open**, removing USB does **not close it**
- This is **standard UNIX behavior**, not a bug

---

## 🖥️ Environment

- **OS:** Kali Linux (VirtualBox)  
- **Language:** C  
- **Kernel Interface:** Netlink (`KOBJECT_UEVENT`)

---

## 🚀 How to Run

1. Compile:
   ```bash
   gcc project.c -o project
   Run (requires root for device events):
2. Run (requires root for device events):
   sudo ./project


3. Enable USB in VirtualBox:

    Enable USB Controller

    Add USB filter for flash drive

    Insert/remove USB while program is running

## 🔧 Useful Commands for Debugging

##  List block devices:

    lsblk


##  Monitor kernel and udev events:

    udevadm monitor --kernel --udev


##  Get full file path:

    realpath <filename>

##  ⚠️ What This Project Does NOT Do

    Does not read USB data

    Does not encrypt files

    Does not modify kernel

    Does not bypass OS security

##  This project observes and reacts to OS behavior — it does not override it.

##  📚 Educational Use

    This project was built for learning and demonstrating Operating System concepts and should be evaluated as such.

##  🧠 Final Notes

    This project reflects real OS design decisions and is intentionally minimal to keep behavior transparent and explainable.
