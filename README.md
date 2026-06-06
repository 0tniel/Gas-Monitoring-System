# Mine-Gas-Monitoring-System

A real-time gas monitoring and leak detection system using Arduino Uno, MQ-7, MQ-135, and DHT11 sensors with dashboard-based monitoring and emergency alerts.

## Hardware Used

- Arduino Uno
- MQ-7 Carbon Monoxide Sensor
- MQ-135 Air Quality Sensor
- DHT11 Temperature & Humidity Sensor
- Buzzer
- LEDs
- Breadboard & Jumper Wires

## Features

- Real-Time Gas Monitoring
- Carbon Monoxide Detection
- Air Quality Monitoring
- Temperature & Humidity Monitoring
- Live Streamlit Dashboard
- Audio & Visual Alerts
- Real-Time Data Visualization

## Project Structure

```text
Mine-Gas-Monitoring-System/
├── Dashboard.py
├── mine_gas_monitor.ino
├── Gas Monitoring Hardware.png
├── Gas Monitoring Output.png
└── README.md
```

## Alert Thresholds

### Carbon Monoxide (CO)

| CO Level (ppm) | Status |
|---------------|---------|
| 0 - 10 | Safe |
| 10 - 50 | Low CO Traces |
| 50 - 100 | Warning |
| > 100 | Critical |

### Toxic Gas Index (TGI)

| TGI Value | Status |
|-----------|---------|
| 0 - 20 | Safe |
| 20 - 50 | Mild Toxic Traces |
| 50 - 80 | Warning |
| > 80 | Critical |

## Dashboard Metrics

- 🟤 CO Level (ppm)
- 🟣 Toxic Gas Index (TGI)
- 🌡️ Temperature (°C)
- 💧 Humidity (%)
- 🚨 Safety Status
- 📈 Real-Time Trend Graphs

## Hardware Setup

<p align="center">
  <img src="Gas Monitoring Hardware.png" width="700">
</p>

## Dashboard Output

<p align="center">
  <img src="Gas Monitoring Output.png" width="900">
</p>

## How to Run

### Arduino Setup

1. Connect all sensors to the Arduino Uno.
2. Open `mine_gas_monitor.ino` in Arduino IDE.
3. Select the correct Board and COM Port.
4. Upload the code to Arduino.

### Dashboard Setup

Install dependencies:

```bash
pip install streamlit pyserial pandas
```

Run the dashboard:

```bash
streamlit run Dashboard.py
```

Update the COM port in `Dashboard.py` if required:

```python
ser = serial.Serial('COM3', 9600)
```

Open the local Streamlit URL displayed in the terminal.

## Applications

- Mine Safety Monitoring
- Industrial Gas Detection
- Air Quality Monitoring
- Environmental Monitoring
- Laboratory Safety Systems
