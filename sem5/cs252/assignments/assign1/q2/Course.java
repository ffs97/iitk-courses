public class Course {
	short course_number = 0;
	String description = "";

	public void setCourseDescription(String description) {
		this.description = description;
	}
	public String getCourseDescription() {
		return this.description;
	}

	public void setCourseNumber(short course_number) {
		this.course_number = course_number;
	}
	public short getCourseNumber() {
		return this.course_number;
	}

	public String toString() {
		return this.course_number + "";
	}
}