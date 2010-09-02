import os
import datetime
import optparse
from blog import db

def just_one(*args):
    c = 0
    for a in args:
        if a:
            c += 1
    return c == 1

def get_post(post_id):
    post = db.Post.by_id(post_id)
    if not post:
        parser.error('No such post: %s' % (post_id,))
    return post

def parse(text):
    title_delim = text.find('\n')
    if not title_delim:
        raise ValueError('File has no newlines!')
    title = text[:title_delim].strip()
    body = text[title_delim:].strip()
    return title, body

if __name__ == '__main__':
    parser = optparse.OptionParser()
    parser.add_option('-n', '--new', default=False, action='store_true', help='Write a new post')
    parser.add_option('-u', '--update', default=None, help='Update an older post')
    parser.add_option('-d', '--delete', default=None, help='Delete a older post')
    parser.add_option('--dump', default=None, help='Delete a older post')
    opts, args = parser.parse_args()

    if not just_one(opts.new, opts.update, opts.delete, opts.dump):
        parser.error('Must specify exactly one from -n, -u, -d, --dump')

    if opts.new or opts.update or opts.dump:
        if opts.new or opts.update:
            if len(args) != 1:
                parser.error('Expected exactly one argument for -n/-u')
            filepath = args[0]
            if not os.path.exists(filepath):
                parser.error('No such file: %r' % (filepath,))
            with open(filepath) as f:
                title, body = parse(f.read())

        if opts.update:
            post = get_post(opts.update)
            post.title = title
            post.content = body
            post.time_updated = datetime.datetime.now()
            post.save()
        elif opts.new:
            post = db.Post.new_post(title, body)
            print post.id
        elif opts.dump:
            post = get_post(opts.update)
            print post.title
            print
            print post.content
    elif opts.delete:
        post = get_post(opts.delete)
        post.delete()
    else:
        assert False
