create temp table thth (bericht char(300), klasse integer);

load from 'D:\prj\uadm\berichtsklassen.csv' delimiter ';'
insert into thth

create temp table th1 (bericht varchar(10), klasse integer);

insert into th1 (bericht, klasse)
select bericht[1,6], klasse from thth where bericht [7] = ' '
select bericht[1,7], klasse from thth where bericht [8] = ' '
select bericht[1,8], klasse from thth where bericht [9] = ' ';

select * from th1 where bericht not in (select rightname from ppappmodright)
select * from ppappmodright where rightname not in (select bericht from th1) and applicationid in (select applicationid from ppapplication where name like 'BI-%');

update ppappmodright set category = 0 where applicationid in (select applicationid from ppapplication where name like 'BI-%');
update ppappmodright set category = 1
where rightname in (select bericht from th1 where klasse = 1)
update ppappmodright set category = 2
where rightname in (select bericht from th1 where klasse = 2)

select * from ppappmodright where applicationid in (select applicationid from ppapplication where name like 'BI-%') and category = 0



select * from ppappmodright where applicationid in (select applicationid from ppapplication where name like 'BI-%') order by rightname
select * from thth where bericht not in (select description from ppappmodright)
select * from thth
