unload to 'C:\prj\uadm\bibetrieb_rechte.csv' delimiter ';' 
select m.name as berichtsgruppe , r.rightname as berichtID, r.description as berichtsbeschreibung 
from ppappmodright r, ppappmodule m where r.applicationid = 13 and r.applicationid = m.applicationid and m.moduleid = r.moduleid order by m.name, r.rightname

select count(*) from ppappmodright where applicationid = 13


select * from ppappmodule where applicationid = 13


select rightname, username, surname, from ppappright a, ppuser u where u.userid = a.userid and moduleid = 2 and branchregionid = 13 and applicationid = 13 and (rightname = 'BCC001' or rightname = 'BCC002' or rightname = 'BCG001') union

select rightname, username, surname from ppappright a, ppuser u, ppcorappusergroup g where g.applicationid = a.applicationid and a.groupid = g.groupid and g.userid = u.userid and moduleid = 8 and branchregionid = 13 and a.applicationid = 13 and (rightname = 'BCC001' or rightname = 'BCC002' or rightname = 'BCG001') order by 1,3


select rightname, username, ntuser, surname, branchregionid from ppappright a, ppuser u where u.userid = a.userid and moduleid = 2  and applicationid = 13 and rightname = 'BKB007'
union
select rightname, username, ntuser, surname, branchregionid from ppappright a, ppuser u, ppcorappusergroup g where g.applicationid = a.applicationid and a.groupid = g.groupid and g.userid = u.userid and moduleid = 2 and a.applicationid = 13 and rightname = 'BKB007' order by 1,3



select rightname, ntuser, surname from ppappright a, ppuser u where u.userid = a.userid  and applicationid in (select applicationid from ppapplication where name like 'BI-%' ) and rightname = 'BCC001'  and u.ntuser = 'prz_fu\r.kiefert'
union
select rightname, ntuser, surname from ppappright a, ppuser u, ppcorappusergroup g where g.applicationid = a.applicationid and a.groupid = g.groupid and g.userid = u.userid and a.applicationid  in (select applicationid from ppapplication where name like 'BI-%' )  and rightname = 'BCC001' and u.ntuser = 'prz_fu\r.kiefert' order by 1,3