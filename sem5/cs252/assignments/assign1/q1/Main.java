public class Main {
	public static void main(String args[]) {
		PassengerVehicle pVehicle = new PassengerVehicle();
		pVehicle.setRegistrationNumber("UP78AA1111");
		pVehicle.setNumberOfWheels((short) 4);
		pVehicle.setFuelType(Automobile.FuelType.Petrol);
		pVehicle.setMaximumCapacity((short) 5);

		pVehicle.setRegistrationNumber("UP78AA1111-12.");

		System.out.println(pVehicle);

		TransportVehicle tVehicle = new TransportVehicle();
		tVehicle.setRegistrationNumber("UP78BB2222");
		tVehicle.setNumberOfWheels((short) 6);
		tVehicle.setFuelType(Automobile.FuelType.Diesel);
		tVehicle.setMaximumLoad(9000);

		System.out.println(tVehicle);
	}
}