import socket
import time

def udp_send_and_receive():
    # Define loopback address and port
    loopback_address = "127.0.0.1"
    port = 12346  # Port to send and receive

    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to the loopback address and port to listen for incoming packets
    udp_socket.bind((loopback_address, port))

    try:
        while True:
            # Send an initial test packet
            message = "Initial test UDP packet from loopback"
            print(f"Sending: {message}")
            udp_socket.sendto(message.encode("utf-8"), (loopback_address, port))

            # Wait to receive a packet
            print("Waiting to receive a packet...")
            data, addr = udp_socket.recvfrom(1024)  # Buffer size is 1024 bytes

            # Modify the received packet
            received_message = data.decode("utf-8")
            print(f"Received from {addr}: {received_message}")
            modified_message = f"Modified: {received_message.upper()}"
            print(f"Sending back modified packet: {modified_message}")

            # Send back the modified packet
            udp_socket.sendto(modified_message.encode("utf-8"), addr)

            # Sleep for 1 second before sending the next packet
            time.sleep(1)

    except KeyboardInterrupt:
        print("\nStopped sending and receiving packets.")
    finally:
        udp_socket.close()

if __name__ == "__main__":
    udp_send_and_receive()
