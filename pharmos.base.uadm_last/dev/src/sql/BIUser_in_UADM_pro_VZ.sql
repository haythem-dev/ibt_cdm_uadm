unload to 'D:\temp\rights_uadm_bg.txt' delimiter ';'
SELECT distinct u.username, u.surname, u.forename
--, u.ntuser
, app.name
--, m.name, a.rightname
FROM ppuser u, ppappright a
, ppapplication app
--, ppappmodule m
WHERE 
--a.applicationid in ( select applicationid from ppapplication where name like 'BI-%' ) and 
a.groupid = 0 and a.userid = u.userid 
and a.branchregionid in (22, 23, 24, 25, 26, 28, 208, 2008)
and a.applicationid = app.applicationid 
--and a.applicationid = m.applicationid and m.moduleid = a.moduleid

UNION

SELECT distinct u.username, u.surname, u.forename
--, u.ntuser
, app.name
--, m.name, a.rightname
FROM ppgroup g, ppappright a, ppuser u, ppcorappusergroup c
, ppapplication app
--, ppappmodule m
WHERE 
--a.applicationid in ( select applicationid from ppapplication where name like 'BI-%' ) and 
a.groupid = g.groupid and a.userid = 0
and a.branchregionid in (22, 23, 24, 25, 26, 28, 208, 2008)
and u.userid = c.userid
and g.groupid = c.groupid
and c.applicationid = a.applicationid
and a.applicationid = app.applicationid 
--and a.applicationid = m.applicationid and m.moduleid = a.moduleid

ORDER BY 4,  2, 3




select distinct a.applicationid, a.name, m.name, b.name
from ppapplication a, ppappright r, ppappmodule m, ppbranch b
where a.applicationid = r.applicationid
and r.groupid = 66
and a.applicationid = 13 
and a.applicationid = m.applicationid
and b.branchid = r.branchregionid
and b.branchid in (5,6)


select distinct groupid, m.name from ppappright r, ppappmodule m
where r.applicationid = 13
and userid = 0
and branchregionid = 12
and r.moduleid = m.moduleid
and r.groupid = 69
and m.applicationid = r.applicationid



select * from ppappright module where applicationid = 13



