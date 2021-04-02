#!/bin/sh
#
# Test against python and JS scripts
#
C_BASE45=./base45
PY_BASE45=${PY_BASE45:-$(python-config --prefix)/bin/base45}
JS_BASE45=${JS_BASE45:-${NPMDIR}/bin/base45}

I=0
while test ${I} -lt 100
do
   for len in ${I} $RANDOM
   do
	openssl rand ${len} > x
	cat x  |\
		${C_BASE45} | ${PY_BASE45} --decode |\
		${PY_BASE45} --encode | ${C_BASE45} -d |\
		${JS_BASE45} | ${C_BASE45} -d |\
		${C_BASE45}  |  ${JS_BASE45} -d |\
		${JS_BASE45} | ${PY_BASE45} --decode |\
		${PY_BASE45} --encode |  ${JS_BASE45} -d |\
	cat > y

	if ! diff x y; then
		echo FAIL
		break
	fi
     done
     I=$(expr ${I} + 1)
done
exit 0
