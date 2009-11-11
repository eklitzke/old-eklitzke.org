# fix up the environment before anything else
from eck import tweaks

from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app

from eck.view import *

application = webapp.WSGIApplication(
    [('/', Home),
	 ('/.*', Home) # Base case is to go to home, which redirects to /
	], debug=True)

def main():
    run_wsgi_app(application)

if __name__ == '__main__':
    main()
