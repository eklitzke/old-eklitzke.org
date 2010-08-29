import os
import yaml

base_path = os.path.realpath(os.path.join(os.path.dirname(__file__), '..'))
if not os.path.exists(os.path.join(base_path, 'config.yaml')):
    base_path = '/etc/blog'

yaml_path = os.path.join(base_path, 'config.yaml')
with open(yaml_path) as yaml_file:
    config = yaml.load(yaml_file)

import blog.db
import blog.handlers
