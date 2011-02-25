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
