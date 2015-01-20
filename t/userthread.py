import struct
import time
import logging
import socket
logger = logging.getLogger('client')
import threading


def send(client, code, msg):
    leng = len(msg)
    null = struct.pack('<B',  0)
    head = struct.pack('<HH', code, int(len(msg))+len(null))
    client.sendall(head+msg+null)

def recv(client):
    data = client.recv(4)
    cmd, leng  = struct.unpack("<HH", data)
    data = client.recv(leng)
    print 'cmd : %d, msg : %s' %(cmd, data[:-1])

    
def room_message(client, code, uid, room_id, message=None):

    room_info = struct.pack('<II', uid, room_id)
    null = struct.pack('<B',  0)
    leng = len(room_info)

    msg = room_info
    if message:
        msg = room_info + message + null
        leng += len(null) + len(message)
    head = struct.pack('<HH', code, leng)
    client.send(head+ msg)

def client_send(client):
    room_message(client, 3, 12, 13)
    for i in range(9):
        print "client_send"
        room_message(client, 7, 12, 13, "Hello")

def client_recv(client):
    #time.sleep(2)
    for i in range(10):
        recv(client)



if __name__ == '__main__':
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('127.0.0.1', 9999))
    send_t = threading.Thread(target=client_send, args=(client,))
    recv_t = threading.Thread(target=client_recv, args=(client,))
    send_t.start()
    recv_t.start()

    send_t.join()
    recv_t.join()