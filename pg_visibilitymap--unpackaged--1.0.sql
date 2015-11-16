-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_visibilitymap FROM unpackaged" to load this file. \quit

ALTER EXTENSION pg_visibilitymap ADD function pg_is_all_visible(regclass, bigint);
ALTER EXTENSION pg_visibilitymap ADD function pg_visibilitymap(regclass);