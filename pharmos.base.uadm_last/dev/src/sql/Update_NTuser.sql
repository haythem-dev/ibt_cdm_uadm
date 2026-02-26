////////////////////////////////////////////////////////
// 2 temp tables -> 1 für Datei neu - alt; 2 für userid, alt
////////////////////////////////////////////////////////
create temp table temp_ntuser (ntuser_old char(50), ntuser_new char(50));
create temp table temp_user_orig (id integer, ntuser_old char(50));

////////////////////////////////////////////////////////
// temp tables füllen
////////////////////////////////////////////////////////
load from 'C:\prj\uadm\bi_usermannheim2.txt' delimiter ';' insert into temp_ntuser;
insert into temp_user_orig (id, ntuser_old) select userid, ntuser from ppuser where ntuser in ( select ntuser_old from temp_ntuser)

////////////////////////////////////////////////////////
// füllung + anzahl überprüfen
////////////////////////////////////////////////////////
select * from temp_ntuser order by ntuser_old;
select * from temp_user_orig order by ntuser_old;
select ntuser from ppuser where ntuser like 'prz.dom%';

////////////////////////////////////////////////////////
// alte kennung mit neuer ersetzen
////////////////////////////////////////////////////////
update ppuser set ntuser = (select lower(ntuser_new) from temp_ntuser t, temp_user_orig o where o.ntuser_old = t.ntuser_old and o.id = userid ) 
where userid in (select id from temp_ntuser t, temp_user_orig o where o.ntuser_old = t.ntuser_old) 

////////////////////////////////////////////////////////
// füllung + anzahl überprüfen
////////////////////////////////////////////////////////
select userid, username, surname, ntuser, lastlogin from ppuser where ntuser like 'prz.dom%';
select surname, ntuser from ppuser, temp_user_orig where userid = id order by surname


select * from ppuser where ntuser !='' and ntuser not like 'phoenix%'
 
