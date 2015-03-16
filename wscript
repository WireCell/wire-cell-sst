#!/usr/bin/env python

APPNAME = 'WireCellSst'

def options(ctx):
    ctx.load('find_package')

def configure(ctx):
    ctx.load('find_package')
#    ctx.env.LIBPATH_WireCell = ctx.options.prefix + '/lib'
#    ctx.env.INCLUDES_WireCell = ctx.options.prefix + '/include'
    ctx.check_cxx(lib='WireCellData', libpath=ctx.options.prefix + '/lib', 
                  uselib_store='WireCell')
    ctx.check_cxx(lib='WireCellNav', libpath=ctx.options.prefix + '/lib',
                  uselib_store='WireCell')



def build(bld):
    # main code library
    bld.shared_library(use='WireCell')
    bld.api_headers()
    bld.root_dictionary()#includes=bld.env.INCLUDES_WireCell)

#    for testsrc in bld.path.ant_glob('test/test_*.cxx'):
#        bld.test_program([testsrc])
