select u.surname, m.name as berichtsgruppe , a.description as bericht  from ppuser u, ppappright r, ppappmodule m, ppappmodright a where u.userid = r.userid and r.moduleid = m.moduleid and r.applicationid = m.applicationid and branchregionid = 40 and r.applicationid = 13 and r.rightname = a.rightname
union

SELECT u.surname, m.name as berichtsgruppe, mr.description as bericht
FROM ppapplication a, ppappmodule m, ppbranch br, ppuser u, ppappright r, ppappmodright mr
WHERE r.groupid IN 
(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = 13) 
and u.userid IN 
(SELECT c.userid FROM ppcorappusergroup c WHERE c.applicationid = 13 and c.groupid = r.groupid) 
and r.applicationid = a.applicationid and r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid = 40 and r.branchregionid = br.branchid and r.applicationid = 13 and mr.rightname = r.rightname
order by 1,2,3