.open 'iitk.db'

SELECT COUNT(*) FROM students WHERE married_status = 1;

SELECT COUNT(*) FROM students WHERE hostel in
	(SELECT hostel_id FROM hostels WHERE married = 1);

SELECT COUNT(*) FROM staff_faculty WHERE is_faculty = 1;
SELECT COUNT(*) FROM staff_faculty WHERE is_faculty = 0;
SELECT COUNT(*) FROM students;
SELECT COUNT(DISTINCT(uid)) FROM family;
SELECT
	((SELECT COUNT(*) FROM core_members) +
	(SELECT COUNT(DISTINCT(uid)) FROM family));

SELECT
	(SELECT COUNT(*) FROM family WHERE core_id in
		(SELECT pfid FROM staff_faculty WHERE is_faculty = 1)) / 
	((SELECT COUNT(*) FROM staff_faculty WHERE is_faculty = 1) * 1.0);

SELECT
	((SELECT COUNT(*) c FROM departments GROUP BY head HAVING c > 1) > 0);

SELECT
	(((SELECT COUNT(*) FROM students WHERE degree = 'UG' AND (advisor_1 != '' OR advisor_2 != '')) * 100.0) /
	(SELECT COUNT(*) FROM students WHERE degree = 'UG'));

-- SELECT COUNT(*) FROM staff_faculty WHERE (SELECT(SELECT COUNT(*) FROM students WHERE advisor_1 = sta))

SELECT COUNT(DISTINCT(a.pfid)) FROM students s, staff_faculty a
	WHERE (a.pfid = s.advisor_1 OR a.pfid = s.advisor_2) AND (a.department != s.department);
