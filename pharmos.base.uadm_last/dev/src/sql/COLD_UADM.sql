--- ppappmodule: neues Module "COLD" unter BI-Betrieb anlegen
select * from ppapplication
BI-Betrieb: 	13
Cold: 		18

create temp table th_cold_rights (appname char(20), modname char (10), riname char(40), ridescde varchar(254), ridescen varchar(254));
drop table th_cold_rights 

load from 'D:\temp\ZBN_ColdRights_20090407.txt' delimiter ';'
insert into th_cold_rights;

select * from th_cold_rights 

insert into ppappmodright (applicationid, moduleid, rightname, description, description_en, description_hr, description_fr)
select 13, 18, riname, ridescde, ridescen, ridescen, ridescen from th_cold_rights

select * from ppappmodright where applicationid = 13 and moduleid = 18


create temp table th_cold_user_groups (riname char(40), branch smallint, ntuser char(100), lang char(3), usrgrp char(30));

drop table th_cold_user_groups 

load from 'D:\temp\ZBN_UserRights_20090407.txt' delimiter ';'
insert into th_cold_user_groups;

select * from th_cold_user_groups 

insert into ppgroup (name)
select distinct 'COLD_' || usrgrp from th_cold_user_groups;

select * from ppgroup

insert into ppappright (rightname, branchregionid, groupid, userid, applicationid, moduleid)
select distinct riname, branch, groupid, 0, 13, 18
from th_cold_user_groups t, ppgroup g
where 'COLD_' || t.usrgrp = g.name

select * from ppappright where applicationid = 13 and moduleid = 18

unload to 'D:\temp\cold_user_not_known.txt'
select distinct ntuser from th_cold_user_groups where lower(ntuser) NOT IN (select ntuser from ppuser);
--> diese User neu anlegen !?

select distinct u.surname, u.forename, u.ntuser, t.ntuser from ppuser u, th_cold_user_groups t
where lower(t.ntuser) NOT IN (select ntuser from ppuser) and 
lower(t.ntuser) like '%' || lower(u.surname) || '%' 

create temp table th_cold_user (ntuser char(100), usrid integer);

insert into th_cold_user 
select distinct lower(t.ntuser), u.userid from th_cold_user_groups t, ppuser u
where lower(t.ntuser) = u.ntuser

select * from th_cold_user 


insert into ppcorappusergroup (userid, groupid, applicationid)
select distinct u.usrid, g.groupid, 13 from
th_cold_user_groups t, th_cold_user u, ppgroup g
where g.name = 'COLD_' || t.usrgrp and lower(t.ntuser) = u.ntuser

select * from ppcorappusergroup where applicationid = 13
