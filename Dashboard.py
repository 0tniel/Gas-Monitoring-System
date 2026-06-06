import streamlit as st
import serial
import time
import re
from collections import deque
import pandas as pd
from datetime import datetime

# --- Serial Setup ---
try:
    ser = serial.Serial('COM3', 9600)
    time.sleep(2)
except serial.SerialException as e:
    st.error(f"❌ Could not open serial port: {e}")
    st.stop()

# --- Streamlit Setup ---
st.set_page_config(page_title="⛏️ Mine Gas Dashboard", layout="wide")
st.title("⛏️ Real-time Mine Gas Monitoring Dashboard")

# --- Data Storage ---
max_points = 50
co_data = deque(maxlen=max_points)
tgi_data = deque(maxlen=max_points)
timestamps = deque(maxlen=max_points)

# --- Layout: Top 4 metrics ---
col1, col2, col3, col4 = st.columns(4)
co_metric = col1.metric("🟤 CO Level (ppm)", "—")
tgi_metric = col2.metric("🟣 Toxic Gas Index (TGI)", "—")
temp_metric = col3.metric("🌡️ Temp (°C)", "—")
hum_metric = col4.metric("💧 Humidity (%)", "—")

# --- Chart and Status Box ---
chart_placeholder = st.empty()
status_box = st.empty()
raw_expander = st.expander("📋 Raw Sensor Output", expanded=False)

# --- Reverse-ordered Raw Log ---
if "log_history" not in st.session_state:
    st.session_state.log_history = []

# --- Helper: Parse Sensor Data Block ---
def parse_block(lines):
    block_text = "\n".join(lines)

    co_match = re.search(r"CO ppm = ([\d.]+)", block_text)
    tgi_match = re.search(r"TGI: ([\d.]+)", block_text)
    temp_match = re.search(r"Temp \(°C\): ([\d.]+)", block_text)
    hum_match = re.search(r"Humidity \(%\): ([\d.]+)", block_text)

    data = {}
    if co_match:
        data["CO"] = float(co_match.group(1))
    if tgi_match:
        data["TGI"] = float(tgi_match.group(1))
    if temp_match:
        data["Temp"] = float(temp_match.group(1))
    if hum_match:
        data["Humidity"] = float(hum_match.group(1))

    return data, block_text

# --- Serial Data Buffer ---
buffer = []

# --- Main Loop ---
while True:
    try:
        line = ser.readline().decode('utf-8').strip()
        if line:
            buffer.append(line)

            if "TGI:" in line:
                data, block_text = parse_block(buffer)

                if "CO" in data and "TGI" in data:
                    co = data["CO"]
                    tgi = data["TGI"]

                    # Update Metrics
                    co_metric.metric("🟤 CO Level (ppm)", f"{co:.2f}")
                    tgi_metric.metric("🟣 Toxic Gas Index (TGI)", f"{tgi:.1f}")
                    if "Temp" in data:
                        temp_metric.metric("🌡️ Temp (°C)", f"{data['Temp']:.1f}")
                    if "Humidity" in data:
                        hum_metric.metric("💧 Humidity (%)", f"{data['Humidity']:.1f}")

                    # Append to chart data
                    co_data.append(co)
                    tgi_data.append(tgi)
                    timestamps.append(time.strftime('%H:%M:%S'))

                    # Line Chart
                    df = pd.DataFrame({
                        "Time": list(timestamps),
                        "CO (ppm)": list(co_data),
                        "TGI": list(tgi_data)
                    }).set_index("Time")

                    chart_placeholder.line_chart(df, use_container_width=True)

                    # --- Status Analysis ---
                    status_messages = []

                    # CO Thresholds
                    if co > 100:
                        status_messages.append("🚨 Dangerous CO levels! (>100 ppm)")
                    elif co > 50:
                        status_messages.append("⚠️ Elevated CO detected! (>50 ppm)")
                    elif co > 10:
                        status_messages.append("🟡 Low CO traces.")

                    # TGI Thresholds
                    if tgi > 80:
                        status_messages.append("🚨 Severe toxic gas levels! (TGI > 80)")
                    elif tgi > 50:
                        status_messages.append("⚠️ High ammonia/NOx levels! (TGI > 50)")
                    elif tgi > 20:
                        status_messages.append("🟡 Mild toxic traces.")

                    # Show status
                    if status_messages:
                        status_box.warning("\n".join(status_messages))
                    else:
                        status_box.success("✅ Air is clean and safe.")

                    # --- Update Raw Log View ---
                    st.session_state.log_history.insert(0, block_text)
                    with raw_expander:
                        st.code("\n\n".join(st.session_state.log_history))

                buffer.clear()

        time.sleep(0.1)

    except Exception as e:
        st.error(f"🔥 Error: {e}")
        break

explain this entire code
