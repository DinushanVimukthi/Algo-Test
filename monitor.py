import psutil
import json
import time

def get_host_resources():
    resources = {
        "CPU_Utilization": psutil.cpu_percent(interval=1),
        "Memory": {
            "Total": psutil.virtual_memory().total,
            "Available": psutil.virtual_memory().available,
            "Used": psutil.virtual_memory().used
        },
        "Disk": {
            "Total": psutil.disk_usage('/').total,
            "Free": psutil.disk_usage('/').free,
            "Used": psutil.disk_usage('/').used
        },
        "Network": {
            "Bytes_Sent": psutil.net_io_counters().bytes_sent,
            "Bytes_Received": psutil.net_io_counters().bytes_recv
        }
    }
    return resources

def log_resources(interval=5):
    while True:
        resources = get_host_resources()
        print(json.dumps(resources, indent=4))
        time.sleep(interval)

if __name__ == "__main__":
    log_resources()
