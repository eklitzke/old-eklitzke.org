from eck.handler import RequestHandler

class Home(RequestHandler):

    def get(self):
        if self.request.path != '/':
            self.redirect('/')
            return
        self.render_template('home.html')
