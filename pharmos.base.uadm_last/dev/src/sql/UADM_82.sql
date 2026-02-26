/////  selektieren von Berichtsgruppen (Modulen) aller Benutzer pro VZ und Applikation

select a.name as BEREICH, m.name as BERICHTSGRUPPE, u.username, u.surname, u.forename, ar.branchregionid as FILIALE
 from ppappright ar, ppuser u, ppappmodule m, ppapplication a
where u.userid = ar.userid and ar.branchregionid = 82 and 
ar.applicationid IN (select applicationid from ppapplication where name like 'BI-%')
and m.moduleid = ar.moduleid
and m.applicationid = ar.applicationid
and a.applicationid = ar.applicationid
union
select a.name as BEREICH, m.name as BERICHTSGRUPPE, u.username, u.surname, u.forename, ar.branchregionid as FILIALE
 from ppappright ar, ppuser u, ppappmodule m, ppapplication a, ppcorappusergroup g 
where g.applicationid = ar.applicationid and ar.groupid = g.groupid and g.userid = u.userid 
 and ar.branchregionid = 82 and 
ar.applicationid IN (select applicationid from ppapplication where name like 'BI-%')
and m.moduleid = ar.moduleid
and m.applicationid = ar.applicationid
and a.applicationid = ar.applicationid