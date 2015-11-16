/* contrib/pg_visibilitymap/pg_visibilitymap--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_visibilitymap" to load this file. \quit

CREATE FUNCTION pg_is_all_visible(regclass, bigint)
RETURNS bool
AS 'MODULE_PATHNAME', 'pg_is_all_visible'
LANGUAGE C STRICT;

CREATE FUNCTION
  pg_visibilitymap(rel regclass, blkno OUT bigint, all_visible OUT bool)
RETURNS SETOF RECORD
AS $$
  SELECT blkno, pg_is_all_visible($1, blkno) AS all_visible 
  FROM generate_series(0, pg_relation_size($1) / current_setting('block_size')::bigint - 1) AS blkno;
$$
LANGUAGE SQL;

-- Don't want these to be available to public.
REVOKE ALL ON FUNCTION pg_is_all_visible(regclass, bigint) FROM PUBLIC;
REVOKE ALL ON FUNCTION pg_visibilitymap(rel regclass) FROM PUBLIC;