unload to 'C:\prj\uadm\user_to_compare_pps2.txt' select userid, username, password, activ, changepw from ppuser

create temp table user_to_compare_pos1 (userid integer, username char(50), password char(30), activ smallint, changepw smallint)
create temp table user_to_compare_pps2 (userid integer, username char(50), password char(30), activ smallint, changepw smallint)

insert into user_to_compare_pps2 select  userid, username, password, activ, changepw from ppuser
load from 'C:\prj\uadm\user_to_compare_pps2.txt' insert into user_to_compare_pos1

select * from user_to_compare_pos1 t, ppuser p where t.userid = p.userid and t.activ != p.activ
select * from user_to_compare_pos1 t, ppuser p where t.userid = p.userid and t.changepw != p.changepw and  t.password = p.password 
select * from user_to_compare_pos1 t, ppuser p where t.userid = p.userid and t.password != p.password 

update ppuser set password = (select t.password from user_to_compare_pos1 t where ppuser.userid = t.userid ) 