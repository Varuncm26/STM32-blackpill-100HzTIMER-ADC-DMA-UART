## ⚡ STM32 Timer-Triggered ADC (DMA & UART)

This project implements a precise, non-blocking data acquisition system using the **STM32F103 (Blackpill)**. it uses a hardware timer (TIM3) to trigger ADC conversions every 10ms. The results are processed and transmitted via UART using **Direct Memory Access (DMA)**.

### 🛠 System Configuration

| Parameter | Value | Note |
| :--- | :--- | :--- |
| **MCU Frequency** | `72 MHz` | Maximum operating speed |
| **ADC Clock** | `12 MHz` | Prescaler derived from APB2 |
| **Sampling Rate** | `100 Hz` | Triggered every 10ms |
| **Timer Prescaler** | `7199` | Reduces 72MHz to 10kHz |
| **Timer Period** | `99` | 10kHz / 100 ticks = 100Hz |

### 🧮 Timing

To achieve the precise **100Hz** sampling rate, I configured **TIM3** with the following logic:

$$\text{Update Frequency} = \frac{\text{Timer Clock}}{(\text{Prescaler} + 1) \times (\text{Period} + 1)}$$

$$100 \text{ Hz} = \frac{72,000,000}{(7199 + 1) \times (99 + 1)}$$

### 🔋 Voltage Conversion

The ADC on the STM32 has a 12-bit resolution ($2^{12} = 4096$ levels). The raw value is converted to a readable voltage using standard reference logic:

$$\text{Voltage} = \frac{\text{Raw ADC Value} \times 3.3V}{4095}$$

### 🔌 Hardware Setup & Testing

I built a test bench using an **ESP32** to validate the system:

1.  The ESP32 acts as a DAC, generating stepped analog voltages (**0V, 1V, 2V, 3V**) each for 5 seconds in a cycle fed into the STM32's `PA0` pin.
2.  The STM32 reads these values and transmits the result back via UART.
3.  The ESP32 reads the UART response, and the data is verified using **CoolTerm** on the PC.

   ![WhatsApp Image 2025-12-18 at 8 46 34 PM](https://github.com/user-attachments/assets/ea5ae0a5-128d-4c1d-9e4d-13752c237bc2)


https://github.com/user-attachments/assets/39b8b1bd-aa69-433f-aa55-b9c7a7b66483


### 🚀Code Flow
1.  **Timer3** runs in the background and triggers the ADC exactly 100 times per second.
2.  **ADC1** performs the conversion and uses **DMA** to store the raw value directly into a memory buffer .
3.  A **Callback Flag** is set to notify the main loop that new data is ready.
4.  The Main Loop calculates the voltage and formats a string.
5.  **UART DMA** transmits the data asynchronously. A `uart_busy` flag ensures we don't overwrite the buffer while transmission is active.
