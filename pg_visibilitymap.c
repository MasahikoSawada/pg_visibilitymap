/*-------------------------------------------------------------------------
 *
 * pg_visibilitymap.c
 *  display contents of a visibility map
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/visibilitymap.h"
#include "catalog/pg_class.h"
#include "funcapi.h"
#include "storage/bufmgr.h"
#include "utils/rel.h"

PG_MODULE_MAGIC;

/*
 * Returns visibility on a given page, according to the visibility map.
 */
PG_FUNCTION_INFO_V1(pg_is_all_visible);

/*
 * The function prototypes are created as a part of PG_FUNCTION_INFO_V1
 * macro since 9.4, and hence the declaration of the function prototypes
 * here is necessary only for 9.3 or before.
 */
#if PG_VERSION_NUM < 90400
Datum	pg_is_all_visible(PG_FUNCTION_ARGS);
#endif


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

	/* Check for relation type */
	if (!(rel->rd_rel->relkind == RELKIND_RELATION ||
		  rel->rd_rel->relkind == RELKIND_MATVIEW))
		ereport(ERROR,
				(errcode(ERRCODE_WRONG_OBJECT_TYPE),
				 errmsg("\"%s\" is not a table or materialized view",
						RelationGetRelationName(rel))));

	/* Check for the number of blocks */
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
