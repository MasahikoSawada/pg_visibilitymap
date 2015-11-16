# pg_visibilitymap
The diagnostic functions for VisibilityMap (VM).

# Functions

## pg_is_all_visible(relname regclass, blkno bigint)
This function shows the value recoreded in the visibility map for given page.

## pg_visibilitymap(relname regclass)
This function shows the value recoreded in the visibility map for all page of relation.

# Installation

```
$ cd pg_visibilitymap
$ make USE_PGXS=1
# make USE_PGXS=1 install
$ psql
=# CREATE EXTENSION pg_visibilitymap;
CREATE EXTENSION
```
