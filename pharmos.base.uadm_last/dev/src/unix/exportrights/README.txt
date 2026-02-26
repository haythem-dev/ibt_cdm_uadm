exportrights extracts all relations between users or usergroups to applications and its region/branches and permissions.

there are several modi and command line parameters available for different purposes

1) get all permissions for usergroups only for all countries and region/branches from PROD
exportrights.exe -server ode21 -db zpcua1 -mode 1 -csvfile PROD_UADMGroup_Permission_Report.csv -go yes

2) get all users and usergroups permitted to applications for all countries and region/branches from PROD
exportrights.exe -server ode21 -db zpcua1 -mode 1 -csvfile PROD_UADM_User_Usergroup_Applications_Report.csv

3) get all users and 
exportrights.exe -server ode21 -db zpcua1 -mode 0 -csvfile PROD_UADM_User_Apps.csv -area 2003,2014,61,62,63,64,69,83,203,214