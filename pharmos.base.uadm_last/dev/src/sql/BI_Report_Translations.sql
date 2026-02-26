create temp table th_transl (appname char (50), modname char (50), riname char (500));
			
load from 'C:\Dokumente und Einstellungen\thomas.hoerath\Desktop\biberichters.csv' delimiter ';'
insert into th_transl

select * from th_transl

create temp table th_right (riname char (10), ridesc char (500), appname char (50), modname char (50))

insert into th_right
--select substring(riname from 0 for 7), riname, appname, modname from th_transl where riname[7] = '';
--select substring(riname from 0 for 8), riname, appname, modname from th_transl where riname[8] = '';
select substring(riname from 0 for 9), riname, appname, modname from th_transl where riname[9] = '';


select * from th_right
select * from ppappmodright
select * from th_right, ppappmodright where riname = rightname


select count(riname), riname from th_right
group by 2
order by 1


update ppappmodright set description_hr = (select ridesc from th_right where ppappmodright.rightname = riname) where rightname in (select riname from th_right)

------------------------

create temp table th_app (appid integer, appname char (50))

insert into th_app
select distinct applicationid, appname from th_right, ppappmodright where riname = rightname

select * from th_app
select * from ppapplication, th_app where appid = applicationid

update ppapplication set name_hr = (select appname from th_app where ppapplication.applicationid = appid) where applicationid in (select appid from th_app)

------------------------

create temp table th_mod (appid integer, modid integer, modname char (50))

insert into th_mod
select distinct applicationid, moduleid, modname from th_right, ppappmodright where riname = rightname

delete from th_mod where modname = 'Lieferanten'

select appid, modid, modname from th_mod order by 1, 2, 3

select * from ppappmodule, th_mod where appid = applicationid and modid = moduleid

update ppappmodule set name_hr = (select modname from th_mod where ppappmodule.applicationid = appid and ppappmodule.moduleid = modid) where moduleid in (select modid from th_mod where  ppappmodule.applicationid = appid)

