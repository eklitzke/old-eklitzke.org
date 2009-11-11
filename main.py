# fix up the environment before anything else
from eck import tweaks
from jinja2 import Environment, PackageLoader

from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from google.appengine.ext.webapp import RequestHandler

env = Environment(loader=PackageLoader('eck', 'templates'))

class Home(RequestHandler):

	def get(self):
		template = env.get_template('index.html')
		self.response.headers['Content-Type'] = 'text/html; charset=utf-8'
		self.response.out.write(template.render())


application = webapp.WSGIApplication(
    [('/', Home)],
    debug=True)

def main():
    run_wsgi_app(application)

if __name__ == '__main__':
    main()
