zpcua1@ode21
zpcua2@ide21

//--- in welcher Filiale besitzt ein anzugebender Benutzer mindestens ein Recht in einer anzugebenden Anwendung---//
select distinct branchregionid from ppappright app where applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB') 
and (userid in (select userid from ppuser where username = 't.hoerath') 
OR groupid in (select groupid from ppcorappusergroup where applicationid = app.applicationid and userid = (select userid from ppuser where username = 't.hoerath') ))
order by 1

//--- welche Benutzer haben mindestens ein Recht (Einzel oder Gruppe) in einer anzugebenden Anwendung und einem anzugebenden VZ ---//
select username, surname, forename from ppuser where userid in (select userid from ppappright where applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB') 
and branchregionid = 5 )
UNION select username, surname, forename from ppuser where userid in (select userid from ppcorappusergroup where applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB') 
and groupid in (select groupid from ppappright where applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB')  
and branchregionid = 5 ))
order by surname

//--- Einzelrechte eines anzugebenden Benutzers für eine anzugebende Filiale in einer anzugebenden Applikation ---//
select mod.name as modname, ar.rightname from ppappright ar, ppappmodule mod where ar.applicationid = mod.applicationid and ar.moduleid = mod.moduleid and ar.applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB') 
and userid in (select userid from ppuser where username = 't.hoerath') 
and ar.branchregionid = 5
order by 1, 2

//--- Gruppenrechte eines anzugebenden Benutzers für eine anzugebende Filiale in einer anzugebenden Applikation ---//
select mod.name as modname, ar.rightname from ppappright ar, ppappmodule mod where ar.applicationid = mod.applicationid and ar.moduleid = mod.moduleid and ar.applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB') 
and groupid in (select groupid from ppcorappusergroup where applicationid = ar.applicationid and userid in (select userid from ppuser where username = 't.hoerath') )
and ar.branchregionid = 5
order by 1, 2

//--- Gruppen, über die ein anzugebender Benutzers für eine anzugebende Filiale in einer anzugebenden Applikation Rechte hat ---//
select name from ppgroup where groupid in 
(select groupid from ppcorappusergroup g where 
--groupid in (select groupid from ppappright where 
--branchregionid = 5 and 
-- applicationid =  g.applicationid ) and
-- applicationid = (select applicationid from ppapplication where name = 'BI-BETRIEB') and
userid = (select userid from ppuser where username = 'd.minges') )
order by 1


select * from ppuser where surname = 'Minges'