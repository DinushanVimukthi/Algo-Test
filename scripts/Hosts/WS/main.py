from socket import AF_INET, AF_INET6

import psutil  # For system metrics
import asyncio
import websockets
import json
import time


def get_ethernet_ip():
    network_data = {}

    for interface, addrs in psutil.net_if_addrs().items():
        # Check if the interface family contains "AF_INET" and AF_INET6 then get network information netmask, broadcast address, and IP address
        for addr in addrs:
            if addr.family == AF_INET:
                network_data[interface] = {
                    "netmask": addr.netmask,
                    "broadcast": addr.broadcast,
                    "ip": addr.address,
                }
            if addr.family == AF_INET6:
                network_data[interface] = {
                    "netmask": addr.netmask,
                    "broadcast": addr.broadcast,
                    "ip": addr.address,
                }
    return network_data


async def send_update_data(websocket, host_id, cpu, memory):
    # Prepare the data to send (update data for a specific host)
    data = {
        "type": "update_data",
        "host": host_id,
        "cpu": cpu,
        "memory": memory
    }
    await websocket.send(json.dumps(data))
    print(f"Sent update data for {host_id}: {data}")

async def send_metrics():
    uri = "ws://0.0.0.0:8000"
    try:
        async with websockets.connect(uri) as websocket:
            while True:
                print(psutil.net_if_addrs())

                metrics = {
                    "type": "update_data",
                    "memory": {
                        "total": psutil.virtual_memory().total,
                        "free": psutil.virtual_memory().free,
                        "used": psutil.virtual_memory().used,
                    },
                    "disk": {
                        "total": psutil.disk_usage("/").total,
                        "free": psutil.disk_usage("/").free,
                        "used": psutil.disk_usage("/").used,
                    },
                    "network": {
                        "total": psutil.net_io_counters().bytes_sent + psutil.net_io_counters().bytes_recv,
                        "used": psutil.net_io_counters().bytes_sent,
                        "free": psutil.net_io_counters().bytes_recv,
                    },
                    "cpu": {
                        "total": psutil.cpu_percent(),
                        "used": psutil.cpu_percent(),
                        "free": 100 - psutil.cpu_percent(),
                    },
                    "units": {
                        "memory": "bytes",
                        "disk": "bytes",
                        "network": "bytes",
                        "cpu": "percent",
                    },
                    # Get the hostname from the system
                    "host": "host" + str(psutil.cpu_percent()),
                    #     Get network address
                    "ip": get_ethernet_ip(),
                }
                # Send metrics to the server
                await websocket.send(json.dumps(metrics))
                print(f"Sent: {metrics}")
                time.sleep(5)  # Adjust reporting interval as needed
    except websockets.ConnectionClosed:
        print("Server disconnected. Reconnecting...")
    except ConnectionRefusedError:
        print("Connection refused. Retrying...")
    finally:
        pass


#  run send_metrics function when pass the argument send_metrics

asyncio.run(send_metrics())
