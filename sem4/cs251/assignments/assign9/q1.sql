.open 'iitk.db'
PRAGMA foreign_keys = ON;

CREATE TABLE departments(
	department_id integer PRIMARY KEY NOT NULL,
	department text NOT NULL UNIQUE,
	head integer
);

CREATE TABLE hostels(
	hostel_id integer PRIMARY KEY NOT NULL,
	hostel text NOT NULL UNIQUE,
	married integer NOT NULL,
	gender integer NOT NULL
);

CREATE TABLE students(
	roll_no integer PRIMARY KEY NOT NULL,
	name text NOT NULL,
	email text NOT NULL,
	department integer NOT NULL,
	gender integer NOT NULL,
	hostel integer NOT NULL,
	phone text,
	degree text,
	married_status integer NOT NULL,
	advisor_1 integer,
	advisor_2 integer,
	FOREIGN KEY(department) REFERENCES departments(department_id),
	FOREIGN KEY(hostel) REFERENCES hostels(hostel_id)
);

CREATE TRIGGER "check_advisors" BEFORE INSERT ON students
BEGIN
	SELECT RAISE(ROLLBACK, 'UG STUDENTS MUST HAVE VALID ADVISORS')
	WHERE new.degree = 'UG' AND (SELECT COUNT(f.pfid)
		FROM staff_faculty AS f
		WHERE (f.pfid != new.advisor_1 AND f.pfid != new.advisor_2) AND ((new.advisor_1 != '' AND new.advisor_1 != NULL) OR (new.advisor_2 != '' AND new.advisor_2 != NULL))) > 0;

	SELECT RAISE(ROLLBACK, 'PG STUDENTS MUST HAVE AN ADVISOR IN THEIR DEPARTMENT')
	WHERE new.degree = 'PG' AND (SELECT COUNT(f.pfid)
		FROM staff_faculty AS f
		WHERE (f.pfid = new.advisor_1 OR f.pfid = new.advisor_2) AND (f.department = new.department)) < 1;

	SELECT RAISE(ROLLBACK, 'STUDENTS MUST BE PLACED IN THE RIGHT HOSTELS')
	WHERE ((SELECT COUNT(*) FROM hostels AS h WHERE h.hostel_id = new.hostel AND h.married = new.married_status) = 0) OR
		((SELECT count(*) FROM hostels AS h WHERE h.hostel_id = new.hostel AND h.gender = new.gender) = 0 );
END;

CREATE TABLE staff_faculty(
	pfid integer PRIMARY KEY NOT NULL,
	name text NOT NULL,
	email text NOT NULL,
	department integer NOT NULL,
	address text NOT NULL,
	phone text,
	is_faculty integer NOT NULL,
	FOREIGN KEY(department) REFERENCES departments(department_id)
);

CREATE TABLE core_members(
	core_id integer PRIMARY KEY NOT NULL,
	name text NOT NULL,
	member_type integer NOT NULL,
	email text NOT NULL UNIQUE
);

CREATE TRIGGER core_members_add_students AFTER INSERT ON students
BEGIN
	INSERT INTO core_members(core_id, name, member_type, email) VALUES (new.roll_no, new.name, 1, new.email);
END;

CREATE TRIGGER core_members_add_staff AFTER INSERT ON staff_faculty
BEGIN
	INSERT INTO core_members(core_id, name, member_type, email) VALUES (new.pfid, new.name, 0, new.email);
END;

CREATE TABLE family(
	uid integer NOT NULL,
	name text NOT NULL,
	core_id integer NOT NULL,
	relation text NOT NULL,
	FOREIGN KEY(core_id) REFERENCES core_members(core_id),
	PRIMARY KEY(uid, core_id)
);
