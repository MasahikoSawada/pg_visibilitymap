create extension pg_visibilitymap;

CREATE TABLE t1 AS SELECT * FROM generate_series(1,1000) AS p;
\d t1
VACUUM t1;
SELECT relpages FROM pg_class WHERE relname = 't1';
SELECT * FROM pg_visibilitymap('t1');
DELETE FROM t1 WHERE p < 500;
SELECT * FROM pg_visibilitymap('t1');
VACUUM t1;
SELECT * FROM pg_visibilitymap('t1');
DROP TABLE t1;

--
-- Errors
--
CREATE TABLE t2 (col int primary key);
INSERT INTO t2 SELECT generate_series(1,100);
CREATE VIEW t2_view AS (SELECT * FROM t2);
CREATE MATERIALIZED VIEW t2_matview AS (SELECT * FROM t2);
SELECT pg_is_all_visible('t2', 0);
SELECT pg_is_all_visible('t2_view', 0);
SELECT pg_is_all_visible('t2_matview', 0);
DROP TABLE t2 CASCADE;