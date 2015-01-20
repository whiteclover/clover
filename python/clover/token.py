import base64
import hmac
import hashlib
import time

SECRET = ''

class AccessToken(object):

	def __init__(self, uid, secret='7oGwHH8NQDKn9hL12Gak9G/MEjZZYk4PsAxqKU4cJoY=', expired_at=None):
		self.uid = uid
		self.secret = secret
		self.expired_at = expired_at or (time.time() + 60*60*4)


	def __str__(self):

		uid = str(self.uid)
		expired_at = str(self.expired_at)
		return uid  + '|' +  expired_at  + '|' + _signed_secret(uid, expired_at, self.secret)


	def as_json(self):
		return {'token' : str(self)}


def _signed_secret(uid, expired_at, secret):
		digest = hmac.new(SECRET + secret, uid + expired_at , hashlib.sha1).digest()
		return base64.b64encode(digest)

def encode_token(uid, secret='7oGwHH8NQDKn9hL12Gak9G/MEjZZYk4PsAxqKU4cJoY=', expired_at=None):
	return AccessToken(uid, secret, expired_at)


def decode_token(token, secret='7oGwHH8NQDKn9hL12Gak9G/MEjZZYk4PsAxqKU4cJoY='):

	parts = token.split('|')

	if len(parts) != 3:
		return None

	uid, expired_at, signed_secret = parts
	if signed_secret != _signed_secret(uid, expired_at, secret):
		return None

	expired_at = float(expired_at)
	if expired_at <= time.time():
		return None

	uid = int(uid)
	
	return AccessToken(uid, secret, expired_at)

