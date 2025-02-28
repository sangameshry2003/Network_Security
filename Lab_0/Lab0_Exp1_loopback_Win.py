import socket
import time

def send_packets():
    # Create a socket object
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP socket
    server_address = ('127.0.0.1', 12345)  # Loopback address with port 12345

    try:
        while True:
            # Message to send
            message = "Hello, this is a test UDP packet on Loopback i/f on Windows!"
            
            # Send the message
            print(f"Sending: {message}")
            sock.sendto(message.encode('utf-8'), server_address)
            
            # Wait 1 second before sending the next packet
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nStopped sending packets.")
    finally:
        sock.close()

if __name__ == "__main__":
    send_packets()
