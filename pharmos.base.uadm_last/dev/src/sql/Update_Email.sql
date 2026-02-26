////////////////////////////////////////////////////////
// 2 temp tables -> 1 für Datei neu - alt; 2 für userid, alt
////////////////////////////////////////////////////////
create temp table temp_email (ntuser char(50), emailuser char(50));
create temp table temp_user_orig (id integer, ntuser_orig char(50));
create temp table temp_email_delete (ntuser_delete char(50));

////////////////////////////////////////////////////////
// temp tables füllen
////////////////////////////////////////////////////////
load from 'C:\prj\uadm\bi_emailuser.txt' delimiter ';' insert into temp_email (ntuser, emailuser);

insert into temp_email_delete  (ntuser_delete) select ntuser  from temp_email where emailuser != "" ;
delete from temp_email where ntuser not in ( select ntuser_delete from temp_email_delete )
delete from temp_email where ntuser  = 'neuhausen.dom\nh.zbv'			--> SPEICHERN + HÄNDISCH SPÄTER

insert into temp_user_orig (id, ntuser_orig) select userid, ntuser from ppuser where ntuser in ( select ntuser from temp_email)

select ntuser from ppuser, temp_user_orig where ntuser = ntuser_orig and userid != id	--> SPEICHERN + HÄNDISCH SPÄTER

delete from temp_email where ntuser in ( select ntuser from ppuser, temp_user_orig where ntuser = ntuser_orig and userid != id )

delete from temp_user_orig

insert into temp_user_orig (id, ntuser_orig) select userid, ntuser from ppuser where ntuser in ( select ntuser from temp_email)

update ppuser set emailuser = (select t.emailuser from temp_email t, temp_user_orig o where o.ntuser_orig = t.ntuser and o.id = userid ) 
where userid in (select o.id from temp_email t, temp_user_orig o where o.ntuser_orig = t.ntuser)  


////////////////////////////////////////////////////////
// füllung + anzahl überprüfen
////////////////////////////////////////////////////////
select * from ppuser where emailuser = '' and ntuser != ''
select * from temp_email;
select * from temp_user_orig

select ntuser from temp_email where ntuser not in (select ntuser_orig from temp_user_orig)



