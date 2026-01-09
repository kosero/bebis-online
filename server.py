import asyncio
import json

import websockets

clients = {}
next_id = 1


async def handler(ws):
    global next_id
    cid = next_id
    next_id += 1
    clients[ws] = cid

    await ws.send(json.dumps({"type": "id", "id": cid}))
    print(f"[+] client {cid}")

    try:
        async for msg in ws:
            data = json.loads(msg)
            if data["type"] == "pos":
                for c in clients:
                    await c.send(msg)
    except:
        pass
    finally:
        print(f"[-] client {cid}")
        clients.pop(ws)

        leave_msg = json.dumps({"type": "leave", "id": cid})

        for c in clients:
            await c.send(leave_msg)


async def main():
    async with websockets.serve(handler, "ws.atchannel.top", 8765):
        print("Server started")
        await asyncio.Future()


asyncio.run(main())
