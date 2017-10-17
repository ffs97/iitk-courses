public class Main {
	public static void main(String args[]) {
		Student s = new Student();
		s.setRollNumber("123");
		s.setSemester((short) 3);
		s.setCGPA((float) 8.5);

		Student s2 = new Student();
		s2.setRollNumber("42");
		s2.setSemester((short) 3);
		s2.setCGPA((float) 8.5);

		Course c = new Course();
		c.setCourseNumber((short) 5);
		c.setCourseDescription("CS252");

		CourseLists.addCourseMapping(c, s);
		CourseLists.addCourseMapping(c, s2);

		System.out.println(CourseLists.courseMap);
	}
}