import struct
import time
import logging
import socket
logger = logging.getLogger('client')



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


if __name__ == '__main__':
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('127.0.0.1', 9999))

    try:
        start = time.time()
        for i in range(10):
            send(client, 1, 'pingping' + str(i))
            recv(client)
            send(client, 1, 'ping' + str(i))
            recv(client)
        # send(client, opcode.REQ_ENTER_ROOM, 'test_room')
        # recv(client)
        # time.sleep(5)
        # print 'recv room msg'
        # for i in range(100):
        #   recv(client)
        # send(client, opcode.REQ_LEAVE_ROOM, 'test_room')
        # recv(client)
        print "Speend: ", time.time() - start
        client.close()
    except KeyboardInterrupt:
            exit(1)