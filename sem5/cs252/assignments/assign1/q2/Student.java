public class Student {
	public String roll_number = "";
	public short semester = 0;
	public float cgpa = 0;

	public void setRollNumber(String roll_number) {
		if (roll_number.matches("[A-Za-z0-9]+")) {
			this.roll_number = roll_number;
		}
		else {
			System.out.println("Invalid Roll Number" + roll_number);
		}
	}
	public String getRollNumber() {
		return this.roll_number;
	}

	public void setSemester(short semester) {
		this.semester = semester;
	}
	public short getSemester() {
		return this.semester;
	}

	public void setCGPA(float cgpa) {
		this.cgpa = cgpa;
	}
	public float getCGPA() {
		return this.cgpa;
	}

	public String toString() {
		return this.roll_number;
	}
}
