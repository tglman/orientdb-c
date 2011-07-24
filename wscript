#! /usr/bin/env python
# encoding: utf-8

import platform, os
import Environment, Scripting, Runner, Options

APPNAME = 'orientdb-c'
VERSION = '0.9'


def configure(conf):
        conf.check_tool('gcc')


def build(bld):
        obj = bld.new_task_gen (features = 'c cshlib')
        obj.source = bld.path.ant_glob ('src/*.c ')
        obj.target = APPNAME + '.' + VERSION
        start_dir = bld.path.find_dir('src')
        bld.install_files('${PREFIX}/include/' + APPNAME , ['o_document.h' ,'o_document_value.h', 'o_database_document.h','o_database.h','o_record.h','o_record_id.h','o_database_error_handler.h','o_output_stream.h','o_input_stream.h','o_metadata.h','o_schema.h','o_class.h','o_property.h','o_transaction.h','o_query.h','o_list_record.h','o_list_document.h'],cwd = start_dir)

