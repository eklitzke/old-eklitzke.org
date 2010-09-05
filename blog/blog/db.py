import os
import datetime

import schemaless
from schemaless import c
from schemaless import orm

from blog import base_path, config

datastore = schemaless.DataStore(**config['datastore'])
session = orm.Session(datastore)
Base = orm.make_base(session, tags_file=os.path.join(base_path, 'tags.yaml'))

class Post(Base):

    _columns = [
        orm.String('title', 255, required=True),
        orm.Text('content', required=True),
        orm.DateTime('time_created'),
        orm.DateTime('time_updated'),
        orm.Boolean('active', default=False)
        ]

    _indexes = [['time_created'], ['active', 'time_created']]

    @classmethod
    def new_post(cls, title, content, active=False):
        now = datetime.datetime.now()
        return cls(post_id=schemaless.guid(), title=title, content=content, time_created=now, time_updated=now, active=active).save()

    @classmethod
    def active(cls):
        return c.active == True

    def trimmed_content(self, length=80):
        if len(self.content) < length:
            return self.content
        return self.content[:length] + '...'

class PostTag(Base):

    _columns = [
        orm.Char('post_id', 32, required=True),
        orm.String('tag', 255, required=True),
        ]

    _indexes = [['post_id'], ['tag']]
