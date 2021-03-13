#!/bin/sh

xgettext -jFo en_US/messages.po --keyword=_ --from-code=ISO-8859-1 ../src/*.cpp ../include/*.h
