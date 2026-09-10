from __future__ import annotations

import json
import threading
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

ROOT = Path(__file__).parent
HOST = "127.0.0.1"
PORT = 8765

state = {
    "firmware": None,
    "format": None,
    "size": 0,
    "screen": "boot",
    "battery": 84,
    "serial": ["Chatter emulator ready", "Load an ELF or BIN to begin"],
    "messages": [],
    "buttons": [],
}


def load_firmware(payload: dict) -> dict:
    name = str(payload.get("name", "firmware"))
    raw = payload.get("data", "")
    data = bytes.fromhex(raw)
    suffix = Path(name).suffix.lower()
    fmt = "ELF" if suffix == ".elf" and data[:4] == b"\x7fELF" else "BIN"
    state["firmware"] = name
    state["format"] = fmt
    state["size"] = len(data)
    state["screen"] = "home"
    state["serial"] = [
        f"Loaded {name}",
        f"Image format: {fmt} ({len(data):,} bytes)",
        "Boot complete (behavioral emulator)",
    ]
    return snapshot()


def snapshot() -> dict:
    return dict(state)


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *_args):
        pass

    def _send(self, status: int, content_type: str, body: bytes):
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Cache-Control", "no-store")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        path = urlparse(self.path).path
        if path == "/":
            self._send(200, "text/html; charset=utf-8", (ROOT / "software-emulator.html").read_bytes())
        elif path == "/state":
            self._send(200, "application/json", json.dumps(snapshot()).encode())
        else:
            self._send(404, "text/plain", b"Not found")

    def do_POST(self):
        length = int(self.headers.get("Content-Length", "0"))
        payload = json.loads(self.rfile.read(length) or b"{}")
        action = payload.get("action")

        if action == "load":
            result = load_firmware(payload)
        elif action == "button":
            button = str(payload.get("button", "A"))
            state["buttons"].append(button)
            state["serial"].append(f"Input: {button}")
            if button in {"A", "ENTER"}:
                state["screen"] = "messages"
            elif button in {"B", "BACK"}:
                state["screen"] = "home"
            result = snapshot()
        elif action == "battery":
            state["battery"] = max(0, min(100, int(payload.get("value", 84))))
            state["serial"].append(f"Battery set to {state['battery']}%")
            result = snapshot()
        elif action == "message":
            text = str(payload.get("text", "")).strip()
            if text:
                state["messages"].append({"text": text, "incoming": True})
                state["screen"] = "messages"
                state["serial"].append(f"Incoming message: {text}")
            result = snapshot()
        else:
            result = snapshot()

        self._send(200, "application/json", json.dumps(result).encode())


if __name__ == "__main__":
    server = ThreadingHTTPServer((HOST, PORT), Handler)
    url = f"http://{HOST}:{PORT}/"
    print(f"Chatter software emulator: {url}")
    threading.Timer(0.4, lambda: webbrowser.open(url)).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
