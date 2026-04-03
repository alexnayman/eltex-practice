all: calc plugins/add.so plugins/sub.so plugins/mul.so plugins/dv.so plugins/sqr.so plugins/inv.so

calc: calc_main.c
	gcc calc_main.c -o calc -ldl

plugins/add.so: add.c
	mkdir -p plugins
	gcc -fPIC -shared -o plugins/add.so add.c

plugins/sub.so: sub.c
	mkdir -p plugins
	gcc -fPIC -shared -o plugins/sub.so sub.c

plugins/mul.so: mul.c
	mkdir -p plugins
	gcc -fPIC -shared -o plugins/mul.so mul.c

plugins/dv.so: dv.c
	mkdir -p plugins
	gcc -fPIC -shared -o plugins/dv.so dv.c

plugins/sqr.so: sqr.c
	mkdir -p plugins
	gcc -fPIC -shared -o plugins/sqr.so sqr.c

plugins/inv.so: inv.c
	mkdir -p plugins
	gcc -fPIC -shared -o plugins/inv.so inv.c

clean:
	rm -rf calc plugins
