#!/usr/bin/env python
# Copyright (C) 2015 Thomas Huang
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


import base64
import uuid
from solo.decorator import lazy_attr

from hashlib import sha256
from datetime import datetime

class Account(object):

    def __init__(self, username, email, real_name, password, status, 
        secret=None, role='user', uid=None, created=None):
        """If the user load from database, if will intialize the uid and secure password.
        Otherwise will hash encrypt the real password

        arg role enum: the string in ('user', 'administrator', 'root')
        arg status enum: the string in ('actived', 'banned')
        arg password fix legnth string: the use sha256 password hash
        """

        self.username = username
        self.email = email
        self.real_name = real_name
        self.status = status
        self.role = role
        self.secret = secret or base64.b64encode(uuid.uuid4().bytes + uuid.uuid4().bytes)
        self.created = created or datetime.now()

        if uid is not None:
            self.uid = uid
            self._password = password
        else:
            self.uid = None
            self._password = self.SecurePassword(password)

    def password():
        doc = "The password property."
        def fget(self):
            return self._password
        def fset(self, value):
            self._password = self.SecurePassword(value)
        def fdel(self):
            del self._password
        return locals()
    password = property(**password())

    def Check(self, password):
        """Check the password"""
        return self.password == self.SecurePassword(password)

    def SecurePassword(self, password):
        """Encrypt password to sha256 hash"""
        return sha256(password).hexdigest()

    def IsBanned(self):
        return self.status == 'banned'

    def as_json(self):
        data = self.__dict__.copy()
        del data['_password']
        return data



class Guild(object):


    def __init__(self, name, gid):
        self.owner = owner
        self.gid = gid


    @lazy_attr
    def members(self):
        return Backend('member').FindByGID(self.gid)


class Email(object):

    def __init__(self, **data):
        self.__dict__.update(data)