#!/usr/bin/bash -e

export BASEDIR=$(pwd)
export PROJECT=uadm
export SRCDIR=${BASEDIR}
export EXPORTDIR=${BASEDIR}/../../export/${PROJECT}

echo "exporting ${PROJECT} to ${EXPORTDIR}..." >&2

rm -rf $EXPORTDIR
mkdir -p ${EXPORTDIR}/bin
mkdir -p ${EXPORTDIR}/proc
mkdir -p ${EXPORTDIR}/vars
mkdir -p ${EXPORTDIR}/lib
mkdir -p ${EXPORTDIR}/include

if [ ! -d ${SRCDIR}/unix/onlineserver/build/release ] ; then
	echo "error: ${PROJECT} has not been built." >&2
	exit 1
fi

echo "copy binaries"
cp -r ${SRCDIR}/unix/exportrights/build/release/*.bin ${EXPORTDIR}/bin/
cp -r ${SRCDIR}/unix/loadbireports/build/release/*.bin ${EXPORTDIR}/bin/
cp -r ${SRCDIR}/unix/proc/*.sh ${EXPORTDIR}/proc/
cp -r ${SRCDIR}/unix/proc/*.option ${EXPORTDIR}/proc/
cp -r ${SRCDIR}/unix/vars/*.vars ${EXPORTDIR}/vars/
cp -r ${SRCDIR}/unix/onlineserver/*.h ${EXPORTDIR}/include/
cp -r ${SRCDIR}/unix/onlineserver/build/release/*.so ${EXPORTDIR}/lib/

cd ${EXPORTDIR}
echo "move unix files in archive built.tar.gz"
TARDIRS=`ls -d *`
tar --remove-files -czf ./$PROJECT.tar.gz $TARDIRS
rm -rf $TARDIRS

cp ${SRCDIR}/ivy.xml ${EXPORTDIR}
cp ${SRCDIR}/build.xml ${EXPORTDIR}

echo "done" >&2
cd ${BASEDIR}
