select * from ppappright r , ppgroup g, ppbranch b1, ppbranch b2 
where g.name like '%_' || lpad(b2.branchid, 2, '0' ) || '_%' 
and applicationid = 45 and branchregionid != b2.branchid
and g.groupid = r.groupid
and b1.branchid = r.branchregionid
and b2.branchid < 100