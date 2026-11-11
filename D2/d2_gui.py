
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import subprocess
import threading
import queue
import os
import time
import re
import filecmp

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CLIENT = os.path.join(BASE_DIR, "udp_client")
SERVER = os.path.join(BASE_DIR, "udp_server")
PORT = "5000"
PACKET_SIZE = 1392


class TransferGUI:

    def __init__(self, root):
        self.root = root
        self.root.title("Reliable File Transfer | D2")
        self.root.geometry("950x680")
        self.root.configure(bg="#101827")

        self.selected_file = ""
        self.running = False
        self.events = queue.Queue()

        self.build_ui()
        self.root.after(100, self.process_events)

    def build_ui(self):

        title = tk.Label(
            self.root,
            text="RELIABLE FILE TRANSFER",
            font=("Arial", 24, "bold"),
            bg="#101827",
            fg="#38bdf8"
        )
        title.pack(pady=(20, 5))

        subtitle = tk.Label(
            self.root,
            text="UDP • Stop-and-Wait ARQ • File Integrity",
            font=("Arial", 11),
            bg="#101827",
            fg="#94a3b8"
        )
        subtitle.pack(pady=(0, 20))

        main = tk.Frame(self.root, bg="#1e293b", padx=25, pady=20)
        main.pack(fill="both", expand=True, padx=30, pady=10)

        tk.Label(
            main, text="SELECT FILE",
            font=("Arial", 12, "bold"),
            bg="#1e293b", fg="white"
        ).pack(anchor="w")

        file_frame = tk.Frame(main, bg="#1e293b")
        file_frame.pack(fill="x", pady=10)

        self.file_label = tk.Label(
            file_frame,
            text="No file selected",
            bg="#334155",
            fg="#cbd5e1",
            anchor="w",
            padx=12,
            pady=10
        )
        self.file_label.pack(side="left", fill="x", expand=True)

        tk.Button(
            file_frame,
            text="Browse",
            command=self.browse_file,
            bg="#0ea5e9",
            fg="white",
            relief="flat",
            padx=20,
            pady=8
        ).pack(side="right", padx=(10, 0))

        tk.Label(
            main, text="DESTINATION",
            font=("Arial", 12, "bold"),
            bg="#1e293b", fg="white"
        ).pack(anchor="w", pady=(15, 5))

        self.destination = tk.StringVar(value="Local Test Receiver")

        self.destination_box = ttk.Combobox(
            main,
            textvariable=self.destination,
            values=["Local Test Receiver"],
            state="readonly",
            width=35
        )
        self.destination_box.pack(anchor="w")

        self.route_label = tk.Label(
            main,
            text="Network route: Local mode (Mininet route not connected)",
            bg="#1e293b",
            fg="#fbbf24",
            font=("Arial", 10)
        )
        self.route_label.pack(anchor="w", pady=15)

        self.start_button = tk.Button(
            main,
            text="START TRANSFER",
            command=self.start_transfer,
            bg="#16a34a",
            fg="white",
            font=("Arial", 12, "bold"),
            relief="flat",
            padx=25,
            pady=12
        )
        self.start_button.pack(anchor="w", pady=5)

        tk.Label(
            main,
            text="TRANSFER PROGRESS",
            bg="#1e293b",
            fg="white",
            font=("Arial", 11, "bold")
        ).pack(anchor="w", pady=(20, 5))

        self.progress = ttk.Progressbar(
            main,
            orient="horizontal",
            length=850,
            mode="determinate"
        )
        self.progress.pack(fill="x")

        self.progress_label = tk.Label(
            main,
            text="0%",
            bg="#1e293b",
            fg="#38bdf8",
            font=("Arial", 11, "bold")
        )
        self.progress_label.pack(anchor="e", pady=5)

        tk.Label(
            main,
            text="LIVE TRANSFER LOGS",
            bg="#1e293b",
            fg="white",
            font=("Arial", 11, "bold")
        ).pack(anchor="w", pady=(15, 5))

        self.logs = tk.Text(
            main,
            height=12,
            bg="#020617",
            fg="#4ade80",
            insertbackground="white",
            font=("Courier", 9),
            relief="flat",
            wrap="word"
        )
        self.logs.pack(fill="both", expand=True)

        self.status = tk.Label(
            self.root,
            text="Status: Waiting for file selection",
            bg="#101827",
            fg="#94a3b8",
            font=("Arial", 10)
        )
        self.status.pack(pady=10)

    def browse_file(self):

        path = filedialog.askopenfilename()

        if path:
            self.selected_file = path
            self.file_label.config(text=os.path.basename(path))
            self.log("File selected. Ready to transfer.")

    def log(self, message):
        self.events.put(("log", message))

    def start_transfer(self):

        if self.running:
            return

        if not self.selected_file:
            messagebox.showwarning(
                "Missing file",
                "Please select a file first."
            )
            return

        if not os.path.isfile(self.selected_file):
            messagebox.showerror(
                "File not found",
                "The selected file no longer exists."
            )
            return

        if not os.path.isfile(CLIENT) or not os.path.isfile(SERVER):
            messagebox.showerror(
                "Missing programs",
                "Compile udp_client and udp_server first."
            )
            return

        # Log selected file details when transfer starts.
        self.log("=" * 45)
        self.log("NEW FILE TRANSFER")
        self.log(f"Source path: {os.path.abspath(self.selected_file)}")
        self.log(f"File name: {os.path.basename(self.selected_file)}")
        self.log(f"File size: {os.path.getsize(self.selected_file)} bytes")
        self.log(f"Destination: {self.destination.get()}")
        self.log("Transfer mode: Local UDP")
        self.log("=" * 45)

        self.running = True
        self.start_button.config(state="disabled")
        self.progress["value"] = 0
        self.progress_label.config(text="0%")
        self.status.config(text="Status: Transfer in progress")

        threading.Thread(
            target=self.transfer_worker,
            daemon=True
        ).start()

    def transfer_worker(self):

        server = None
        client = None
        start_time = time.time()

        selected_file = self.selected_file
        filename = os.path.basename(selected_file)
        output_file = os.path.join(BASE_DIR, "received_" + filename)

        try:
            self.log("Starting receiver...")

            server = subprocess.Popen(
                [SERVER, PORT, output_file],
                cwd=BASE_DIR,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                bufsize=1
            )

            def read_server():
                for line in server.stdout:
                    self.log("[SERVER] " + line.rstrip())

            threading.Thread(
                target=read_server,
                daemon=True
            ).start()

            time.sleep(1)

            if server.poll() is not None:
                raise RuntimeError("UDP server stopped unexpectedly.")

            self.log("Starting sender...")
            self.log(f"Sending selected file: {selected_file}")

            client = subprocess.Popen(
                [CLIENT, "127.0.0.1", PORT, selected_file],
                cwd=BASE_DIR,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                bufsize=1
            )

            file_size = os.path.getsize(selected_file)

            for line in client.stdout:

                line = line.rstrip()
                self.log("[CLIENT] " + line)

                match = re.search(
                    r"Packet\s+(\d+)\s+acknowledged",
                    line,
                    re.IGNORECASE
                )

                if match and file_size > 0:
                    packet_number = int(match.group(1))

                    percentage = min(
                        100,
                        ((packet_number + 1) * PACKET_SIZE / file_size) * 100
                    )

                    self.events.put(("progress", percentage))

            return_code = client.wait(timeout=300)

            if return_code != 0:
                raise RuntimeError("Client reported a transfer failure.")

            server.wait(timeout=10)

            elapsed = time.time() - start_time

            if not os.path.exists(output_file):
                raise RuntimeError("Receiver output file was not created.")

            original_size = os.path.getsize(selected_file)
            received_size = os.path.getsize(output_file)

            if original_size != received_size:
                raise RuntimeError(
                    f"File size mismatch: sent {original_size}, "
                    f"received {received_size}"
                )

            if not filecmp.cmp(
                selected_file,
                output_file,
                shallow=False
            ):
                raise RuntimeError("File integrity verification failed.")

            throughput = (
                received_size / elapsed if elapsed > 0 else 0
            )

            self.events.put(("progress", 100))
            self.log("FILE INTEGRITY VERIFIED!")
            self.log(f"Received file: {output_file}")
            self.log(f"File size: {received_size} bytes")
            self.log(f"Transfer time: {elapsed:.3f} seconds")
            self.log(f"Throughput: {throughput:.2f} bytes/second")
            self.events.put(
                ("status", "SUCCESS: File transferred and verified.")
            )

        except Exception as error:

            self.log("ERROR: " + str(error))
            self.events.put(("status", "Transfer failed."))

        finally:

            if client and client.poll() is None:
                client.kill()

            if server and server.poll() is None:
                server.kill()

            self.events.put(("finished", None))

    def process_events(self):

        while not self.events.empty():

            event, value = self.events.get()

            if event == "log":
                self.logs.insert("end", value + "\n")
                self.logs.see("end")

            elif event == "progress":
                self.progress["value"] = value
                self.progress_label.config(text=f"{value:.1f}%")

            elif event == "status":
                self.status.config(text="Status: " + value)

            elif event == "finished":
                self.running = False
                self.start_button.config(state="normal")

        self.root.after(100, self.process_events)


if __name__ == "__main__":
    root = tk.Tk()
    app = TransferGUI(root)
    root.mainloop()
