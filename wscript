#! /usr/bin/env python
# encoding: utf-8

import platform, os
from waflib import Utils,Build
APPNAME = 'orientdb-c'
VERSION = '0.9'

def options(opt):
	opt.load('compiler_c')

def configure(conf):
	conf.load('compiler_c')

def build(bld):
	bld.shlib(source=bld.path.ant_glob ('src/*.c'),target=APPNAME + '-' + VERSION)
	start_dir = bld.path.find_dir('src')
	bld.install_files('${PREFIX}/include/' + APPNAME , ['o_document.h' ,'o_document_value.h', 'o_database_document.h','o_database.h','o_record.h','o_record_id.h','o_database_error_handler.h','o_output_stream.h','o_input_stream.h','o_metadata.h','o_schema.h','o_class.h','o_property.h','o_transaction.h','o_query.h','o_list_record.h','o_list_document.h'],cwd = start_dir)

def run_test(bld):
	env = os.environ.copy()
	env['LD_LIBRARY_PATH']='build'
	Utils.subprocess.Popen('ctesf-console build/lib' + APPNAME + '-test-' + VERSION+'.so',env=env,shell=True)

def check(bld):
	bld.find_program('ctesf-console')
	build(bld);
	bld.add_post_fun(run_test)
	bld.shlib(
		source=bld.path.ant_glob ('test/*.c'),
		libpath=['build/'],
		includes=['/usr/include/ctesf-1.0/'],
		target=APPNAME + '-test-' + VERSION,
		use=APPNAME+'-'+VERSION
	)



class CheckBuild(Build.BuildContext):
	cmd = 'check'
	fun = 'check'
