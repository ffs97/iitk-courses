public class TransportVehicle extends Automobile {
	public double max_load = 0;

	public void setMaximumLoad(double max_load) {
		this.max_load = max_load;
	}
	public double getMaximumLoad() {
		return this.max_load;
	}

	public String toString() {
        return "Transport Vehicle, Registration# " + this.reg_number + ", No. of wheels - " + this.num_wheels + ", Fuel Type - " + this.fuel_type + ", Maximum Load - " + this.max_load + " Kg";
    }
}