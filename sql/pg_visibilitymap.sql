create extension pg_visibilitymap;

create table t1 as select * from generate_series(1,1000) as p;
\d t1
vacuum t1;
select relpages from pg_class where relname = 't1';

select * from pg_visibilitymap('t1');

delete from t1 where p < 500;

select * from pg_visibilitymap('t1');

vacuum t1;

select * from pg_visibilitymap('t1');
