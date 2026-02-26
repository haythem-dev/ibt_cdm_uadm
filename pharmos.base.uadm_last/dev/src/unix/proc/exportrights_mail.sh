#! /usr/bin/ksh

#--------------------------------------------------------------------#
# READ IN GLOBAL VARIABLES
#--------------------------------------------------------------------#
. $(dirname $0)/../vars/uadm_global.vars

#--------------------------------------------------------------------#
# PARAMETERS
#--------------------------------------------------------------------#
typeset -i rc=0;

#--------------------------------------------------------------------#
# CHECK GIVEN PARAMETERS
#--------------------------------------------------------------------#

DIR=${DATA_PATH}
PREFIX="CH_ExportRights"
SUFFIX=".csv"
cd $DIR
rc=$?
if [[ rc -ne 0 ]]
then
   echo "Error ${rc}. Problems accessing directory '${DIR}'. Please check!"
   exit $rc
fi

for DATAFILE in $( ls -rtl ${PREFIX}*${SUFFIX} | awk '{print $9}' )
do
	export TIMESTAMP=$(date '+%Y%m%d_%H%M%S')
	export FILESTAMP=${PREFIX}_${TIMESTAMP}${SUFFIX}
	export MAILFROM="PHARMOS UADM Export"
	export MAILTO="t.arnold@externals.phoenixgroup.eu, w.feki@externals.phoenixgroup.eu, s.tahari@phoenixgroup.eu"
	export SUBJECT="Export Rights for CH (file timestamp ${FILESTAMP})"
	export ATTACH=$DATAFILE
	(
		echo "To: $MAILTO"
		echo "Subject: $SUBJECT"
		echo "MIME-Version: 1.0"
		echo 'Content-Type: multipart/mixed; boundary="-q1w2e3r4t5"'
		echo
		echo '---q1w2e3r4t5'
		echo "Content-Type: text/html"
		echo "Content-Disposition: inline\n"
		echo "Dear colleagues,<br><br>"
		echo "The exported rights from UADM and Mainframe and OrderEntry are created.<br>"
		echo "You can find in the attachment the report</b>.<br><br>\n"
		echo "Best regards<br>\n"
		echo "PHARMOS UADM Export<br>\n"
		echo '---q1w2e3r4t5'
		echo "Content-Type: text/csv;"
		echo "Content-Disposition: attachment; filename=\"$ATTACH\";"
		echo
		cat "$ATTACH"
		echo '---q1w2e3r4t5--'
	) | /usr/sbin/sendmail $MAILTO -f $MAILFROM
	echo "file '$DATAFILE' was mailed successfully."

	if [[ $rc == 0 ]]; then
		mv "${DIR}/${DATAFILE}" "${DIR}/archive/${FILESTAMP}"
	fi
done;
if [[ $rc != 0 ]]; then
	echo "no file '$DATAFILE' report was not created, no mail send out"
	rc=0
fi

exit $rc
