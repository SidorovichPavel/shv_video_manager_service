-- Database generated with pgModeler (PostgreSQL Database Modeler).
-- pgModeler version: 1.1.3
-- PostgreSQL version: 14.0
-- Project Site: pgmodeler.io
-- Model Author: ---
-- object: svh | type: ROLE --
-- DROP ROLE IF EXISTS svh;
CREATE ROLE svh WITH 
	INHERIT
	LOGIN
	 PASSWORD 'swh123qwe';
-- ddl-end --


-- Database creation must be performed outside a multi lined SQL file. 
-- These commands were put in this file only as a convenience.
-- 
-- object: svh_video_service_db | type: DATABASE --
-- DROP DATABASE IF EXISTS svh_video_service_db;
CREATE DATABASE svh_video_service_db;
-- ddl-end --


-- object: svh_video_schema | type: SCHEMA --
-- DROP SCHEMA IF EXISTS svh_video_schema CASCADE;
CREATE SCHEMA svh_video_schema;
-- ddl-end --
ALTER SCHEMA svh_video_schema OWNER TO svh;
-- ddl-end --

SET search_path TO pg_catalog,public,svh_video_schema;
-- ddl-end --

-- object: svh_video_schema.videos | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.videos CASCADE;
CREATE TABLE svh_video_schema.videos (
	id bigserial NOT NULL,
	created_at timestamptz NOT NULL,
	owner uuid NOT NULL,
	title text NOT NULL,
	descrpt text NOT NULL,
	age_rating text,
	CONSTRAINT videos_pk PRIMARY KEY (id)
);
-- ddl-end --
ALTER TABLE svh_video_schema.videos OWNER TO svh;
-- ddl-end --

-- object: svh_video_schema.tags | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.tags CASCADE;
CREATE TABLE svh_video_schema.tags (
	id bigserial NOT NULL,
	title text NOT NULL,
	CONSTRAINT tags_pk PRIMARY KEY (id)
);
-- ddl-end --
ALTER TABLE svh_video_schema.tags OWNER TO svh;
-- ddl-end --

-- object: svh_video_schema.tags_to_videos | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.tags_to_videos CASCADE;
CREATE TABLE svh_video_schema.tags_to_videos (
	id_videos bigint,
	id_tags bigint

);
-- ddl-end --
ALTER TABLE svh_video_schema.tags_to_videos OWNER TO svh;
-- ddl-end --

-- object: videos_fk | type: CONSTRAINT --
-- ALTER TABLE svh_video_schema.tags_to_videos DROP CONSTRAINT IF EXISTS videos_fk CASCADE;
ALTER TABLE svh_video_schema.tags_to_videos ADD CONSTRAINT videos_fk FOREIGN KEY (id_videos)
REFERENCES svh_video_schema.videos (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: tags_fk | type: CONSTRAINT --
-- ALTER TABLE svh_video_schema.tags_to_videos DROP CONSTRAINT IF EXISTS tags_fk CASCADE;
ALTER TABLE svh_video_schema.tags_to_videos ADD CONSTRAINT tags_fk FOREIGN KEY (id_tags)
REFERENCES svh_video_schema.tags (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: svh_video_schema.categories | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.categories CASCADE;
CREATE TABLE svh_video_schema.categories (
	id serial NOT NULL,
	title text NOT NULL,
	CONSTRAINT categories_pk PRIMARY KEY (id)
);
-- ddl-end --
ALTER TABLE svh_video_schema.categories OWNER TO svh;
-- ddl-end --

-- object: svh_video_schema.categories_to_videos | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.categories_to_videos CASCADE;
CREATE TABLE svh_video_schema.categories_to_videos (
	id_videos bigint,
	id_categories integer

);
-- ddl-end --
ALTER TABLE svh_video_schema.categories_to_videos OWNER TO svh;
-- ddl-end --

-- object: videos_fk | type: CONSTRAINT --
-- ALTER TABLE svh_video_schema.categories_to_videos DROP CONSTRAINT IF EXISTS videos_fk CASCADE;
ALTER TABLE svh_video_schema.categories_to_videos ADD CONSTRAINT videos_fk FOREIGN KEY (id_videos)
REFERENCES svh_video_schema.videos (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: categories_fk | type: CONSTRAINT --
-- ALTER TABLE svh_video_schema.categories_to_videos DROP CONSTRAINT IF EXISTS categories_fk CASCADE;
ALTER TABLE svh_video_schema.categories_to_videos ADD CONSTRAINT categories_fk FOREIGN KEY (id_categories)
REFERENCES svh_video_schema.categories (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: svh_video_schema.video_type | type: TYPE --
-- DROP TYPE IF EXISTS svh_video_schema.video_type CASCADE;
CREATE TYPE svh_video_schema.video_type AS
(
 id bigint,
 created_at timestamptz,
 owner uuid,
 title text,
 descrpt text,
 age_rating text
);
-- ddl-end --

-- object: svh_video_schema.video_tag_type | type: TYPE --
-- DROP TYPE IF EXISTS svh_video_schema.video_tag_type CASCADE;
CREATE TYPE svh_video_schema.video_tag_type AS
(
 video_id bigint,
 tag_id bigint
);
-- ddl-end --

-- object: svh_video_schema.visibility_modes | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.visibility_modes CASCADE;
CREATE TABLE svh_video_schema.visibility_modes (
	id serial NOT NULL,
	title text NOT NULL,
	CONSTRAINT visibility_modes_pk PRIMARY KEY (id)
);
-- ddl-end --
ALTER TABLE svh_video_schema.visibility_modes OWNER TO svh;
-- ddl-end --

-- object: svh_video_schema.visibility_modes_to_video | type: TABLE --
-- DROP TABLE IF EXISTS svh_video_schema.visibility_modes_to_video CASCADE;
CREATE TABLE svh_video_schema.visibility_modes_to_video (
	id_videos bigint,
	id_visibility_modes integer

);
-- ddl-end --
ALTER TABLE svh_video_schema.visibility_modes_to_video OWNER TO svh;
-- ddl-end --

-- object: videos_fk | type: CONSTRAINT --
-- ALTER TABLE svh_video_schema.visibility_modes_to_video DROP CONSTRAINT IF EXISTS videos_fk CASCADE;
ALTER TABLE svh_video_schema.visibility_modes_to_video ADD CONSTRAINT videos_fk FOREIGN KEY (id_videos)
REFERENCES svh_video_schema.videos (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: visibility_modes_fk | type: CONSTRAINT --
-- ALTER TABLE svh_video_schema.visibility_modes_to_video DROP CONSTRAINT IF EXISTS visibility_modes_fk CASCADE;
ALTER TABLE svh_video_schema.visibility_modes_to_video ADD CONSTRAINT visibility_modes_fk FOREIGN KEY (id_visibility_modes)
REFERENCES svh_video_schema.visibility_modes (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --


