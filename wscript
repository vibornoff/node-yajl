import Options
from os import popen, unlink, symlink, getcwd
from os.path import exists

APPNAME = 'yajl-js'
VERSION = '0.5.1'

srcdir = '.'
blddir = 'build'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  
  libyajl = conf.check( header_name='yajl/yajl_version.h' )
  if not libyajl:
    conf.fatal( "Could not find yajl headers. Make sure libyajl development package is installed." )

def build(bld):
  obj = bld.new_task_gen( 'cxx', 'shlib', 'node_addon' )
  obj.target = 'yajl'
  obj.source = 'src/yajl.cc src/handle.cc src/tree.cc'
  obj.lib = 'yajl'
