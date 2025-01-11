import asyncio
import websockets
import json


async def fetch_data():
    uri = "ws://0.0.0.0:8000"
    async with websockets.connect(uri) as websocket:
        # Request all data
        request_message = json.dumps({"type": "get_destination", "host": "host1"})
        await websocket.send(request_message)

        # Receive response
        response = await websocket.recv()
        all_data = json.loads(response)
        print("Received data:", all_data)


asyncio.run(fetch_data())
