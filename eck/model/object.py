from google.appengine.ext import db

class ObjectStorage(db.Model):
    name = db.StringProperty(required=True) # unique
    data = db.BlobProperty(required=True)
    size_hint = db.IntegerProperty()
    owner = db.UserProperty(required=True)
    public = db.BooleanProperty()
    notes = db.TextProperty()
    created = db.DateTimeProperty(required=True)
