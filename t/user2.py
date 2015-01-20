import struct
import time
import logging
import socket
logger = logging.getLogger('client')

#from gevent import socket, spawn, joinall,sleep

def send(client, code, msg):
    leng = len(msg)
    null = struct.pack('<B',  0)
    head = struct.pack('<HH', code, int(len(msg))+len(null))
    client.sendall(head+msg+null)

def recv(client):
    data = client.recv(4)
    cmd, leng  = struct.unpack("<HH", data)
    data = client.recv(leng)
    #print 'cmd : %d, msg : %s' %(cmd, data[:-1])


def room_message(client, code, uid, room_id, message=None):

    room_info = struct.pack('<II', uid, room_id)
    null = struct.pack('<B',  0)
    leng = len(room_info)

    msg = room_info
    if message:
        msg = room_info + message + null
        leng += len(null) + len(message)
    head = struct.pack('<HH', code, leng)
    client.sendall(head+ msg)

def client_send(client):
    room_message(client, 3, 12, 13)
    for i in range(9):
        room_message(client, 7, 12, 13, "Hello")

def client_recv(client):
    for i in range(10):
        recv(client)



if __name__ == '__main__':
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('127.0.0.1', 9999))
    # jobs = []

    # start = time.time()

    # jobs.append(spawn(client_send, client))
    # jobs.append(spawn(client_recv, client))

    # print "Done"
    # joinall(jobs)
    # print "Spend : ", time.time() - start
    try:
        start = time.time()
        room_message(client, 3, 99, 13)
        recv(client)
        # room_message(client, 7, 99, 13, "User2")
        # recv(client)

        for i in xrange(10000):
            room_message(client, 7, 99, 13, "User2 Message " + str(i))
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