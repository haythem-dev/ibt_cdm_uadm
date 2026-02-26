#!/bin/bash

_DBNAME=$1;
_DAYSTOCHECK=$2;


function checkParameter()
{
	 if [ -z "$_DBNAME" ]; 
	 then 
		echo "USAGE: zpcua1@ode21_tcp 60"
		exit 99
	 fi
	 
	 if [ -z "$_DAYSTOCHECK" ]; 
	 then 
		echo "USAGE: zpcua1@ode21_tcp 60"
		exit 99
	 fi
}


function setLastPasswdChange()
{

dbaccess ${_DBNAME} <<eof
update ppuser set changepw = 1 
where userid in 
(
	select userid from ppuser 
		where changepw = 0
	AND 
		(lastpwchange = 0	OR	 ( lastpwchange > 0 AND
		(( to_date(to_char(lastpwchange), '%Y%m%d')::DATE)  + ${_DAYSTOCHECK} UNITS DAY ) < CURRENT))  
);
eof

}

checkParameter
setLastPasswdChange



