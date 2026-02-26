SELECT u.surname, u.forename, u.ntuser
FROM ppuser u, ppappright a 
WHERE a.applicationid in ( select applicationid from ppapplication where name like 'BI-%' ) and a.groupid = 0 and a.userid = u.userid 

UNION 

SELECT u.surname, u.forename, u.ntuser
FROM ppuser u 
WHERE EXISTS ( SELECT 1 FROM ppcorappusergroup c WHERE c.applicationid in ( select applicationid from ppapplication where name like 'BI-%' ) and c.userid = u.userid )

ORDER BY 1
