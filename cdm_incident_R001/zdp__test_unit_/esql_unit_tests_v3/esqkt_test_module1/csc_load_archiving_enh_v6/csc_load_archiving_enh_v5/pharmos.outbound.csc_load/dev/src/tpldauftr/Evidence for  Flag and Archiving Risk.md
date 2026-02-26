## Evidence for `erfasst` Flag and Archiving Risk

My previous analysis concluded that while the `erfasst` flag (`kdauftragstat[5] == '1'`) prevents archiving of orders in that specific state, it does not eliminate the risk of archiving other types of open orders, especially in `-h24` mode. Here's the evidence from the `tpldauftr_before_fix_utf8.cxx` script to support this conclusion:

### 1. `kdauftragstat[5] != '1'` Condition

The `grep` command output clearly shows that `kdauftragstat[5] != '1'` is a condition used in the SQL queries for selecting orders to be archived. This means orders where `kdauftragstat[5]` is indeed `'1'` are *excluded* from the archiving process. This confirms the user's assertion that this flag blocks archiving.

```c
4675: strcat( str, " and kdauftragstat[5] != '1'" ); /* Erfasst */
4927: strcat( str, " and kdauftragstat[5] != '1'" ); /* Erfasst */
```

These lines are found within functions responsible for building the SQL `WHERE` clauses for order selection, likely `PrepareAll` or similar archiving preparation functions. The comment `/* Erfasst */` further confirms that `kdauftragstat[5] == '1'` signifies an 


entered/recorded state, and orders in this state are explicitly excluded from archiving.

### 2. H24 Mode Bypasses Pre-Archiving Checks

The `main` function shows conditional execution of `PruefeFaktur`, `PruefeAuftrag`, and `PruefeVBAuftrag` functions based on the `i24` flag. The `i24` flag is set to `YES` when the `-h24` command-line argument is provided.

```c
if(i24 == NO)
{
	if ( PruefeFaktur(iFilialNr) != OK )
	{
		fprintf( stderr, "ES WURDEN NICHT ALLE AUFTRAEGE FAKTURIERT!\n" );
		PrintMsg( fun, "es wurden nicht alle Auftraege fakturiert!", FEHLER );
		/* return;  Nach Aussage von Herrn Sendlbeck muss geloescht trotzdem  */
		/* geloescht werden ! */
	}
}

// ... similar blocks for PruefeAuftrag and PruefeVBAuftrag
```

This code snippet demonstrates that when `i24` (h24 mode) is active, these crucial `Pruefe` (check) functions are skipped. These functions are likely responsible for verifying if orders are fully processed, fakturiert (invoiced), or otherwise completed before archiving. Bypassing these checks in h24 mode significantly increases the risk of archiving orders that are still logically "open" from a business perspective, even if they are not in the `erfasst` state.

### 3. Other `kdauftragstat` Conditions

The `grep` output also shows other conditions related to `kdauftragstat`:

```c
615:		where kdauftragstat[1,4] != "0000"
689:		strcat( str, " and (kdauftragstat[9] == '1' " );
690:		strcat( str, " or kdauftragstat[10] == '1') " );
4676:	strcat( str, " and kdauftragstat[10] != '1'" ); /* Storniert */
4928:	strcat( str, " and kdauftragstat[10] != '1'" ); /* Storniert */
```

These lines indicate that the archiving logic also considers `kdauftragstat[9]`, `kdauftragstat[10]`, and `kdauftragstat[1,4]`. Specifically, `kdauftragstat[10] != '1'` means that *cancelled* orders are also excluded from archiving. However, the presence of these other flags reinforces the idea that the system relies on a specific set of status flags to determine archivable orders. If an order's "open" status is not explicitly covered by these exclusion flags (like `erfasst` or `storniert`), it remains vulnerable to archiving.

### Conclusion with Evidence

While `kdauftragstat[5] == '1'` effectively prevents archiving of orders marked as `erfasst`, the script's overall logic, especially in `-h24` mode, still presents a risk of archiving genuinely open orders. The `-h24` flag bypasses critical pre-archiving checks, and the archiving process relies on a limited set of explicit exclusion flags. Any order status that falls outside these specific exclusions, but is still considered "open" by business rules, could be archived prematurely. Therefore, the conclusion that the risk of archiving open orders still exists is supported by the code's conditional logic and the limited scope of its archiving safeguards.

