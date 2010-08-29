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
        orm.VARCHAR('title', 255, nullable=False),
        orm.TEXT('content', nullable=False),
        orm.DATETIME('time_created'),
        orm.DATETIME('time_updated')
        ]

    _indexes = [['time_created']]

    @classmethod
    def new_post(cls, title, content):
        now = datetime.datetime.now()
        return cls(post_id=schemaless.guid(), title=title, content=content, time_created=now, time_updated=now).save()

    def trimmed_content(self, length=80):
        if len(self.content) < length:
            return self.content
        return self.content[:length] + '...'

class PostTag(Base):

    _columns = [
        orm.CHAR('post_id', 32, nullable=False),
        orm.VARCHAR('tag', 255, nullable=False)
        ]

    _indexes = [['post_id'], ['tag']]
