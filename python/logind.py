from solo.web.server import WebServer

if __name__ == '__main__':
    from clover.server import LoginApp
    app = LoginApp()
    WebServer(('127.0.0.1', 8080), app, log=None).start()