
from clover.token import encode_token, decode_token, AccessToken
from webob import exc
from solo.web.util import jsonify


def GuildMenu(m):
    
    GuildCtl = GuildController()
    m.connect('guild.verify', '/guild/verify', controller=GuildCtl, action='DebugVerify')

GUILD_FULL = 50

class GuildController(object):

    @jsonify
    def DebugVerify(self, token, guild_id):
        token = decode_token(token)
        if token is None:
            return {'status' : 'auth', 'msg' : 'token is not validate'}
        name =   'name-' + str(token.uid)
        return {'status': 'ok', 'name': name, 'uid' : token.uid}
        
    @jsonify
    def Verify(self, token, guild_id):
        try:
            guild_id = int(guild_id)
        except:
            return {'status' : 'error', 'msg' : 'guild id must be int'}

        user = Backend('user').FindByUID(lookup_uid(token))
        if not user:
            return {'status' : 'error', 'msg' : 'not find user'}
            
        token = decode_token(token, user.secret)
        if token is None:
            return {'status' : 'auth', 'msg' : 'token is not validate'}

        if Backend('member').Find(guild_id, token.uid):
            return {'status': 'ok', 'name': member.name, 'uid' : member.uid}
        return {'status': 'error', 'msg' : 'user is not in the guild :%d' % (guild_id) }


    @jsonify
    def Reuqest(self, token, guild_id):
        try:
            guild_id = int(guild_id)
        except:
            return {'status' : 'error', 'msg' : 'guild: "%d" id must be int' % (guild_id) }

        # chekc user whether join any guild
        member = Backend('member').FindByUid(token.uid)
        if member:
            return {'status' : 'joined', 'msg' : 'you had joined the guild "%d"' % (guild_id)}

        guild = Backend('guild').Find(guild_id)
        if guild:

            if guild.status == 'auto' and guild.size < GUILD_FULL:
                member = Member(token.uid, guild_id)
                Backend('member').Save(member)
                Backend('guild').IncrMemberCount(guild_id)
                return {'status' : 'ok', 'msg' : 'add success'}

            elif guild.status == 'allow':
                req = Backend('requests').Find(token.uid, guild_id)
                if req:
                    return {'status' : 'requested', 'msg' : ' You had sent reuqest'}
                Backend('requests').Save(Request('uid', guild_id))
                Backend('inbox').Push(Email('uid'))
                return {'status' : 'wait', 'msg' : 'wait guilder handle your request'}

            elif guild.status == 'forbidden':
                return {'status' : 'error', 'msg' : 'the guild not allowed member join'}

        return {'status' : 'unknow', 'msg' : 'Unkonw error'}
