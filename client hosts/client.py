import psutil
import requests
import time

# Central server URL
CENTRAL_SERVER_URL = "http://127.0.0.1:5000/report"

def collect_metrics():
    return {
        "hostname": psutil.users()[0].name,  # Replace with actual hostname retrieval
        "CPU_Utilization": psutil.cpu_percent(interval=1),
        "Memory": {
            "Total": psutil.virtual_memory().total,
            "Used": psutil.virtual_memory().used,
        },
        "Disk": {
            "Total": psutil.disk_usage('/').total,
            "Used": psutil.disk_usage('/').used,
        },
        "Network": {
            "Bytes_Sent": psutil.net_io_counters().bytes_sent,
            "Bytes_Received": psutil.net_io_counters().bytes_recv,
        }
    }

def send_metrics():
    while True:
        metrics = collect_metrics()
        try:
            response = requests.post(CENTRAL_SERVER_URL, json=metrics)
            if response.status_code == 200:
                print("Metrics sent successfully!")
            else:
                print(f"Failed to send metrics: {response.status_code}")
        except Exception as e:
            print(f"Error sending metrics: {e}")
        time.sleep(10)  # Send metrics every 10 seconds

if __name__ == "__main__":
    send_metrics()
