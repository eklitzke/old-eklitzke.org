import datetime
import mimetypes

from google.appengine.ext.webapp.util import login_required
from google.appengine.api import users
from eck.handler import RequestHandler, handle_error
from eck.model.object import ObjectStorage

class ObjectManager(RequestHandler):

    @login_required
    def get(self):
        self.env['title'] = 'object manager'
        query = ObjectStorage.all()
        query.order('name')
        objects = query.fetch(100)
        for obj in objects:
            obj.public = bool(obj.public)
            obj.size_kb = (obj.size_hint or 0) / 1024.0

        self.env['num_objects'] = len(objects)
        self.env['objects'] = objects
        self.render_template('object_manager.html')

    def post(self):
        given_name = self.request.get('filename')
        notes = self.request.get('comments')
        data = self.request.get('file_obj')

        obj = ObjectStorage(name=given_name,
                            data=data,
                            owner=users.get_current_user(),
                            created=datetime.datetime.now())
        obj.public = bool(self.request.get('public'))
        obj.size_hint = len(data)
        obj.notes = notes
        obj.put()

        self.redirect('/data')

class DataAccess(RequestHandler):

    @handle_error
    def get(self, name):
        query = ObjectStorage.all()
        query.filter('name =', name)
        results = list(query.fetch(1))
        if not results:
            self.error(404)

        obj = results[0]
        if not obj.public:
            self.error(403)

        content_type, encoding = mimetypes.guess_type(name)
        self.response.headers['Content-Type'] = content_type
        if encoding:
            self.response.headers['Content-Encoding'] = encoding
        self.response.out.write(obj.data)

    def post(self, key_id):
        obj = ObjectStorage.get_by_id(int(key_id))

        if self.request.get('delete', 'false') == 'true':
            obj.delete()
            self.redirect('/data')
            return

        self.log.info('form is_public = %s' % self.request.get('public'))
        is_public = self.request.get('public', 'false') == 'true'
        self.log.info('is_public = %s' % (is_public,))
        obj.public = is_public
        obj.put()

        self.redirect('/data')
