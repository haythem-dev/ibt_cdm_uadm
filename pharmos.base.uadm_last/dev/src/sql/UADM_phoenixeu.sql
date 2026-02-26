create temp table mail_th (email char(50), userid integer)

insert into mail_th 
select REPLACE(mailuser, "phoenix-ag.de", "phoenixgroup.eu"), userid from ppuser where mailuser like '%phoenix-ag.de%'
select REPLACE(emailuser, "phoenix-ag.de", "phoenixgroup.eu"), userid from ppuser where emailuser like '%phoenix-ag.de%'

select * from mail_th 

--drop table mail_th 

update ppuser set mailuser = (select email from mail_th  where ppuser.userid = mail_th.userid) where userid in (select userid from mail_th)
update ppuser set emailuser = (select email from mail_th  where ppuser.userid = mail_th.userid) where userid in (select userid from mail_th)


select * from ppuser where mailuser != ''
select * from ppuser where emailuser != ''