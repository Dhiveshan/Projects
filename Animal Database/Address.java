/** A class that represents the address of the animal shelter.
 *  Provided to you (no need to modify) */
public class Address {
	private String streetAddress;
	private String city;
	private String zipcode;

	Address(String stringaddr, String city, String zipcode) {
		this.city = city;
		this.zipcode = zipcode;
		this.streetAddress = stringaddr;
	}

	public String getCity() {
		return city;
	}

	public String getZipCode() {
		return zipcode;
	}

	public String getStreetAddress() {
		return streetAddress;
	}

	public String toString() {
		return streetAddress + "," + System.lineSeparator() + city + " " + zipcode;
	}

}
