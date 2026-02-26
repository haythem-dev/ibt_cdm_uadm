
unload to 'C:\temp\uadm_appl_user_54.txt' delimiter ';'
select a.name as APPL, u.username, u.surname, u.forename, ar.branchregionid as FILIALE
 from ppappright ar, ppuser u, ppapplication a
where u.userid = ar.userid and ar.branchregionid = 54 
and a.applicationid = ar.applicationid
union
select a.name as APPL, u.username, u.surname, u.forename, ar.branchregionid as FILIALE
 from ppappright ar, ppuser u, ppapplication a, ppcorappusergroup g 
where g.applicationid = ar.applicationid and ar.groupid = g.groupid and g.userid = u.userid 
and ar.branchregionid = 54 
and a.applicationid = ar.applicationid
order by 1, 2