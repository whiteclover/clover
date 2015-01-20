
from clover.token import encode_token, decode_token, AccessToken
from webob import exc
from solo.web.util import jsonify

def UserMenu(m):
	
	UserCtl = UserController()

	m.connect('user.auth', '/auth', controller=UserCtl, action='DebugAuth')


class UserController(object):

	@jsonify
	def DebugAuth(self, uid, password=None):
		try:
			uid = int(uid)
		except:
			raise exc.HTTPBadRequest('uid must be int type')

		return AccessToken(uid)


	@jsonify
	def Auth(self, username, password=None):

		user = Backend('user').FindByName(username)
		if user is None:
			raise exc.HTTPBadRequest('the user not exist')

		if user.Check(password): #and not user.IsBanned():
			return AccessToken(user.uid, user.serect)
		else:
			raise exc.HTTPBadRequest('the password is not correct')

