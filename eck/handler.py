from jinja2 import Environment, PackageLoader
jinja_env = Environment(loader=PackageLoader('eck', 'templates'))

from google.appengine.ext.webapp import RequestHandler as _RequestHandler

class RequestHandler(_RequestHandler):

	def initialize(self, request, response):
		super(RequestHandler, self).initialize(request, response)
		response.headers['Content-Type'] = 'text/html; charset=utf-8'

	def get_template(self, name):
		return jinja_env.get_template(name)

	def render_template(self, name, **kwargs):
		template = self.get_template('index.html')
		self.response.out.write(template.render(**kwargs))
