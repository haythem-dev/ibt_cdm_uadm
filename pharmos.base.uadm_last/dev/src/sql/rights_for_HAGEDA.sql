select ntuser from ppuser where ntuser like '%alxmun.dom%' order by 1 

select * from ppapplication where name like 'BI%'

select * from ppgroup where name like 'BI%' order by groupid


select count (*) from ppappright where branchregionid = 21 and applicationid = 13 and userid = 0;
select count (*) from ppappright where branchregionid = 21 and applicationid = 13 and groupid = 0


select * from ppgroup where groupid in ( 
select distinct groupid from ppappright where branchregionid = 21 and applicationid = 13 and userid = 0);


select userid, username, surname, ntuser from ppuser where userid in ( 
select distinct userid from ppappright where branchregionid = 21 and applicationid = 36 and groupid = 0 and userid in (select userid from ppuser where ntuser not like '%alxmun.dom%' ) and userid != 432);


select count ( *) from ppappright where applicationid = 13 and groupid = 17


delete from ppappright where branchregionid = 21 and applicationid = 19 and groupid = 0 and userid in (select userid from ppuser where ntuser not like '%alxmun.dom%' ) and userid != 432