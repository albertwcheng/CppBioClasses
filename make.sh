g++ -o TestGff -I../CppUtilClasses/ TestGff.cpp

if [ ! -e $SAMTOOLPATH/sam.h ]; then
	echo '$SAMTOOLPATH not defined or $SAMTOOLPATH/sam.h not found'
	exit
fi
g++ -o TestBamUtil -I$SAMTOOLINCLUDEPATH -L$SAMTOOLLIBPATH -lm -lbam -lz TestBamUtil.cpp $SAMTOOLLIBPATH/libbam.a