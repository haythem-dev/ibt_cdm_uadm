unload to 'D:\temp\rights_05.txt'
select app.name as appl, u.surname, m.name as modul, a.rightname from ppuser u, ppappright r, ppappmodule m, ppappmodright a, ppapplication app where u.userid = r.userid and r.moduleid = m.moduleid and r.applicationid = m.applicationid and branchregionid = 5 and r.applicationid in(6,17,24,25,28,35,37,39) and r.rightname = a.rightname and r.applicationid = app.applicationid

union

SELECT app.name as appl, u.surname, m.name as modul, mr.rightname
FROM ppapplication a, ppappmodule m, ppbranch br, ppuser u, ppappright r, ppappmodright mr, ppapplication app
WHERE r.groupid IN 
(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid in(6,17,24,25,28,35,37,39)) 
and u.userid IN 
(SELECT c.userid FROM ppcorappusergroup c WHERE c.applicationid in (6,17,24,25,28,35,37,39) and c.groupid = r.groupid) 
and r.applicationid = a.applicationid and r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid = 5 and r.branchregionid = br.branchid and r.applicationid in(6,17,24,25,28,35,37,39) and r.applicationid = app.applicationid and mr.rightname = r.rightname
order by 1,2,3


select * from ppappmodright