import Options
from os import popen, unlink, symlink, getcwd
from os.path import exists

APPNAME = 'yajl'
VERSION = '0.8.0'

srcdir = '.'
blddir = 'build'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  conf.check( header_name='yajl/yajl_version.h',
              define_name='yajl2',
              execute=True,
              define_ret=True,
              fragment='''
                  #include <yajl/yajl_version.h>
                  int main (void) { return (YAJL_MAJOR >= 2) ? 0 : 1; }
              ''' )

  if not conf.env.yajl2:
    conf.fatal( "Could not find required yajl headers.\nMake sure libyajl at least of version 2.0 and its development headers are installed." )

def build(bld):
  obj = bld.new_task_gen( 'cxx', 'shlib', 'node_addon' )
  obj.target = 'yajl'
  obj.source = 'src/yajl.cc src/handle.cc'
  obj.lib = 'yajl'
