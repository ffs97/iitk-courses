public class PassengerVehicle extends Automobile {
	public short max_capacity = 0;

	public void setMaximumCapacity(short max_capacity) {
		this.max_capacity = max_capacity;
	}
	public short getMaximumCapacity() {
		return this.max_capacity;
	}

	public String toString() {
        return "Passenger Vehicle, Registration# " + this.reg_number + ", No. of wheels - " + this.num_wheels + ", Fuel Type - " + this.fuel_type + ", Maximum Capacity - " + this.max_capacity;
    }
}