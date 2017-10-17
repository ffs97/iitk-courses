abstract public class Automobile {
	public enum FuelType { 
		Petrol, Diesel, CNG
	}

	public String reg_number = "";
	public short num_wheels = 0;
	public FuelType fuel_type = FuelType.Petrol; 

	public void setRegistrationNumber(String reg_number) {
		if (reg_number.matches("[A-Za-z0-9]+")) {
			this.reg_number = reg_number;
		}
		else {
			System.out.println("Invalid Registration Number");
		}
	}
	public String getRegistrationNumber() {
		return this.reg_number;
	}

	public void setNumberOfWheels(short num_wheels) {
		this.num_wheels = num_wheels;
	}
	public short getNumberOfWheels() {
		return this.num_wheels;
	}

	public void setFuelType(FuelType fuel_type) {
		this.fuel_type = fuel_type;
	}
	public FuelType getFuelType() {
		return this.fuel_type;
	}
}
