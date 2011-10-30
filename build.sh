# Go to Nuttx dir
cd nuttx

# Clean the garbage from previous builds
make clean distclean

# Configure for WEIO board
cd tools
./configure.sh weio/nsh
cd ..

# Make it !
make

# Copy the nuttx.bin to project home dir
if [ -f nuttx.bin ]
then
    echo ""
    echo ">>> WEIO binary is ready"
    echo ""
    cp nuttx.bin ../weio.bin
    cp nuttx ../weio.elf
fi

# Go back to project home dir
cd ..




