
def InboxMenu(m):
    
    InboxCtl = InboxController()

    m.connect('inbox.get', '/inbox', controller=InboxCtl, action='Get')


class InboxController(object):

    def Get(self, token):
        pass


    def Label(self, token, status):
        pass


    def Send(self, token, form_user, to_user, data):
        pass


