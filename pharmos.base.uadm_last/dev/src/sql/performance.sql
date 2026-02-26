SELECT a.name as Bereich, m.name as Berichtsgruppe, r.description as Bericht 
FROM ppapplication a, ppappmodright r, ppappmodule m, ppappright ar 
WHERE ar.applicationid 
IN (SELECT a.applicationid FROM ppapplication a 
WHERE a.name LIKE 'BI-%') and ar.applicationid = a.applicationid and m.applicationid = ar.applicationid and r.applicationid = ar.applicationid and m.moduleid = r.moduleid and r.rightname = ar.rightname and EXISTS (SELECT 1 FROM ppuser u WHERE u.ntuser='PRZ_FU\66aeguen' and u.userid = ar.userid) 
UNION SELECT a.name, m.name, r.description 
FROM ppapplication a, ppappmodright r, ppappmodule m, ppappright ar 
WHERE ar.applicationid IN (SELECT a.applicationid FROM ppapplication a WHERE a.name LIKE 'BI-%') and ar.applicationid = a.applicationid and m.applicationid = ar.applicationid and r.applicationid = ar.applicationid and m.moduleid = r.moduleid and r.rightname = ar.rightname and ar.groupid IN (SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = ar.applicationid and c.userid IN (SELECT u.userid FROM ppuser u WHERE u.ntuser='PRZ_FU\66aeguen'));


select count(*) from ppuser


/////////////////////////////////
prod
///////////////////////////////
SELECT a.name as Bereich, m.name as Berichtsgruppe, r.description as Bericht 
FROM ppapplication a, ppappmodright r, ppappmodule m, ppappright ar 
WHERE ar.applicationid 
IN (SELECT a.applicationid FROM ppapplication a 
WHERE a.name LIKE 'BI-%') and ar.applicationid = a.applicationid and m.applicationid = ar.applicationid and r.applicationid = ar.applicationid and m.moduleid = r.moduleid and r.rightname = ar.rightname and EXISTS (SELECT 1 FROM ppuser u WHERE u.ntuser='prz_fu\66aeguen' and u.userid = ar.userid) 
UNION SELECT a.name, m.name, r.description 
FROM ppapplication a, ppappmodright r, ppappmodule m, ppappright ar 
WHERE ar.applicationid IN (SELECT a.applicationid FROM ppapplication a WHERE a.name LIKE 'BI-%') and ar.applicationid = a.applicationid and m.applicationid = ar.applicationid and r.applicationid = ar.applicationid and m.moduleid = r.moduleid and r.rightname = ar.rightname and ar.groupid IN (SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = ar.applicationid and c.userid IN (SELECT u.userid FROM ppuser u WHERE u.ntuser='prz_fu\66aeguen'));




SELECT distinct r.branchregionid FROM PPAPPRIGHT r, PPAPPLICATION a
where
r.rightname= 'BLA001' and
r.applicationid= a.applicationid and
r.userid IN (SELECT u.userid FROM PPUSER u WHERE lower( u.ntuser) = lower( 'PRZ_FU\66aeguen') )
UNION
SELECT distinct r.branchregionid FROM PPAPPRIGHT r, PPAPPLICATION a
where
r.rightname= 'BLA001' and
r.applicationid= a.applicationid and
r.groupid IN (SELECT c.groupid FROM PPCORAPPUSERGROUP c WHERE c.applicationid = r.applicationid and c.userid IN
(SELECT u.userid FROM PPUSER u WHERE lower( u.ntuser) = lower( 'PRZ_FU\66aeguen' )) );


SELECT r.branchregionid FROM PPAPPRIGHT r, PPAPPLICATION a
where
r.rightname= 'BLA001' and
r.applicationid= a.applicationid and
r.userid IN (SELECT u.userid FROM PPUSER u WHERE u.ntuser = 'prz_fu\66aeguen' )
UNION
SELECT r.branchregionid FROM PPAPPRIGHT r, PPAPPLICATION a
where
r.rightname= 'BLA001' and
r.applicationid= a.applicationid and
r.groupid IN (SELECT c.groupid FROM PPCORAPPUSERGROUP c WHERE c.applicationid = r.applicationid and c.userid IN
(SELECT u.userid FROM PPUSER u WHERE u.ntuser = 'prz_fu\66aeguen' ) )




select * from ppappright;
select * from ppuser;
select * from ppapplication;
select * from ppappmodright where applicationid = 13;
