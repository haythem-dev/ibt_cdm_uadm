#!/bin/sh

#########################################################################
#
# copy project files to export directory (tar.gz)
#   - header files
#   - libraries
#   - shell scripts for installation
#
#########################################################################

# exit immediately if a command exits with non-zero exit status
set -e

PROJECT=libtpld

SAVEDIR=`pwd`
BASEDIR=`CDPATH=. cd $SAVEDIR/../.. && pwd`
SRCDIR=$BASEDIR/dev/src
DESTDIR=$BASEDIR/export/$PROJECT

echo "-----------------------------------------------------"
echo " copy $PROJECT files to export directory"
echo "      from $SRCDIR"
echo "      to   $DESTDIR"
echo " compress export files to $DESTDIR/$PROJECT.tar.gz"
echo "-----------------------------------------------------"

rm -rf $DESTDIR
mkdir -p $DESTDIR/lib $DESTDIR/include/libtpld

echo "copy header files"
cp -r $SRCDIR/libtpld/*.h $DESTDIR/include/libtpld/

echo "copy library files"
cp -r $SRCDIR/build/debug $DESTDIR/lib/
cp -r $SRCDIR/build/release $DESTDIR/lib/

cd $DESTDIR
echo "move files in archive $PROJECT.tar.gz"
gtar -czf ./$PROJECT.tar.gz include lib
rm -rf include/ lib/

cp $SRCDIR/ivy.xml $DESTDIR
cp $SRCDIR/build.xml $DESTDIR

cd $SAVEDIR

