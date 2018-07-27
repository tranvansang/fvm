all:
	make -C ../reorder0/
	make -C ../src0/
	rm -rf test2.err test2.lst
	echo "Run original program"
	./L3-sol0
	echo "Run sequential program"
	./L3-rsol0

sub:
	qsub go2.sh
