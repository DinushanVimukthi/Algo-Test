import asyncio
import websockets
import json

# Store the latest data from each host
host_data_map = {}


def get_host_data():
    return host_data_map


# Perform some action before sending host data (this can be customized as needed)
async def perform_action_on_host(host_id):
    print(f"Performing action on host {host_id}")
    # Example of action (you can modify this based on your requirements)
    await asyncio.sleep(2)  # Simulating some action like processing, network call, etc.
    print(f"Action on host {host_id} completed")


async def handle_connection(websocket, path):
    global host_data_map
    try:
        async for message in websocket:
            # Parse the received message
            data = json.loads(message)
            host_id = data.get("host")  # Unique identifier for the host

            if data.get("type") == "update_data":
                # Update the map with the latest data
                host_data_map[host_id] = data
                print(f"Updated data for host {host_id}: {data}")

            elif data.get("type") == "get_destination":
                # Perform action on the specific host (if necessary)
                await perform_action_on_host(host_id)

                # Send the requested host data
                if host_id in host_data_map:
                    await websocket.send(json.dumps(host_data_map[host_id]))
                else:
                    await websocket.send(json.dumps({"error": "Host data not found"}))

            # Example: Respond to a "request_all_data" for all data
            elif data.get("type") == "request_all_data":
                print("Sending all data to client")
                await websocket.send(json.dumps(host_data_map))

    except websockets.ConnectionClosed:
        print("A client disconnected.")
    finally:
        pass  # Optionally handle cleanup if needed


# The entry point to start the server
async def start_server():
    server = await websockets.serve(handle_connection, "0.0.0.0", 8000)
    print("WebSocket server started on ws://0.0.0.0:8000")
    await server.wait_closed()  # Keep the server running


if __name__ == "__main__":
    # Start the server using asyncio.run()
    asyncio.run(start_server())
