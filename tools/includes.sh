#!/bin/bash

INCLUDES="../i6engine-utils/include/ ../i6engine-core/include/ ../i6engine-modules/include/"

for i; do
	~/i6engine/tools/includes $i $INCLUDES> `basename ${i}`.dot
	dot -Tpdf `basename ${i}`.dot > `basename ${i}`.pdf
done
