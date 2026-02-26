1/ Initial kdauftrag table state (script not launched)
before launching script:
this query:
SELECT kdauftragstat 
FROM kdauftrag;

gives:

0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
##############################################################################################################
2/ Script tpldauftr launched:
here console:

h.benabdelaziz@zdev21:/software/wss/de/ksc/load/proc # ./tpldauftr.sh -start -fil 5
=====================================================================================
SH: START /software/wss/de/ksc/load/proc/printStartStop -start -fil 5
=====================================================================================
O 00 14.08.25 09:25:40 pid=17891846 ================================================
O 00 14.08.25 09:25:40 pid=17891846 START
O 00 14.08.25 09:25:40 pid=17891846 ================================================
ES WURDEN NICHT ALLE AUFTRAEGE ABGESCHLOSSEN!
start_process:
        RC:<0>: --> OK:    Application started.
        Parameter: </software/wss/de/ksc/load/bin/tpldauftr.bin --business-day-mode enabled -fil 5 --business-day-mode enabled -db ode21@zdev21_shm -del 35>;
O 00 14.08.25 09:26:03 pid=17891846 ================================================
O 00 14.08.25 09:26:03 pid=17891846 STOP with Return-Code: <<0>>.
O 00 14.08.25 09:26:03 pid=17891846 ================================================
=====================================================================================
SH: STOP /software/wss/de/ksc/load/proc/printStartStop -start -fil 5
=====================================================================================
h.benabdelaziz@zdev21:/software/wss/de/ksc/load/proc #
##############################################################################################################
and here log:

log tpldauftr.log:
H 05 14.08.25 09:25:40 pid=37093632 tpldauft Main gestartet mit Tagen:35 fuer DB: ode21@zdev21_shm->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898572 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898591 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898714 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898716 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898719 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898735 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898784 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898803 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898814 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898852 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898873 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898887 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898972 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:898996 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:899641 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:899642 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:899769 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:899792 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft PruefeAuftrag AuftragNr:900030 wurde nicht abgeschlossen!->0/0
F 05 14.08.25 09:25:40 pid=37093632 tpldauft Main es wurden nicht alle Auftraege abgeschlossen!->0/0
H 05 14.08.25 09:25:41 pid=37093632 tpldauft DeleteOld Aelter 10.07.2025(Kopf/Debtrel/Rechart/Discacc/Zus/Pos/Rab/Charge/PosRechart/PosPromo/Refund/Xml/PosEreignis/PosTour/Bem/Erg/Dis/Nal/Vb/Vbp/StE/Res/Werte/Change/Prolink/VBPosRab/OrderRemarks/OrderPositionDiscount/NewIBT(OrderRelationNo)/CscOrderFlags/ToteInfo/CscOrderPosSplittInfo/CscOrderIBTPickingData/DocumentPrintInfo:0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/32/0/0/1680/0/0/0/0/0/0/0/0/0/0/0/0/0/1308/0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Kdtourdruck geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Fkwarteschl geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPos  geloescht: 8275->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosRab  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosCharge  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosChange  geloescht: 222->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosRechart  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosPromo  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosRefund  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosXml  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosEreignis  geloescht: 8355->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPosTour  geloescht: 790->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragPartnerFunction  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Auftrag     geloescht: 356->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragDebtrel     geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragZus  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragProlink  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragErgs geloescht: 3161->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragBem  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragWerte  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragRechart  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragDiscacc  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main AuftragDisp geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main KoAuftrag   geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Koeinheit   geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Verbund     geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Verbundpos  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Verbundposrab  geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Verbundpos ohne Header geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Costomerorderno geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Kdauftraginfo geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Orderremarks geloescht: 32088->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main OrderPositionDiscount geloescht: 4923->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main OrderRelationNo geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main CscOrderFlags geloescht: 356->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main CscOrderPosSplittInfo geloescht: 76->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main CscOrderIBTPickingData geloescht: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Nummer      geaendert: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main Auftr?ge reaktiviert: 0->0/0
H 05 14.08.25 09:26:03 pid=37093632 tpldauft Main beendet->0/0

###########################################################################
command informix launched again:

SELECT kdauftragstat 
FROM kdauftrag;

and result was:
---------------
0000000001000000
0000000001000000
0000000001000000
0000000001000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000000
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001
0000100000000001