all: install

install:
	(cd visualization/shotViz; make clean; make)
	(cd visualization/dbViz; make clean; make)
	(cd ai; make clean; make)
	(cd client; rm -f shotViz; ln -s ../visualization/shotViz/shotViz shotViz; make clean; make)

clean:
	(cd visualization/shotViz; make clean)
	(cd visualization/dbViz; make clean)
	(cd client; make clean)




