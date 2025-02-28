# Lab2_exp3_client_Win.py

import socket
import time

HOST = '172.24.235.252'  # Replace with the server's IP address
PORT = 65431

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    serial_number = 1
    while True:
        message = f"{serial_number}. Hello, server!"
        s.sendall(message.encode())
        data = s.recv(1024)
        print(f"Received from server: {data.decode()}")
        serial_number += 1
        time.sleep(2)