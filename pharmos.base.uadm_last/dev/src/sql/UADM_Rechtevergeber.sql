unload to 'D:\temp\User_die_in_UADM_Rechte_vergeben_dürfen.txt' delimiter ';'
SELECT u.surname, u.forename, u.username, r.rightname
FROM  ppuser u, ppappright r
WHERE r.applicationid = 3 and r.userid = u.userid and r.moduleid = 1 and r.rightname not in ('READ', 'USRDEL')
UNION 
SELECT u.surname, u.forename, u.username, r.rightname
FROM ppuser u, ppappright r
WHERE
u.userid IN (SELECT c.userid FROM ppcorappusergroup c WHERE c.applicationid = 3 and c.groupid = r.groupid) 
and r.moduleid = 1 and r.rightname not in ('READ', 'USRDEL') and r.applicationid = 3 and r.userid = 0 
ORDER BY 1,2,3,4