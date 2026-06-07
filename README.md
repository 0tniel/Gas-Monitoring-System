# Mine-Gas-Monitoring-System

A real-time gas monitoring and leak detection system using Arduino Uno, MQ-7, MQ-135, and DHT11 sensors with a Streamlit-based dashboard for live monitoring, visualization, and emergency alerts.

## Hardware Used

- Arduino Uno
- MQ-7 Carbon Monoxide Sensor
- MQ-135 Air Quality Sensor
- DHT11 Temperature & Humidity Sensor
- Buzzer
- LED Indicator
- Breadboard
- Jumper Wires

## Software Used

- Arduino IDE
- Python
- Streamlit
- PySerial
- Pandas

## Features

- Real-Time Gas Monitoring
- Carbon Monoxide Detection
- Toxic Gas Detection
- Temperature & Humidity Monitoring
- Live Dashboard Visualization
- Audio & Visual Alerts
- Serial Communication Interface
- Real-Time Trend Graphs

## Project Structure

```text
Mine-Gas-Monitoring-System/
├── Mine_Gas_Monitoring_System.ino
├── Dashboard.py
├── Gas Monitoring Hardware.png
├── Gas Monitoring Output.png
├── requirements.txt
└── README.md
```

## Dashboard Metrics

- CO Level (ppm)
- Toxic Gas Index (TGI)
- Temperature (°C)
- Humidity (%)
- Safety Status
- Real-Time Trend Graphs

## Alert Conditions

### Carbon Monoxide (CO)

| CO Level | Status |
|-----------|---------|
| < 30 ppm | Safe |
| ≥ 30 ppm | Alert |

### Toxic Gas Index (TGI)

| TGI Value | Status |
|------------|---------|
| < 60 | Safe |
| ≥ 60 | Alert |

When either threshold is exceeded:

- Buzzer turns ON
- LED turns ON
- Warning message appears on dashboard

## Hardware Setup

<p align="center">
  <img src="Gas Monitoring Hardware.png" width="700">
</p>

## Dashboard Output

<p align="center">
  <img src="Gas Monitoring Output.png" width="900">
</p>

## How It Works

1. MQ-7 measures Carbon Monoxide concentration.
2. MQ-135 measures air quality and toxic gas levels.
3. DHT11 measures temperature and humidity.
4. Arduino processes sensor readings and sends data over Serial communication.
5. Streamlit dashboard receives and visualizes the data.
6. If gas levels exceed thresholds, alerts are triggered through the buzzer, LED, and dashboard.

## How to Run

### Arduino Setup

1. Connect the sensors to Arduino Uno:
   - MQ-7 → A0
   - MQ-135 → A1
   - DHT11 → A3
   - Buzzer → D8
   - LED → D9

2. Open `Mine_Gas_Monitoring_System.ino` in Arduino IDE.

3. Select:
   - Board: Arduino Uno
   - Correct COM Port

4. Upload the code.

### Dashboard Setup

Install required packages:

```bash
pip install -r requirements.txt
```

Or install manually:

```bash
pip install streamlit pyserial pandas
```

Run the dashboard:

```bash
streamlit run Dashboard.py
```

Update the COM port in `Dashboard.py` if necessary:

```python
ser = serial.Serial('COM3', 9600)
```

Open the Streamlit URL displayed in the terminal.

## Requirements

Create a file named `requirements.txt`:

```text
streamlit
pyserial
pandas
```

## Applications

- Mine Safety Monitoring
- Industrial Gas Detection
- Environmental Monitoring
- Air Quality Monitoring
- Laboratory Safety Systems
- Hazardous Gas Detection

## Output

The system provides:

- Real-Time Sensor Readings
- Live Environmental Monitoring
- Gas Leak Alerts
- Dashboard Visualization
- Historical Trend Analysis
