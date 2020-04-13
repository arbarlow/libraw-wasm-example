.DEFAULT_GOAL := build-as-lib

build:
	emcc -s WASM=1 --bind \
	  -s ASSERTIONS=1 -s SAFE_HEAP=1 -s DETERMINISTIC=1 \
	  -s ALLOW_MEMORY_GROWTH=1 \
	  -s MODULARIZE=1 \
	  -I libraw \
	  libraw/object/*.o \
	  -s 'EXPORT_NAME="LibRaw"' \
	  rawr.cpp -o raw.js
	cp raw.js ../src/
	cp raw.wasm ../public/static/js/raw.wasm

build-as-lib:
	emcc -s WASM=1 --bind \
	  -s ASSERTIONS=1 -s SAFE_HEAP=1 -s DETERMINISTIC=1 \
	  -s ALLOW_MEMORY_GROWTH=1 \
	  -s MODULARIZE=1 \
	  -I libraw \
	  -l libraw \
	  -L libraw/lib \
	  -s 'EXPORT_NAME="LibRaw"' \
	  rawr.cpp -o raw.js
	cp raw.js ../src/
	cp raw.wasm ../public/static/js/raw.wasm

unarchive:
	tar -zxvf LibRaw-0.19.5.tar.gz
	mv LibRaw-0.19.5 libraw
	cd libraw
	emconfigure ./configure
	emmake make -f Makefile.dist
