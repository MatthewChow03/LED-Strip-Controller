# LED Strip Controller

Most coloured LED strips are controlled via a remote or bluetooth app. I created firmware to control the LEDs from my computer using an IR transceiver circuit powered by an **Arudino** microcontroller. The firmware was written in **C++** with an optional **PowerShell** script to control the program from the terminal.

https://user-images.githubusercontent.com/74833839/148328788-9cb8448d-6898-4720-9720-cc439e223258.mp4

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#features">Features</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#schematic">Schematic</a></li>
        <li><a href="#dependencies">Dependencies</a></li>
        <li><a href="#how-to-run-the-program">How to run the program</a></li>
      </ul>
    </li>
  </ol>
</details>

## Features

- Control IR based LED strips (NEC protocol) using the Arudino IDE serial console or terminal
- Monitor data being sent over IR using the Arudino IDE serial console or terminal
- Help menu listing available commands (terminal only)

## Getting Started

The program can be run from the Arduino IDE. Once the firmware has been flashed to the microcontroller, the program can also be run from the terminal (with Windows PowerShell).

### Prerequisites

- Arduino IDE: [version 2 download](https://www.arduino.cc/en/software)
- (Optional) Windows PowerShell: [install on Windows, Linus, and macOS](https://docs.microsoft.com/en-us/powershell/scripting/install/installing-powershell?view=powershell-7.2)

### Schematic

*(Coming soon)*

### Dependencies
The [IRremote.h](https://github.com/Arduino-IRremote/Arduino-IRremote) library must be installed in your directory or through the [Arudino IDE Library Manager](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries).

### How to run the program

#### Arduino IDE

- Flash the code to the microcontroller
- Open the serial monitor under `Tools > Serial Monitor`
- Use valid commands found on line 37 of [`LED-Strip-Controller.ino`](https://github.com/MatthewChow03/LED-Strip-Controller/blob/main/LED-Strip-Controller.ino)

#### PowerShell Script

- Flash the code to the microcontroller
- Open Windows PowerShell and change into the project directory:
```
cd "C:\Users\name\repo"
```
- Run the script:

```
.\script.ps1
```
- Note, the script can be run from other CLIs such as command prompt:
```
powershell "& ""C:\my_path\script.ps1"""
```
