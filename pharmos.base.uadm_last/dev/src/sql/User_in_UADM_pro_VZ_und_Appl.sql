select * from ppapplication;
select * from ppgroup where name like 'GI_09%'

unload to 'D:\temp\IRM_Benutzer_Neuhausen.txt' delimiter ';'
SELECT u.surname, u.forename, u.username
FROM ppuser u, ppappright a
WHERE 
a.applicationid in ( 43 ) and 
a.groupid = 0 and a.userid = u.userid 
and a.branchregionid in ( 52 )
UNION
SELECT u.surname, u.forename, u.username
FROM ppgroup g, ppappright a, ppuser u, ppcorappusergroup c
WHERE 
a.applicationid in ( 43 ) and 
a.groupid = g.groupid and a.userid = 0
and a.branchregionid in ( 52 )
and u.userid = c.userid
and g.groupid = c.groupid
and c.applicationid = a.applicationid
ORDER BY  1, 2


unload to 'D:\temp\IRM_Benutzerrechte_Neuhausen.txt' delimiter ';'
SELECT u.surname, u.forename, u.username, m.name, r.rightname, a.description
FROM ppappmodule m,  ppuser u, ppappright r, ppappmodright a
WHERE r.applicationid = 43 and r.userid = u.userid  and r.moduleid = m.moduleid 
and a.applicationid = r.applicationid and a.moduleid = r.moduleid and a.rightname = r.rightname
and m.applicationid = r.applicationid and r.branchregionid = 52 
UNION 
SELECT u.surname, u.forename, u.username, m.name, r.rightname, a.description
FROM ppappmodule m,  ppuser u, ppappright r, ppappmodright a
WHERE r.groupid IN 
(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = 43) and u.userid IN 
(SELECT c.userid FROM ppcorappusergroup c WHERE c.applicationid = 43 and c.groupid = r.groupid) 
and r.moduleid = m.moduleid and m.applicationid = r.applicationid 
and a.applicationid = r.applicationid and a.moduleid = r.moduleid and a.rightname = r.rightname
and r.branchregionid = 52
ORDER BY 1,2,3,4,5


unload to 'D:\temp\GI_User_pro_Benutzergruppe_58.txt' delimiter ';'
select name, surname, forename, username from ppuser u, ppgroup g, ppcorappusergroup c
where u.userid = c.userid and c.applicationid = 45 and c.groupid = g.groupid and g.name like 'GI_58%'
order by 1, 2


