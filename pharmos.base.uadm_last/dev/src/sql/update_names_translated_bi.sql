-------------------------------------
ppappmodright
------------------------------------
create temp table bi_translation (id_app integer, id_mod integer, id_right char (50), en char(300));
insert into bi_translation select a.applicationid, m.moduleid, r.rightname, r.description_en from ppappmodright r, ppapplication a, ppappmodule m where r.applicationid = a.applicationid and m.moduleid = r.moduleid and m.applicationid = a.applicationid and a.name LIKE 'BI-%' ;

update ppappmodright set description_fr =  (select en from bi_translation where id_app = applicationid and id_mod = moduleid and id_right = rightname) where applicationid = (select id_app from bi_translation where id_app = applicationid and  id_mod = moduleid and id_right = rightname) and moduleid = (select id_mod from bi_translation where id_mod = moduleid and id_app = applicationid and id_right = rightname) and rightname = (select id_right from bi_translation where id_mod = moduleid and id_app = applicationid and id_right = rightname) and description_fr = '';

update ppappmodright set description_hr =  (select en from bi_translation where id_app = applicationid and id_mod = moduleid and id_right = rightname) where applicationid = (select id_app from bi_translation where id_app = applicationid and  id_mod = moduleid and id_right = rightname) and moduleid = (select id_mod from bi_translation where id_mod = moduleid and id_app = applicationid and id_right = rightname) and rightname = (select id_right from bi_translation where id_mod = moduleid and id_app = applicationid and id_right = rightname) and description_hr = '';

-------------------------------------
ppappmodule
------------------------------------
create temp table bi_translation (id_app integer, id_mod integer, en char(50));
insert into bi_translation select m.applicationid, m.moduleid, m.name_en from ppappmodule m, ppapplication a where m.applicationid = a.applicationid and a.name LIKE 'BI-%';
update ppappmodule set name_fr = (select en from bi_translation where id_app = applicationid and id_mod = moduleid) where applicationid = (select id_app from bi_translation where id_app = applicationid and  id_mod = moduleid) and moduleid = (select id_mod from bi_translation where id_mod = moduleid and id_app = applicationid) and name_fr = '';
update ppappmodule set name_hr = (select en from bi_translation where id_app = applicationid and id_mod = moduleid) where applicationid = (select id_app from bi_translation where id_app = applicationid and  id_mod = moduleid) and moduleid = (select id_mod from bi_translation where id_mod = moduleid and id_app = applicationid) and name_hr = '';

-------------------------------------
ppapplication
------------------------------------
create temp table bi_translation (id integer, en char(50));
insert into bi_translation select applicationid, name_en from ppapplication where name LIKE 'BI-%';
update ppapplication set name_hr = (select en from bi_translation where id = applicationid ) where applicationid = (select id from bi_translation where id = applicationid) and name_hr = '';
update ppapplication set name_fr = (select en from bi_translation where id = applicationid ) where applicationid = (select id from bi_translation where id = applicationid) and name_fr = '';