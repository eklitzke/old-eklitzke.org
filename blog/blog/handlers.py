import os
import tornado.web
from tornado.web import authenticated
from blog import base_path, config
from blog import db

class RequestHandler(tornado.web.RequestHandler):

    def initialize(self):
        super(RequestHandler, self).initialize()
        self.env = {'title': 'eklitzke.blog'}

    def get_current_user(self):
        return self.get_secure_cookie('user')

    def render(self, name, **kwargs):
        self.env.update(kwargs)
        return super(RequestHandler, self).render(name, **self.env)

class MainHandler(RequestHandler):

    path = '/'

    def get(self):
        post = self.get_argument('post', None)
        if post:
            posts = [db.Post.by_id(post)]
        else:
            posts = db.Post.query(order_by='time_created', desc=True, limit=5)
        self.env['posts'] = posts
        self.render('main.html')

class LoginHandler(RequestHandler):

    path = '/login'

    def get(self):
        self.render('login.html')

    def post(self):
        next = self.get_argument('next', '/')
        name = self.get_argument('name', None)
        password = self.get_argument('password', None)

        if name == 'evan' and password == 'topsecret':
            self.set_secure_cookie('user', 'evan')
        self.redirect('/')

class AdminHandler(RequestHandler):

    path = '/admin'

    @authenticated
    def get(self):
        self.env['posts'] = db.Post.query(order_by='time_created', desc=True)
        self.render('admin.html')

def builds_paths():
    paths = []
    for v in globals().itervalues():
        try:
            if issubclass(v, RequestHandler):
                if hasattr(v, 'path'):
                    paths.append((v.path, v))
                if hasattr(v, 'paths'):
                    paths.extend((p, v) for p in v.paths)
        except TypeError:
            continue
    return paths

application = tornado.web.Application(
    builds_paths(),
    login_url='/login',
    cookie_secret='3c8670ee7fbfea1757c7c25ae0bb1dd2',
    template_path=os.path.join(base_path, 'templates'),
    static_path=os.path.join(base_path, 'static'),
    xsrf_cookies=True,
    debug=True)
