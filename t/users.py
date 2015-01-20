#!/usr/bin/env python
# encoding: utf-8

from gevent import socket, spawn, joinall,sleep
import sys
import random
import struct
import os
import time


import logging


def setdebug(debug=False):
    level = logging.DEBUG if debug else logging.INFO
    logging.basicConfig(level=level,
                        format='%(asctime)s %(levelname)-8s %(message)s',
                        datefmt='%Y-%m-%d %H:%M:%S', filemode='a+')

LOGGER = logging.getLogger('users')

def send(client, code, msg):
    leng = len(msg)
    null = struct.pack('<B',  0)
    head = struct.pack('<HH', code, int(len(msg))+len(null))
    client.sendall(head+msg+null)

def recv(client, uid):
    data = client.recv(4)
    cmd, leng  = struct.unpack("<HH", data)
    data = client.recv(leng)
    LOGGER.debug('uid: %d, cmd : %d, msg : %s' ,uid, cmd, data[:-1])


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

def client_send(client, uid):
    room_message(client, 3, uid, 13)
    # for i in range(9):
    #     room_message(client, 7, uid, 13, "User " + str(uid))

def client_recv(client, uid):
    for i in xrange(10000+1):
        recv(client, uid)

setdebug(True)
jobs = []

start = time.time()
for i in xrange(40):
    ss = socket.socket()
    ss.connect(('127.0.0.1', 9999))
    jobs.append(spawn(client_send, ss, i))
    jobs.append(spawn(client_recv, ss, i))

print "Done"
joinall(jobs)
print "Spend : ", time.time() - start