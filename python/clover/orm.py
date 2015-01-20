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

import db



class ChatServerMapper(object):


    def Find(self, roomName=None):
        pass


    def Balance(self):
        pass



class RoomMapper(object):

    pass



class InBoxMapper(object):

    TABLE = 'inbox'

    def Find(self, uid):
        emails = (db.select(self.TABLE)
            .condition('recepter', uid)
            .condition('status', 'deleted', '<>')
            .condition('created_at', datatime.now(), '>')
            .order_by('created_at', 'DESC')
            .execute(as_dict=True))

        return [self._Load(email) for email in emails]


    def _Load(self, data):
        return Email(**data)


    def Label(self, uid, status):
        return db.update(self.TABLE).condition('recepter', uid).set('status', status).execute()

    def Save(self, email):
        db.insert(self.TABLE).values(email.__dict__).execute()

    Push = Save
