select * from ppgroup where name like '%BI_Betrieb_Amedis_LR_12%'

unload to 'D:\temp\BI_Betrieb_Amedis_LR_12.txt'
select m.name, r.rightname from ppappright r, ppappmodule m where groupid = 1031 and m.applicationid = r.applicationid and m.moduleid = r.moduleid
