#!/bin/sh
#
#  utf8_clipping/test.sh
#

rm -f tmp/utf8.*
mkdir -p tmp
#../osmjs/osmjs --debug --include=../osmjs/js/osm2shape.js --javascript=utf8_clipping/utf8_clipping.js utf8_clipping/utf8_clipping.osm
../../osmjs/osmjs --debug --include=osm2shape_with_debug.js --javascript=utf8_clipping.js utf8_clipping.osm

