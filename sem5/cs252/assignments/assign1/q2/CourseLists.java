import java.util.*;

public class CourseLists {
	public static Map<Course, List<Student>> courseMap = new HashMap<Course, List<Student>>();

	public static void addCourseMapping(Course c, Student s) {
		List<Student> students = courseMap.get(c);
	    if (students == null) {
	        students = new ArrayList<>();
	        courseMap.put(c, students);
	    }
	    students.add(s);
	}
}