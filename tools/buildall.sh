echo cleaning up
make distclean ARCH=arm
make distclean ARCH=x86 
echo make all for arm
make all ARCH=arm
make clean
echo make all for x86
make all ARCH=x86
make clean
echo all done
