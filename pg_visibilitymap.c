/*-------------------------------------------------------------------------
 *
 * pg_visibilitymap.c
 *  display contents of a visibility map
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/visibilitymap.h"
#include "funcapi.h"
#include "storage/bufmgr.h"

PG_MODULE_MAGIC;

/*
 * Returns visibility on a given page, according to the visibility map.
 */
PG_FUNCTION_INFO_V1(pg_is_all_visible);

/*
 * Return the page is all-visible or not, according to the visibility map.
 */
Datum
pg_is_all_visible(PG_FUNCTION_ARGS)
{
	Oid		relid = PG_GETARG_OID(0);
	int64	blkno = PG_GETARG_INT64(1);
	Relation rel;
	Buffer	vmbuffer = InvalidBuffer;
	bool	result = false;

	rel = relation_open(relid, AccessShareLock);

	if (blkno < 0 || blkno > MaxBlockNumber)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("invalid block number")));

	result = visibilitymap_test(rel, blkno, &vmbuffer);

	if (BufferIsValid(vmbuffer))
		ReleaseBuffer(vmbuffer);
	relation_close(rel, AccessShareLock);

	PG_RETURN_BOOL(result);
}
