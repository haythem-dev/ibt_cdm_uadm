unload to 'C:\temp\ppuser.txt' select ntuser, surname from ppuser where ntuser != ''

create temp table temp_userinformix (ntuser char(50), surname char(50));
create temp table temp_usersql (ntuser char(50), surname char(50));
create temp table temp_countryrsql (ntuser char(50), surname char(50));

load from 'C:\temp\ppuser.txt' insert into temp_userinformix 
select * from temp_userinformix 

load from 'C:\temp\ibi_user.csv' delimiter ';' insert into temp_usersql 
select * from temp_usersql 
delete from temp_usersql 

load from 'C:\temp\ibi_usercountryrights.csv' delimiter ';' insert into temp_countryrsql
select * from temp_countryrsql

select * from temp_userinformix where ntuser not in (select ntuser from temp_usersql )

select * from temp_userinformix where ntuser not in (select ntuser from temp_countryrsql )